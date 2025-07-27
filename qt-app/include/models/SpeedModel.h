#ifndef SPEEDMODEL_H
#define SPEEDMODEL_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QMutex>
#include <QTimer>

struct SpeedData {
    double speed;
    QDateTime timestamp;
    QString unit;
    bool isValid;
    QString source;
    double accuracy;
    QString notes;
};

struct SpeedStatistics {
    double currentSpeed;
    double averageSpeed;
    double maxSpeed;
    double minSpeed;
    double medianSpeed;
    double standardDeviation;
    double totalDistance;
    double totalTime;
    int dataPoints;
    QDateTime startTime;
    QDateTime endTime;
    QDateTime lastUpdateTime;
};

struct SpeedAlert {
    QString type;
    QString message;
    QDateTime timestamp;
    double speed;
    double threshold;
    bool isActive;
    QString severity;
};

class SpeedModel : public QObject
{
    Q_OBJECT

public:
    explicit SpeedModel(QObject* parent = nullptr);
    ~SpeedModel();

    void addSpeedData(const SpeedData& data);
    void addSpeedData(double speed, const QDateTime& timestamp = QDateTime::currentDateTime());
    SpeedData getLatestSpeedData() const;
    QVector<SpeedData> getAllSpeedData() const;
    QVector<SpeedData> getSpeedData(int count) const;
    QVector<SpeedData> getSpeedData(const QDateTime& start, const QDateTime& end) const;
    
    
    SpeedStatistics getStatistics() const;
    SpeedStatistics getStatistics(const QDateTime& start, const QDateTime& end) const;
    void calculateStatistics();
    void resetStatistics();
    
  
    double getCurrentSpeed() const;
    double getAverageSpeed() const;
    double getMaxSpeed() const;
    double getMinSpeed() const;
    double getMedianSpeed() const;
    double getStandardDeviation() const;
    
   
    double calculateAverageSpeed(int windowSize = 10) const;
    double calculateMovingAverage(int windowSize = 10) const;
    double calculateAcceleration() const;
    double calculateDeceleration() const;
    double calculateTotalDistance() const;
    double calculateTotalTime() const;
    
   
    bool isSpeedValid(double speed) const;
    bool isSpeedInRange(double speed) const;
    QString validateSpeed(double speed) const;
    void setSpeedRange(double minSpeed, double maxSpeed);
    double getMinSpeedRange() const;
    double getMaxSpeedRange() const;
    
    
    void setSpeedUnit(const QString& unit);
    QString getSpeedUnit() const;
    double convertSpeed(double speed, const QString& fromUnit, const QString& toUnit) const;
    
   
    void addSpeedAlert(const SpeedAlert& alert);
    void clearSpeedAlerts();
    QVector<SpeedAlert> getActiveAlerts() const;
    QVector<SpeedAlert> getAllAlerts() const;
    bool hasActiveAlerts() const;
    
    
    void clearAllData();
    void removeOldData(int maxAgeSeconds);
    void setMaxDataPoints(int maxPoints);
    int getMaxDataPoints() const;
    int getDataPointCount() const;
    
    
    bool exportToFile(const QString& filePath);
    bool importFromFile(const QString& filePath);
    QString toJson() const;
    bool fromJson(const QString& json);
    bool exportToCsv(const QString& filePath);
    
   
    void setUpdateInterval(int intervalMs);
    int getUpdateInterval() const;
    void setAutoCalculateStatistics(bool autoCalculate);
    bool isAutoCalculateStatistics() const;
    void setDataRetentionPeriod(int periodSeconds);
    int getDataRetentionPeriod() const;

signals:
    void speedDataAdded(const SpeedData& data);
    void currentSpeedChanged(double speed);
    void averageSpeedChanged(double averageSpeed);
    void maxSpeedChanged(double maxSpeed);
    void minSpeedChanged(double minSpeed);
    void statisticsUpdated(const SpeedStatistics& statistics);
    void speedAlertTriggered(const SpeedAlert& alert);
    void speedAlertCleared(const SpeedAlert& alert);
    void dataCleared();
    void dataExported(const QString& filePath);
    void dataImported(const QString& filePath);
    void errorOccurred(const QString& error);

private slots:
    void onUpdateTimer();
    void onCleanupTimer();

private:
    
    void processNewSpeedData(const SpeedData& data);
    void updateStatistics();
    void checkSpeedAlerts(double speed);
    void cleanupOldData();
    
    double calculateMedian(const QVector<double>& values) const;
    double calculateStandardDeviation(const QVector<double>& values) const;
    bool isDataStale(const QDateTime& timestamp) const;
    QString formatSpeed(double speed) const;
    
    QVector<SpeedData> m_speedData;
    SpeedStatistics m_statistics;
    QVector<SpeedAlert> m_speedAlerts;
    QMutex m_dataMutex;
    
    QString m_speedUnit;
    double m_minSpeedRange;
    double m_maxSpeedRange;
    int m_maxDataPoints;
    int m_updateInterval;
    int m_dataRetentionPeriod;
    bool m_autoCalculateStatistics;
    
    QTimer* m_updateTimer;
    QTimer* m_cleanupTimer;
    
    bool m_initialized;
    QDateTime m_lastUpdateTime;
    QDateTime m_sessionStartTime;
    
    static const double DEFAULT_MIN_SPEED;
    static const double DEFAULT_MAX_SPEED;
    static const int DEFAULT_MAX_DATA_POINTS;
    static const int DEFAULT_UPDATE_INTERVAL;
    static const int DEFAULT_DATA_RETENTION_PERIOD;
};

#endif 