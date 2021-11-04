#ifndef GREEN_JADEUPDATECONTROLLER_H
#define GREEN_JADEUPDATECONTROLLER_H

#include "httprequestactivity.h"

#include <QtQml>
#include <QObject>

QT_FORWARD_DECLARE_CLASS(Activity)
QT_FORWARD_DECLARE_CLASS(JadeDevice)
QT_FORWARD_DECLARE_CLASS(Session)

class JadeHttpRequestActivity : public HttpRequestActivity
{
    Q_OBJECT
    QML_ELEMENT
public:
    JadeHttpRequestActivity(const QString& path, Session* session);
};

class JadeChannelRequestActivity : public JadeHttpRequestActivity
{
    Q_OBJECT
    QML_ELEMENT
public:
    JadeChannelRequestActivity(const QString& base, const QString& channel, Session* session);
    QVariantList firmwares() const;
private:
    const QString m_base;
};

class JadeBinaryRequestActivity : public JadeHttpRequestActivity
{
    Q_OBJECT
    QML_ELEMENT
public:
    JadeBinaryRequestActivity(const QString& path, Session* session);
};

class JadeUnlockActivity : public Activity
{
    Q_OBJECT
    Q_PROPERTY(JadeDevice* device READ device CONSTANT)
    QML_ELEMENT
public:
    JadeUnlockActivity(const QString& network, JadeDevice* device);
    JadeDevice* device() const { return m_device; }
private:
    void exec() override;
private:
    JadeDevice* const m_device;
    const QString m_network;
};

class JadeUpdateActivity : public Activity
{
    Q_OBJECT
    Q_PROPERTY(JadeDevice* device READ device CONSTANT)
    Q_PROPERTY(QVariantMap firmware READ firmware CONSTANT)
    QML_ELEMENT
public:
    JadeUpdateActivity(const QVariantMap& firmware, const QByteArray& data, JadeDevice* device);
    JadeDevice* device() const { return m_device; }
    QVariantMap firmware() const { return m_firmware; }
private:
    void exec() override;
signals:
    void locked();
private:
    JadeDevice* const m_device;
    const QVariantMap m_firmware;
    const QByteArray m_data;
    qlonglong m_uploaded{0};
};

class JadeUpdateController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Session* session READ session NOTIFY sessionChanged)
    Q_PROPERTY(JadeDevice* device READ device WRITE setDevice NOTIFY deviceChanged)
    Q_PROPERTY(QString channel READ channel WRITE setChannel NOTIFY channelChanged)
    Q_PROPERTY(QVariantList firmwares READ firmwares NOTIFY firmwaresChanged)
    QML_ELEMENT
public:
    explicit JadeUpdateController(QObject *parent = nullptr);
    Session* session() const { return m_session; }
    JadeDevice* device() const { return m_device; }
    void setDevice(JadeDevice* device);
    QString channel() const { return m_channel; }
    void setChannel(const QString& channel);
    QVariantList firmwares() const { return m_firmwares; }
public slots:
    void disconnectDevice();
    void check();
    void update(const QVariantMap& firmware);
    JadeUnlockActivity *unlock();
signals:
    void activityCreated(Activity* activity);
    void sessionChanged(Session* session);
    void deviceChanged(JadeDevice* device);
    void channelChanged(QString channel);
    void firmwaresChanged(const QVariantList& firmwares);
protected:
    void pushActivity(Activity* activity);
    void popActivity();
private:
    Session* m_session{nullptr};
    JadeDevice* m_device{nullptr};
    QString m_channel;
    QVariantList m_firmwares;
    QMap<QString, QByteArray> m_firmware_data;
};

#endif // GREEN_JADEUPDATECONTROLLER_H
