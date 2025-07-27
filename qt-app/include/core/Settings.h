#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QString>

class Settings : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject* parent = nullptr);
    ~Settings();

   
    QString getApplicationName() const;
    QString getApplicationVersion() const;
    QString getOrganizationName() const;
    
    double getMaxSpeed() const;
    void setMaxSpeed(double speed);
    double getMinSpeed() const;
    void setMinSpeed(double speed);
    QString getSpeedUnit() const;
    void setSpeedUnit(const QString& unit);
    
    bool getShowSpeedometer() const;
    void setShowSpeedometer(bool show);
    bool getShowSpeedGraph() const;
    void setShowSpeedGraph(bool show);
    QString getTheme() const;
    void setTheme(const QString& theme);
    
    QString getSerialPort() const;
    void setSerialPort(const QString& port);
    int getBaudRate() const;
    void setBaudRate(int baudRate);
    int getDataTimeout() const;
    void setDataTimeout(int timeout);
    
    bool getEnableLogging() const;
    void setEnableLogging(bool enable);
    QString getLogLevel() const;
    void setLogLevel(const QString& level);
    QString getLogFilePath() const;
    void setLogFilePath(const QString& path);
    
    int getDataBufferSize() const;
    void setDataBufferSize(int size);
    bool getAutoSaveData() const;
    void setAutoSaveData(bool autoSave);
    QString getDataFilePath() const;
    void setDataFilePath(const QString& path);
    
    void loadDefaults();
    void reset();
    bool save();
    bool load();
    
    QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant()) const;
    void setValue(const QString& key, const QVariant& value);

signals:
    void settingChanged(const QString& key, const QVariant& value);
    void settingsLoaded();
    void settingsSaved();

private:
    void initializeDefaults();
    QString getDefaultValue(const QString& key) const;
    
    QSettings* m_settings;
    
    static const double DEFAULT_MAX_SPEED;
    static const double DEFAULT_MIN_SPEED;
    static const QString DEFAULT_SPEED_UNIT;
    static const bool DEFAULT_SHOW_SPEEDOMETER;
    static const bool DEFAULT_SHOW_SPEED_GRAPH;
    static const QString DEFAULT_THEME;
    static const QString DEFAULT_SERIAL_PORT;
    static const int DEFAULT_BAUD_RATE;
    static const int DEFAULT_DATA_TIMEOUT;
    static const bool DEFAULT_ENABLE_LOGGING;
    static const QString DEFAULT_LOG_LEVEL;
    static const int DEFAULT_DATA_BUFFER_SIZE;
    static const bool DEFAULT_AUTO_SAVE_DATA;
};

#endif 