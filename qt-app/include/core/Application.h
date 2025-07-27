#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QSettings>
#include <QTimer>
#include <memory>

class VehicleController;
class SpeedController;
class Settings;

class Application : public QObject
{
    Q_OBJECT

public:
    static Application& getInstance();
    
    
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    
    
    bool initialize();
    void shutdown();
    bool loadConfiguration(const QString& configPath);
    bool saveConfiguration(const QString& configPath = QString());
    
 
    VehicleController* getVehicleController() const;
    SpeedController* getSpeedController() const;
    Settings* getSettings() const;
    
   
    bool isInitialized() const { return m_initialized; }
    QString getVersion() const { return "1.0.0"; }
    
signals:
    void applicationInitialized();
    void applicationShutdown();
    void configurationLoaded();
    void configurationSaved();
    void errorOccurred(const QString& error);

private slots:
    void onHeartbeat();
    void onError(const QString& error);

private:
    Application();
    ~Application();
    
    bool initializeControllers();
    bool initializeSettings();
    void setupConnections();
    
    bool m_initialized;
    std::unique_ptr<VehicleController> m_vehicleController;
    std::unique_ptr<SpeedController> m_speedController;
    std::unique_ptr<Settings> m_settings;
    std::unique_ptr<QSettings> m_config;
    QTimer m_heartbeatTimer;
};

#endif  