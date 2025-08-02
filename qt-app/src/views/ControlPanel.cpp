#include "views/ControlPanel.h"
#include "views/GameView.h"
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QProgressBar>
#include <QTimer>

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
    , m_gameEngine(nullptr)
    , m_gameView(nullptr)
    , m_isGameRunning(false)
    , m_isPaused(false)
    , m_currentSpeed(0.0)
{
    createUI();
    setupConnections();
    
    
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(100); 
    connect(m_updateTimer, &QTimer::timeout, this, &ControlPanel::updateSpeedTimer);
    m_updateTimer->start();
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::setGameEngine(GameEngine *engine)
{
    m_gameEngine = engine;
}

void ControlPanel::setGameView(GameView *view)
{
    m_gameView = view;
}

void ControlPanel::setGameRunning(bool running)
{
    m_isGameRunning = running;
    updateControlStates();
}

void ControlPanel::setSpeed(double speed)
{
    m_currentSpeed = speed;
    updateSpeedDisplay();
}

void ControlPanel::updateSpeedDisplay()
{
    updateSpeedDisplayText();
    
    if (m_speedProgressBar) {
        double maxSpeed = m_gameEngine && m_gameEngine->vehicle() ? 
                         m_gameEngine->vehicle()->maxSpeed() : 200.0;
        int percentage = qRound((m_currentSpeed / maxSpeed) * 100);
        m_speedProgressBar->setValue(percentage);
    }
}

void ControlPanel::updateControlStates()
{
    updateButtonStates();
    updateStatusText();
}

void ControlPanel::onStartButtonClicked()
{
    if (m_gameEngine) {
        m_gameEngine->startGame();
        
        
        if (m_gameView) {
            m_gameView->startCarAnimation();
        }
        
    }
}

void ControlPanel::onStopButtonClicked()
{
    if (m_gameEngine) {
        m_gameEngine->stopGame();
        
        
        if (m_gameView) {
            m_gameView->stopCarAnimation();
        }
        
    }
}

void ControlPanel::onPauseButtonClicked()
{
    if (m_gameEngine && m_isGameRunning) {
        m_gameEngine->pauseGame();
    }
}

void ControlPanel::onSpeedSliderChanged(int value)
{
    double speed = value / 100.0; 
    
    if (m_gameEngine && m_gameEngine->vehicle()) {
        m_gameEngine->vehicle()->setSpeed(speed * 100.0); 
    }
    
    
    if (m_gameView) {
        m_gameView->setCarSpeed(speed * 5.0); 
    }
    
    
    m_speedLabel->setText(QString("Speed: %1 km/h").arg(qRound(speed * 100)));
    
}

void ControlPanel::onAccelerateButtonClicked()
{
    if (m_gameEngine && m_gameEngine->vehicle()) {
        m_gameEngine->vehicle()->accelerate();
    }
}

void ControlPanel::onDecelerateButtonClicked()
{
    if (m_gameEngine && m_gameEngine->vehicle()) {
        m_gameEngine->vehicle()->decelerate();
    }
}

void ControlPanel::onGameEngineSpeedChanged(double speed)
{
    setSpeed(speed);
}

void ControlPanel::onGameStateChanged()
{
    updateControlStates();
}

void ControlPanel::updateSpeedTimer()
{
    
    if (m_gameTimeLabel && m_gameEngine && m_gameEngine->isRunning()) {
       
        m_gameTimeLabel->setText("Game Running");
    }
}

void ControlPanel::createUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(10);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    
    createGameControls();
    createSpeedControls();
    createStatusDisplay();
    createSpeedReportingDisplay();
}

void ControlPanel::setupConnections()
{
    
    if (m_startButton) {
        connect(m_startButton, &QPushButton::clicked, this, &ControlPanel::onStartButtonClicked);
    }
    
    if (m_stopButton) {
        connect(m_stopButton, &QPushButton::clicked, this, &ControlPanel::onStopButtonClicked);
    }
    
    if (m_pauseButton) {
        connect(m_pauseButton, &QPushButton::clicked, this, &ControlPanel::onPauseButtonClicked);
    }
    
    if (m_accelerateButton) {
        connect(m_accelerateButton, &QPushButton::clicked, this, &ControlPanel::onAccelerateButtonClicked);
    }
    
    if (m_decelerateButton) {
        connect(m_decelerateButton, &QPushButton::clicked, this, &ControlPanel::onDecelerateButtonClicked);
    }
    
    if (m_speedSlider) {
        connect(m_speedSlider, &QSlider::valueChanged, this, &ControlPanel::onSpeedSliderChanged);
    }
}

void ControlPanel::createGameControls()
{
    m_gameControlGroup = new QGroupBox("Game Controls", this);
    QHBoxLayout *gameLayout = new QHBoxLayout(m_gameControlGroup);
    
    
    m_startButton = new QPushButton("Start", this);
    m_startButton->setMinimumHeight(40);
    m_startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    
   
    m_stopButton = new QPushButton("Stop", this);
    m_stopButton->setMinimumHeight(40);
    m_stopButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; font-weight: bold; }");
    
   
    m_pauseButton = new QPushButton("Pause", this);
    m_pauseButton->setMinimumHeight(40);
    m_pauseButton->setStyleSheet("QPushButton { background-color: #ff9800; color: white; font-weight: bold; }");
    
    gameLayout->addWidget(m_startButton);
    gameLayout->addWidget(m_pauseButton);
    gameLayout->addWidget(m_stopButton);
    
    m_mainLayout->addWidget(m_gameControlGroup);
}

void ControlPanel::createSpeedControls()
{
    m_speedControlGroup = new QGroupBox("Speed Controls", this);
    QVBoxLayout *speedLayout = new QVBoxLayout(m_speedControlGroup);
    
    QHBoxLayout *speedDisplayLayout = new QHBoxLayout();
    m_speedLabel = new QLabel("Speed: 0 km/h", this);
    m_speedLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #2196F3; }");
    m_speedLabel->setAlignment(Qt::AlignCenter);
    
    speedDisplayLayout->addWidget(m_speedLabel);
    speedLayout->addLayout(speedDisplayLayout);
    
    m_speedProgressBar = new QProgressBar(this);
    m_speedProgressBar->setRange(0, 100);
    m_speedProgressBar->setValue(0);
    m_speedProgressBar->setTextVisible(true);
    m_speedProgressBar->setFormat("%p%");
    m_speedProgressBar->setStyleSheet(
        "QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"
        "QProgressBar::chunk { background-color: #2196F3; border-radius: 3px; }"
    );
    speedLayout->addWidget(m_speedProgressBar);
    
    m_speedSlider = new QSlider(Qt::Horizontal, this);
    m_speedSlider->setRange(0, 100);
    m_speedSlider->setValue(0);
    m_speedSlider->setTickPosition(QSlider::TicksBelow);
    m_speedSlider->setTickInterval(10);
    speedLayout->addWidget(m_speedSlider);
    
    QHBoxLayout *speedButtonLayout = new QHBoxLayout();
    m_accelerateButton = new QPushButton("Accelerate", this);
    m_accelerateButton->setMinimumHeight(30);
    m_accelerateButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; }");
    
    m_decelerateButton = new QPushButton("Decelerate", this);
    m_decelerateButton->setMinimumHeight(30);
    m_decelerateButton->setStyleSheet("QPushButton { background-color: #ff9800; color: white; }");
    
    speedButtonLayout->addWidget(m_accelerateButton);
    speedButtonLayout->addWidget(m_decelerateButton);
    speedLayout->addLayout(speedButtonLayout);
    
    m_mainLayout->addWidget(m_speedControlGroup);
}

void ControlPanel::createStatusDisplay()
{
    m_statusGroup = new QGroupBox("Status", this);
    QVBoxLayout *statusLayout = new QVBoxLayout(m_statusGroup);
    
    m_statusLabel = new QLabel("Ready", this);
    m_statusLabel->setStyleSheet("QLabel { font-size: 14px; color: #666; }");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    
    m_gameTimeLabel = new QLabel("", this);
    m_gameTimeLabel->setStyleSheet("QLabel { font-size: 12px; color: #999; }");
    m_gameTimeLabel->setAlignment(Qt::AlignCenter);
    
    statusLayout->addWidget(m_statusLabel);
    statusLayout->addWidget(m_gameTimeLabel);
    
    m_mainLayout->addWidget(m_statusGroup);
    
    m_mainLayout->addStretch();
}

void ControlPanel::createSpeedReportingDisplay()
{
    m_speedReportingGroup = new QGroupBox("Speed Reporting", this);
    QVBoxLayout *reportingLayout = new QVBoxLayout(m_speedReportingGroup);
    
    m_speedReportingStatusLabel = new QLabel("Speed Reporting: Disabled", this);
    m_speedReportingStatusLabel->setStyleSheet("QLabel { font-size: 14px; color: #f44336; font-weight: bold; }");
    m_speedReportingStatusLabel->setAlignment(Qt::AlignCenter);
    
    m_speedReportingMessageLabel = new QLabel("No speed data sent", this);
    m_speedReportingMessageLabel->setStyleSheet("QLabel { font-size: 12px; color: #666; }");
    m_speedReportingMessageLabel->setAlignment(Qt::AlignCenter);
    m_speedReportingMessageLabel->setWordWrap(true);
    
    reportingLayout->addWidget(m_speedReportingStatusLabel);
    reportingLayout->addWidget(m_speedReportingMessageLabel);
    
    m_mainLayout->addWidget(m_speedReportingGroup);
}

void ControlPanel::updateButtonStates()
{
    if (m_startButton) {
        if (m_isPaused) {
            m_startButton->setText("Resume");
            m_startButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; }");
        } else if (m_isGameRunning) {
            m_startButton->setText("Start");
            m_startButton->setEnabled(false);
            m_startButton->setStyleSheet("QPushButton { background-color: #ccc; color: #666; font-weight: bold; }");
        } else {
            m_startButton->setText("Start");
            m_startButton->setEnabled(true);
            m_startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
        }
    }
    
    if (m_stopButton) {
        m_stopButton->setEnabled(m_isGameRunning);
    }
    
    if (m_pauseButton) {
        m_pauseButton->setEnabled(m_isGameRunning && !m_isPaused);
    }
    
    if (m_accelerateButton && m_decelerateButton) {
        bool canControl = m_isGameRunning && !m_isPaused;
        m_accelerateButton->setEnabled(canControl);
        m_decelerateButton->setEnabled(canControl);
    }
    
    if (m_speedSlider) {
        m_speedSlider->setEnabled(m_isGameRunning && !m_isPaused);
    }
}

void ControlPanel::updateSpeedDisplayText()
{
    if (m_speedLabel) {
        QString speedText = QString("Speed: %1 km/h").arg(qRound(m_currentSpeed));
        m_speedLabel->setText(speedText);
    }
}

void ControlPanel::updateStatusText()
{
    if (m_statusLabel) {
        QString status;
        if (m_isGameRunning) {
            if (m_isPaused) {
                status = "Paused";
            } else {
                status = "Running";
            }
        } else {
            status = "Ready";
        }
        m_statusLabel->setText(status);
    }
}

QString ControlPanel::formatSpeed(double speed) const
{
    return QString("%1 km/h").arg(qRound(speed));
}

QString ControlPanel::formatTime(int seconds) const
{
    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(remainingSeconds, 2, 10, QChar('0'));
}

void ControlPanel::onSpeedReportingStatusChanged(bool reporting)
{
    if (m_speedReportingStatusLabel) {
        if (reporting) {
            m_speedReportingStatusLabel->setText("Speed Reporting: Active");
            m_speedReportingStatusLabel->setStyleSheet("QLabel { font-size: 14px; color: #4CAF50; font-weight: bold; }");
        } else {
            m_speedReportingStatusLabel->setText("Speed Reporting: Disabled");
            m_speedReportingStatusLabel->setStyleSheet("QLabel { font-size: 14px; color: #f44336; font-weight: bold; }");
        }
    }
}

void ControlPanel::onSpeedReported(const QString &endpoint, const QString &message)
{
    if (m_speedReportingMessageLabel) {
        QString displayText = QString("Last sent: %1 km/h").arg(m_currentSpeed);
        m_speedReportingMessageLabel->setText(displayText);
    }
}

void ControlPanel::onSpeedReportingErrorOccurred(const QString &error)
{
    if (m_speedReportingStatusLabel) {
        m_speedReportingStatusLabel->setText("Speed Reporting: Error");
        m_speedReportingStatusLabel->setStyleSheet("QLabel { font-size: 14px; color: #ff9800; font-weight: bold; }");
    }
    
    if (m_speedReportingMessageLabel) {
        m_speedReportingMessageLabel->setText(QString("Error: %1").arg(error));
    }
} 