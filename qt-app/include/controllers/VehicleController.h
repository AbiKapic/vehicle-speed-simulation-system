#ifndef VEHICLECONTROLLER_H
#define VEHICLECONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QQueue>
#include <QMutex>
#include <memory>

class VehicleModel;
class SpeedModel;

class VehicleController : public QObject
{
    Q_OBJECT

public:
    explicit VehicleController(QObject* parent = nullptr);
    ~VehicleController();

    
    bool initialize();
    void shutdown();
    void update();
    
    bool connectToVehicle(const QString& portName, int baudRate = 9600);
    void disconnectFromVehicle();
    bool isConnected() const;
    
    void startDataCollection();
    void stopDataCollection();
    bool isCollectingData() const;
    
    void setTargetSpeed(double speed);
    double getCurrentSpeed() const;
    double getTargetSpeed() const;
    
    QString getVehicleId() const;
    QString getVehicleType() const;
    QString getVehicleStatus() const;
    
    bool exportData(const QString& filePath);
    bool importData(const QString& filePath);
    void clearData();

signals:
    void connected();
    void disconnected();
    void connectionError(const QString& error);
    void dataReceived(const QByteArray& data);
    void speedChanged(double speed);
    void targetSpeedChanged(double targetSpeed);
    void vehicleStatusChanged(const QString& status);
    void dataCollectionStarted();
    void dataCollectionStopped();
    void errorOccurred(const QString& error);

private slots:
    void onSerialPortReadyRead();
    void onSerialPortError(QSerialPort::SerialPortError error);
    void onDataCollectionTimer();
    void onConnectionTimeout();

private:
    bool setupSerialPort();
    void processReceivedData(const QByteArray& data);
    void sendCommand(const QByteArray& command);
    void parseSpeedData(const QByteArray& data);
    
    void addSpeedData(double speed, const QDateTime& timestamp);
    void processDataQueue();
    
    bool validateSpeed(double speed) const;
    QString formatSpeedData(double speed) const;
    
    std::unique_ptr<QSerialPort> m_serialPort;
    QString m_portName;
    int m_baudRate;
    bool m_isConnected;
    
    QTimer* m_dataCollectionTimer;
    QTimer* m_connectionTimeoutTimer;
    bool m_isCollectingData;
    QQueue<QPair<double, QDateTime>> m_speedDataQueue;
    QMutex m_dataMutex;
    
    double m_currentSpeed;
    double m_targetSpeed;
    double m_maxSpeed;
    double m_minSpeed;
    
    QString m_vehicleId;
    QString m_vehicleType;
    QString m_vehicleStatus;
    
    VehicleModel* m_vehicleModel;
    SpeedModel* m_speedModel;
    
    int m_dataBufferSize;
    int m_connectionTimeout;
    bool m_autoReconnect;
    
    int m_totalDataPoints;
    double m_averageSpeed;
    double m_maxRecordedSpeed;
    double m_minRecordedSpeed;
    QDateTime m_sessionStartTime;
};

#endif 