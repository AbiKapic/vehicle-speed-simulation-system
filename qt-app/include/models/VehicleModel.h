#ifndef VEHICLEMODEL_H
#define VEHICLEMODEL_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QMutex>

struct VehicleData {
    QString id;
    QString type;
    QString manufacturer;
    QString model;
    int year;
    double maxSpeed;
    double engineCapacity;
    QString fuelType;
    QString transmission;
    QDateTime registrationDate;
    QString licensePlate;
    QString vin;
    QString color;
    double weight;
    double length;
    double width;
    double height;
    int passengerCapacity;
    QString status;
    QDateTime lastMaintenance;
    double totalMileage;
    double currentMileage;
    QString notes;
};

struct VehicleStatus {
    QString status;
    QString location;
    QDateTime timestamp;
    double currentSpeed;
    double fuelLevel;
    double batteryLevel;
    double engineTemperature;
    double oilPressure;
    bool isRunning;
    bool isConnected;
    QString errorCode;
    QString errorMessage;
};

class VehicleModel : public QObject
{
    Q_OBJECT

public:
    explicit VehicleModel(QObject* parent = nullptr);
    ~VehicleModel();

    
    void setVehicleData(const VehicleData& data);
    VehicleData getVehicleData() const;
    void updateVehicleData(const QString& field, const QVariant& value);
    
    
    void setVehicleStatus(const VehicleStatus& status);
    VehicleStatus getVehicleStatus() const;
    void updateStatus(const QString& status);
    void updateLocation(const QString& location);
    void updateSpeed(double speed);
    void updateFuelLevel(double level);
    void updateBatteryLevel(double level);
    void updateEngineTemperature(double temperature);
    void updateOilPressure(double pressure);
    void setRunningState(bool running);
    void setConnectionState(bool connected);
    void setError(const QString& code, const QString& message);
    void clearError();
    
    
    QString getVehicleId() const;
    QString getVehicleType() const;
    QString getManufacturer() const;
    QString getModel() const;
    int getYear() const;
    double getMaxSpeed() const;
    QString getLicensePlate() const;
    QString getVin() const;
    QString getStatus() const;
    
   
    bool isRunning() const;
    bool isConnected() const;
    bool hasError() const;
    QString getErrorCode() const;
    QString getErrorMessage() const;
    double getCurrentSpeed() const;
    double getFuelLevel() const;
    double getBatteryLevel() const;
    double getEngineTemperature() const;
    double getOilPressure() const;
    
  
    bool isValid() const;
    QString validateVehicleData() const;
    bool isSpeedValid(double speed) const;
    bool isFuelLevelValid(double level) const;
    bool isBatteryLevelValid(double level) const;
    bool isTemperatureValid(double temperature) const;
    bool isOilPressureValid(double pressure) const;
    
   
    bool saveToFile(const QString& filePath);
    bool loadFromFile(const QString& filePath);
    QString toJson() const;
    bool fromJson(const QString& json);
    
    
    void addStatusHistory(const VehicleStatus& status);
    QVector<VehicleStatus> getStatusHistory() const;
    QVector<VehicleStatus> getStatusHistory(const QDateTime& start, const QDateTime& end) const;
    void clearStatusHistory();
    int getStatusHistoryCount() const;

signals:
    void vehicleDataChanged(const VehicleData& data);
    void vehicleStatusChanged(const VehicleStatus& status);
    void statusUpdated(const QString& status);
    void speedChanged(double speed);
    void fuelLevelChanged(double level);
    void batteryLevelChanged(double level);
    void engineTemperatureChanged(double temperature);
    void oilPressureChanged(double pressure);
    void runningStateChanged(bool running);
    void connectionStateChanged(bool connected);
    void errorOccurred(const QString& code, const QString& message);
    void errorCleared();
    void dataSaved(const QString& filePath);
    void dataLoaded(const QString& filePath);

private:
    void emitDataChanged();
    void emitStatusChanged();
    bool validateField(const QString& field, const QVariant& value) const;
    
    VehicleData m_vehicleData;
    VehicleStatus m_currentStatus;
    QVector<VehicleStatus> m_statusHistory;
    QMutex m_dataMutex;
    QMutex m_statusMutex;
    
   
    static const double MIN_SPEED;
    static const double MAX_SPEED;
    static const double MIN_FUEL_LEVEL;
    static const double MAX_FUEL_LEVEL;
    static const double MIN_BATTERY_LEVEL;
    static const double MAX_BATTERY_LEVEL;
    static const double MIN_TEMPERATURE;
    static const double MAX_TEMPERATURE;
    static const double MIN_OIL_PRESSURE;
    static const double MAX_OIL_PRESSURE;
};

#endif 