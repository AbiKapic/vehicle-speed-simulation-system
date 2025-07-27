#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>

class Logger : public QObject
{
    Q_OBJECT

public:
    enum LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3,
        FATAL = 4
    };

    static void initialize(bool debugMode = false);
    static void shutdown();
    
    static void debug(const QString& message);
    static void info(const QString& message);
    static void warning(const QString& message);
    static void error(const QString& message);
    static void fatal(const QString& message);
    
    static void log(LogLevel level, const QString& message);
    
    static void setLogLevel(LogLevel level);
    static void setLogFilePath(const QString& path);
    static void setEnableConsoleOutput(bool enable);
    static void setEnableFileOutput(bool enable);
    
    static QString getLogLevelString(LogLevel level);
    static LogLevel getLogLevelFromString(const QString& levelString);
    static QString getCurrentTimestamp();

signals:
    void logMessage(LogLevel level, const QString& message, const QString& timestamp);

private:
    static Logger* getInstance();
    Logger();
    ~Logger();
    
    void writeLog(LogLevel level, const QString& message);
    void writeToFile(const QString& logEntry);
    void writeToConsole(const QString& logEntry);
    QString formatLogMessage(LogLevel level, const QString& message);
    
    static Logger* s_instance;
    static QMutex s_mutex;
    
    QFile* m_logFile;
    QTextStream* m_textStream;
    LogLevel m_currentLevel;
    bool m_enableConsoleOutput;
    bool m_enableFileOutput;
    QString m_logFilePath;
    QMutex m_writeMutex;
};


#define LOG_DEBUG(msg) Logger::debug(msg)
#define LOG_INFO(msg) Logger::info(msg)
#define LOG_WARNING(msg) Logger::warning(msg)
#define LOG_ERROR(msg) Logger::error(msg)
#define LOG_FATAL(msg) Logger::fatal(msg)

#endif  