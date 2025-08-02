#ifndef SPEEDREPORTINGSERVICE_H
#define SPEEDREPORTINGSERVICE_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDateTime>

class SpeedReportingService : public QObject
{
    Q_OBJECT

public:
    explicit SpeedReportingService(QObject *parent = nullptr);
    ~SpeedReportingService();

    void startReporting(const QString &endpoint = "broker.hivemq.com:1883");
    void stopReporting();
    bool isReporting() const;
    void setSpeedThreshold(double threshold) { m_speedThreshold = threshold; }
    double speedThreshold() const { return m_speedThreshold; }

public slots:
    void onSpeedChanged(double speed);

signals:
    void reportingStatusChanged(bool reporting);
    void speedReported(const QString &endpoint, const QString &message);
    void errorOccurred(const QString &error);

private slots:

private:
    QTcpSocket *m_socket;
    QTimer *m_reportingTimer;
    double m_speedThreshold;
    double m_lastSentSpeed;
    QString m_endpoint;
    bool m_isReporting;
    bool m_isSubscribed;
    QString m_clientId;
    int m_packetId;

    QByteArray encodeRemainingLength(int length);
    void setupReportingTimer();
    void sendSpeedData(double speed);
    QString formatSpeedMessage(double speed);
    void sendMqttConnect();
    void sendMqttPublish(const QString &topic, const QString &message);
    void sendMqttSubscribe(const QString &topic);
    void sendMqttPingReq();
    void handleMqttMessage(const QByteArray &packet);
};

#endif 