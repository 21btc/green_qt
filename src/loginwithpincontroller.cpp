#include "loginwithpincontroller.h"

#include "context.h"
#include "network.h"
#include "networkmanager.h"
#include "session.h"
#include "task.h"
#include "wallet.h"
#include "walletmanager.h"

#include <QJsonDocument>

LoginController::LoginController(QObject* parent)
    : Controller(parent)
{
}

void LoginController::setWallet(Wallet* wallet)
{
    if (m_wallet == wallet) return;
    m_wallet = wallet;
    emit walletChanged();
    update();
}

void LoginController::update()
{
    if (!m_wallet) return;
    if (m_pin.isEmpty()) return;

    if (!m_context) {
        setContext(new Context(this));
    }
    m_context->setWallet(m_wallet);

    login();
}

void LoginController::loginWithPin(const QString& pin)
{
    m_pin = pin;
    update();
}

void LoginController::login()
{
    clearErrors();
    auto network = m_wallet->network();

    auto group = new TaskGroup(this);
    group->setName("login");

    login(group, network);

    m_dispatcher->add(group);

    connect(group, &TaskGroup::failed, this, &LoginController::loginFailed);
    connect(group, &TaskGroup::finished, this, [=] {
        emit loginFinished(m_context);
        setContext(nullptr);
    });
}

void LoginController::login(TaskGroup* group, Network* network)
{
    auto session = m_context->getOrCreateSession(network);
    auto connect_session = new ConnectTask(session);
    auto pin_login = new LoginTask(m_pin, m_wallet->pinData(), session);
    auto get_credentials = new GetCredentialsTask(session);

    connect_session->then(pin_login);
    pin_login->then(get_credentials);

    connect(connect_session, &Task::failed, this, [=](const QString& error) {
        if (error == "timeout error") {
            emit sessionError("id_connection_failed");
        }
    });

    connect(pin_login, &Task::failed, this, [=](const QString& error) {
        if (error == "id_invalid_pin") {
            m_wallet->decrementLoginAttempts();
            emit invalidPin();
        } else if (error == "id_connection_failed") {
            emit sessionError(error);
        }
        emit loginFailed();
    });

    connect(pin_login, &Task::finished, this, [=] {
        m_wallet->resetLoginAttempts();
    });

    group->add(connect_session);
    group->add(pin_login);
    group->add(get_credentials);
}

LoadController::LoadController(QObject* parent)
    : Controller(parent)
    , m_monitor(new TaskGroupMonitor(this))
{
    connect(m_monitor, &TaskGroupMonitor::allFinishedOrFailed, this, &LoadController::loadFinished);
}

static bool compatibleToNetworks(Network* network, const QList<Network*> networks)
{
    for (auto net : networks) {
        if (net == network) return false;
        if (net->isMainnet() != network->isMainnet()) return false;
        if (net->isDevelopment() != network->isDevelopment()) return false;
    }
    return true;
}

void LoadController::load()
{
    const auto networks = m_context->getActiveNetworks();
    const auto sessions = m_context->getSessions();

    auto group = new TaskGroup(this);

    for (auto network : networks) {
        loadNetwork(group, network);
    }

    m_monitor->add(group);
    m_context->dispatcher()->add(group);

    connect(group, &TaskGroup::finished, this, [=] {
        auto wallet = m_context->wallet();
        Q_ASSERT(wallet);
        WalletManager::instance()->addWallet(wallet);
        wallet->setContext(m_context);
    });

    if (m_context->credentials().contains("mnemonic")) {
        for (auto network : NetworkManager::instance()->networks()) {
            if (compatibleToNetworks(network, networks)) {
                qDebug() << Q_FUNC_INFO << "ATTEMPT LOGIN" << network->id() << network->name();
                loginNetwork(network);
            }
        }
    }
}

void LoadController::loadNetwork(TaskGroup* group, Network* network)
{
    auto session = m_context->getOrCreateSession(network);
    group->add(new GetWatchOnlyDetailsTask(session));
    group->add(new LoadTwoFactorConfigTask(session));
    group->add(new LoadCurrenciesTask(session));
    if (network->isLiquid()) group->add(new LoadAssetsTask(session));
    auto load_accounts = new LoadAccountsTask(false, session);
    connect(load_accounts, &Task::finished, this, [=] {
        for (auto account : load_accounts->accounts()) {
            group->add(new LoadBalanceTask(account));
        }
    });
    group->add(load_accounts);
}

void LoadController::loginNetwork(Network* network)
{
    auto group = new TaskGroup(this);

    const auto mnemonic = m_context->credentials().value("mnemonic").toString().split(' ');

    auto session = m_context->getOrCreateSession(network);
    auto connect_session = new ConnectTask(session);
    auto login = new LoginTask(mnemonic, QString(), session);

    connect_session->then(login);

    connect(connect_session, &Task::failed, this, [=](const QString& error) {
        if (error == "timeout error") {
            setError("session", "id_connection_failed");
        }
    });

    connect(login, &Task::finished, this, [=] {
        qDebug() << "FINISHED LOGIN" << network->id();
        loadNetwork(group, network);
    });

    connect(login, &Task::failed, this, [=](const QString& error) {
        qDebug() << "ignoring login failed for network" << network->id() << "errr:" << error;
//        emit loginFailed();
    });

    group->add(connect_session);
    group->add(login);

    m_monitor->add(group);
    m_context->dispatcher()->add(group);
}

PinDataController::PinDataController(QObject* parent)
    : Controller(parent)
{
}

void PinDataController::update(const QString& pin)
{
    if (!m_context) return;

    Q_ASSERT(m_context->wallet());

    auto session = m_context->primarySession();
    Q_ASSERT(session);

    auto task = new EncryptWithPinTask(m_context->credentials(), pin, session);
    connect(task, &Task::finished, this, [=] {
        const auto pin_data = task->result().value("result").toObject().value("pin_data").toObject();

        m_context->wallet()->setPinData(session->network(), QJsonDocument(pin_data).toJson());

        emit finished();
    });
    m_context->dispatcher()->add(task);
}
