#ifndef SPEEDREPORTINGSERVICE_H
#define SPEEDREPORTINGSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QDateTime>

class SpeedReportingService : public QObject
{
    Q_OBJECT

public:
    explicit SpeedReportingService(QObject *parent = nullptr);
    ~SpeedReportingService();

    void startReporting(const QString &endpoint = "http://httpbin.org/post");
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
    void onNetworkReplyFinished(QNetworkReply *reply);
    void onNetworkError(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *m_networkManager;
    QTimer *m_reportingTimer;
    double m_speedThreshold;
    double m_lastSentSpeed;
    QString m_endpoint;
    bool m_isReporting;

    void setupReportingTimer();
    void sendSpeedData(double speed);
    QString formatSpeedMessage(double speed);
};

#endif 