#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QMutex>
#include <memory>

class SpeedModel;

class SpeedController : public QObject
{
    Q_OBJECT

public:
    explicit SpeedController(QObject* parent = nullptr);
    ~SpeedController();

  
    bool initialize();
    void shutdown();
    void update();
    
   
    void addSpeedData(double speed, const QDateTime& timestamp);
    double getCurrentSpeed() const;
    double getAverageSpeed() const;
    double getMaxSpeed() const;
    double getMinSpeed() const;
    
    
    double calculateAverageSpeed(int windowSize = 10) const;
    double calculateInstantaneousSpeed() const;
    double calculateAcceleration() const;
    double calculateDeceleration() const;
    
   
    bool isSpeedValid(double speed) const;
    bool isSpeedInRange(double speed) const;
    QString validateSpeed(double speed) const;
    
    
    void setSpeedLimit(double limit);
    double getSpeedLimit() const;
    void setSpeedThreshold(double threshold);
    double getSpeedThreshold() const;
    void setSpeedTolerance(double tolerance);
    double getSpeedTolerance() const;
    
    
    bool isSpeedLimitExceeded() const;
    bool isSpeedThresholdReached() const;
    void enableSpeedAlerts(bool enable);
    bool areSpeedAlertsEnabled() const;
    
    
    void clearSpeedData();
    int getSpeedDataCount() const;
    QVector<QPair<double, QDateTime>> getSpeedData() const;
    QVector<QPair<double, QDateTime>> getSpeedData(int count) const;
    
   
    void calculateStatistics();
    double getAverageSpeedForPeriod(const QDateTime& start, const QDateTime& end) const;
    double getMaxSpeedForPeriod(const QDateTime& start, const QDateTime& end) const;
    double getMinSpeedForPeriod(const QDateTime& start, const QDateTime& end) const;
    
   
    bool exportSpeedData(const QString& filePath);
    bool importSpeedData(const QString& filePath);

signals:
    void speedDataAdded(double speed, const QDateTime& timestamp);
    void currentSpeedChanged(double speed);
    void averageSpeedChanged(double averageSpeed);
    void maxSpeedChanged(double maxSpeed);
    void minSpeedChanged(double minSpeed);
    void speedLimitExceeded(double speed, double limit);
    void speedThresholdReached(double speed, double threshold);
    void speedAlertTriggered(const QString& alert);
    void statisticsUpdated();
    void errorOccurred(const QString& error);

private slots:
    void onUpdateTimer();
    void onStatisticsTimer();

private:
   
    void processSpeedData();
    void updateStatistics();
    void checkSpeedAlerts(double speed);
    
   
    void addSpeedDataInternal(double speed, const QDateTime& timestamp);
    void removeOldData();
    void maintainDataBuffer();
    
   
    double calculateMovingAverage(int windowSize) const;
    double calculateStandardDeviation() const;
    bool isDataStale(const QDateTime& timestamp) const;
    
   
    QVector<QPair<double, QDateTime>> m_speedData;
    QMutex m_dataMutex;
    int m_maxDataPoints;
    
    
    double m_currentSpeed;
    double m_previousSpeed;
    double m_averageSpeed;
    double m_maxSpeed;
    double m_minSpeed;
    
   
    double m_speedLimit;
    double m_speedThreshold;
    double m_speedTolerance;
    bool m_speedAlertsEnabled;
    
   
    QTimer* m_updateTimer;
    QTimer* m_statisticsTimer;
    
    
    int m_updateInterval;
    int m_statisticsInterval;
    int m_dataRetentionPeriod;
    bool m_autoCalculateStatistics;
    
   
    double m_totalDistance;
    double m_totalTime;
    double m_standardDeviation;
    int m_speedLimitViolations;
    int m_speedThresholdEvents;
    
   
    SpeedModel* m_speedModel;
    
   
    bool m_initialized;
    QDateTime m_lastUpdateTime;
    QDateTime m_sessionStartTime;
};

#endif 