#ifndef GREEN_CREATEACCOUNTCONTROLLER_H
#define GREEN_CREATEACCOUNTCONTROLLER_H

#include "controller.h"

#include <QtQml>

class Account;

class CreateAccountController : public Controller
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    QML_ELEMENT
public:
    explicit CreateAccountController(QObject *parent = nullptr);

    QString name() const;
    void setName(const QString& name);

    QString type() const { return m_type; }
    void setType(const QString& type);

signals:
    void nameChanged(const QString& name);
    void typeChanged(const QString& type);
    void accountCreated(Account* account);

public slots:
    void create();

protected:
    bool update(const QJsonObject& result) override;

private:
    QString m_name;
    QString m_type;
};

#endif // GREEN_CREATEACCOUNTCONTROLLER_H
