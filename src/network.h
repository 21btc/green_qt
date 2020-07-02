#ifndef GREEN_NETWORK_H
#define GREEN_NETWORK_H

#include <QtQml>
#include <QJsonObject>
#include <QObject>
#include <QQmlListProperty>

class Network;

class NetworkManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Network> networks READ networks CONSTANT)
public:
    static NetworkManager* instance();

    QQmlListProperty<Network> networks();

    Network* network(const QString& id) const;

private:
    explicit NetworkManager();
    QList<Network*> m_networks;
};

class Network : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(bool liquid READ isLiquid CONSTANT)
    QML_ELEMENT
    QML_UNCREATABLE("Network is instanced by NetworkManager.")
public:
    Network(const QJsonObject& data, NetworkManager* manager);

    QJsonObject data() const { return m_data; }
    QString id() const { return m_id; }
    QString name() const { return m_name; }
    bool isLiquid() const;

    void openTransactionInExplorer(const QString& hash);

private:
    const QJsonObject m_data;
    QString m_id;
    QString m_name;
};

#endif // GREEN_NETWORK_H
