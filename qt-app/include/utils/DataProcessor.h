#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QMutex>
#include <QThread>
#include <memory>

struct ProcessedData {
    double originalValue;
    double processedValue;
    QDateTime timestamp;
    QString processingMethod;
    bool isValid;
    QString notes;
};

struct DataStatistics {
    double mean;
    double median;
    double standardDeviation;
    double variance;
    double minValue;
    double maxValue;
    double range;
    int dataPoints;
    QDateTime startTime;
    QDateTime endTime;
    double totalSum;
    double totalSquaredSum;
};

struct DataFilter {
    QString name;
    QString type;
    double parameter1;
    double parameter2;
    bool isEnabled;
    QString description;
};

class DataProcessor : public QObject
{
    Q_OBJECT

public:
    explicit DataProcessor(QObject* parent = nullptr);
    ~DataProcessor();

    QVector<double> filterData(const QVector<double>& data, const QString& filterType, double parameter = 0.0);
    QVector<double> smoothData(const QVector<double>& data, int windowSize = 5);
    QVector<double> normalizeData(const QVector<double>& data);
    QVector<double> removeOutliers(const QVector<double>& data, double threshold = 2.0);
    QVector<double> interpolateData(const QVector<double>& data, int targetSize);
    
    DataStatistics calculateStatistics(const QVector<double>& data);
    DataStatistics calculateStatistics(const QVector<double>& data, const QDateTime& start, const QDateTime& end);
    double calculateMean(const QVector<double>& data);
    double calculateMedian(const QVector<double>& data);
    double calculateStandardDeviation(const QVector<double>& data);
    double calculateVariance(const QVector<double>& data);
    double calculateCorrelation(const QVector<double>& data1, const QVector<double>& data2);
    
    bool isValidData(const QVector<double>& data);
    QVector<bool> validateDataPoints(const QVector<double>& data, double minValue, double maxValue);
    QString validateDataRange(const QVector<double>& data, double minValue, double maxValue);
    
    QVector<double> convertUnits(const QVector<double>& data, const QString& fromUnit, const QString& toUnit);
    double convertSpeed(double speed, const QString& fromUnit, const QString& toUnit);
    double convertDistance(double distance, const QString& fromUnit, const QString& toUnit);
    double convertTime(double time, const QString& fromUnit, const QString& toUnit);
    
    bool exportToCsv(const QVector<double>& data, const QString& filePath, const QStringList& headers = QStringList());
    bool exportToJson(const QVector<double>& data, const QString& filePath);
    QVector<double> importFromCsv(const QString& filePath, int columnIndex = 0);
    QVector<double> importFromJson(const QString& filePath);
    
    void startRealTimeProcessing();
    void stopRealTimeProcessing();
    bool isRealTimeProcessing() const;
    void addRealTimeData(double value, const QDateTime& timestamp = QDateTime::currentDateTime());
    QVector<ProcessedData> getProcessedData() const;
    
    void addFilter(const DataFilter& filter);
    void removeFilter(const QString& filterName);
    void enableFilter(const QString& filterName, bool enable);
    QVector<DataFilter> getFilters() const;
    void clearFilters();
    
    void setProcessingMode(const QString& mode);
    QString getProcessingMode() const;
    void setBatchSize(int size);
    int getBatchSize() const;
    void setProcessingInterval(int interval);
    int getProcessingInterval() const;
    void setAutoProcessing(bool autoProcess);
    bool isAutoProcessing() const;

signals:
    void dataProcessed(const QVector<ProcessedData>& processedData);
    void statisticsCalculated(const DataStatistics& statistics);
    void filterApplied(const QString& filterName, const QVector<double>& filteredData);
    void realTimeDataProcessed(const ProcessedData& data);
    void processingStarted();
    void processingFinished();
    void errorOccurred(const QString& error);

private slots:
    void onProcessingTimer();
    void onRealTimeProcessing();

private:
    void processBatch(const QVector<double>& data);
    void applyFilters(QVector<double>& data);
    void updateStatistics(const QVector<double>& data);
    
    QVector<double> applyMovingAverageFilter(const QVector<double>& data, int windowSize);
    QVector<double> applyMedianFilter(const QVector<double>& data, int windowSize);
    QVector<double> applyGaussianFilter(const QVector<double>& data, double sigma);
    QVector<double> applyKalmanFilter(const QVector<double>& data, double processNoise, double measurementNoise);
    
    QVector<double> sortData(const QVector<double>& data);
    double calculatePercentile(const QVector<double>& data, double percentile);
    bool isOutlier(double value, const QVector<double>& data, double threshold);
    QString generateProcessingReport(const QVector<double>& originalData, const QVector<double>& processedData);
    
    void processRealTimeData();
    void addToRealTimeBuffer(double value, const QDateTime& timestamp);
    void flushRealTimeBuffer();
    
    QVector<ProcessedData> m_processedData;
    QVector<DataFilter> m_filters;
    QVector<QPair<double, QDateTime>> m_realTimeBuffer;
    QMutex m_dataMutex;
    QMutex m_processingMutex;
    
    QString m_processingMode;
    int m_batchSize;
    int m_processingInterval;
    bool m_autoProcessing;
    bool m_realTimeProcessing;
    
    QTimer* m_processingTimer;
    QTimer* m_realTimeTimer;
    
    DataStatistics m_currentStatistics;
    int m_totalProcessedPoints;
    QDateTime m_processingStartTime;
    
    QThread* m_processingThread;
    bool m_isProcessing;
    
    static const QString DEFAULT_PROCESSING_MODE;
    static const int DEFAULT_BATCH_SIZE;
    static const int DEFAULT_PROCESSING_INTERVAL;
    static const int REAL_TIME_BUFFER_SIZE;
    static const int REAL_TIME_PROCESSING_INTERVAL;
};

#endif 