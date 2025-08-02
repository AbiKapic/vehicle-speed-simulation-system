#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QProgressBar>
#include <QTimer>
#include "../core/GameEngine.h"

class GameView;

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

   
    void setGameEngine(GameEngine *engine);
    void setGameView(GameView *view);
    GameEngine* gameEngine() const { return m_gameEngine; }
    
    
    void setGameRunning(bool running);
    bool isGameRunning() const { return m_isGameRunning; }
    
    
    void setSpeed(double speed);
    double currentSpeed() const { return m_currentSpeed; }
    
    
    void updateSpeedDisplay();
    void updateControlStates();

private slots:
    void onStartButtonClicked();
    void onStopButtonClicked();
    void onPauseButtonClicked();
    void onSpeedSliderChanged(int value);
    void onAccelerateButtonClicked();
    void onDecelerateButtonClicked();
    void onGameEngineSpeedChanged(double speed);
    void onGameStateChanged();
    void updateSpeedTimer();

public slots:
    void onSpeedReportingStatusChanged(bool reporting);
    void onSpeedReported(const QString &endpoint, const QString &message);
    void onSpeedReportingErrorOccurred(const QString &error);

private:
   
    GameEngine *m_gameEngine;
    GameView *m_gameView;
    
    
    bool m_isGameRunning;
    bool m_isPaused;
    double m_currentSpeed;
    
    
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    QPushButton *m_pauseButton;
    QPushButton *m_accelerateButton;
    QPushButton *m_decelerateButton;
    
    QSlider *m_speedSlider;
    QLabel *m_speedLabel;
    QProgressBar *m_speedProgressBar;
    
    QLabel *m_statusLabel;
    QLabel *m_gameTimeLabel;
    QLabel *m_speedReportingStatusLabel;
    QLabel *m_speedReportingMessageLabel;
    
   
    QVBoxLayout *m_mainLayout;
    QGroupBox *m_gameControlGroup;
    QGroupBox *m_speedControlGroup;
    QGroupBox *m_statusGroup;
    QGroupBox *m_speedReportingGroup;
    
    
    QTimer *m_updateTimer;
    
    
    void createUI();
    void setupConnections();
    void createGameControls();
    void createSpeedControls();
    void createStatusDisplay();
    void createSpeedReportingDisplay();
    
    
    void updateButtonStates();
    void updateSpeedDisplayText();
    void updateStatusText();
    QString formatSpeed(double speed) const;
    QString formatTime(int seconds) const;
};

#endif 