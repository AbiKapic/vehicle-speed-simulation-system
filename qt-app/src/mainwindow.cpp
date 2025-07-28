#include "views/MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QGuiApplication>
#include <QStatusBar>
#include <QStyle>
#include <QScreen>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_mainSplitter(nullptr)
    , m_gameView(nullptr)
    , m_controlPanel(nullptr)
    , m_gameEngine(nullptr)
    , m_startAction(nullptr)
    , m_stopAction(nullptr)
    , m_pauseAction(nullptr)
    , m_resetAction(nullptr)
{
    setWindowTitle("Vehicle Speed Simulation System");
    setMinimumSize(1200, 800);
    resize(1400, 900);
    
    createCentralWidget();
    setupGameEngine();
    createMenuBar();
    setupConnections();
    
    statusBar()->showMessage("Ready to start simulation");
    
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        QRect windowGeometry = geometry();
        int x = (screenGeometry.width() - windowGeometry.width()) / 2;
        int y = (screenGeometry.height() - windowGeometry.height()) / 2;
        move(screenGeometry.x() + x, screenGeometry.y() + y);
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
   
    QMenu *gameMenu = menuBar->addMenu("&Game");
    
    m_startAction = gameMenu->addAction("&Start Simulation");
    m_startAction->setShortcut(QKeySequence("Ctrl+S"));
    m_startAction->setIcon(QIcon(":/assets/icons/play.png"));
    connect(m_startAction, &QAction::triggered, this, [this]() {
        if (m_gameEngine) {
            m_gameEngine->startGame();
        }
    });
    
    m_stopAction = gameMenu->addAction("&Stop Simulation");
    m_stopAction->setShortcut(QKeySequence("Ctrl+X"));
    m_stopAction->setIcon(QIcon(":/assets/icons/stop.png"));
    m_stopAction->setEnabled(false);
    connect(m_stopAction, &QAction::triggered, this, [this]() {
        if (m_gameEngine) {
            m_gameEngine->stopGame();
        }
    });
    
    m_pauseAction = gameMenu->addAction("&Pause Simulation");
    m_pauseAction->setShortcut(QKeySequence("Ctrl+P"));
    m_pauseAction->setIcon(QIcon(":/assets/icons/pause.png"));
    m_pauseAction->setEnabled(false);
    connect(m_pauseAction, &QAction::triggered, this, [this]() {
        if (m_gameEngine) {
            m_gameEngine->pauseGame();
        }
    });
    
    gameMenu->addSeparator();
    
    m_resetAction = gameMenu->addAction("&Reset Simulation");
    m_resetAction->setShortcut(QKeySequence("Ctrl+R"));
    m_resetAction->setIcon(QIcon(":/assets/icons/reset.png"));
    connect(m_resetAction, &QAction::triggered, this, [this]() {
        if (m_gameEngine) {
            m_gameEngine->stopGame();
           
            if (m_gameEngine->vehicle()) {
                m_gameEngine->vehicle()->setPosition(QPointF(100, 300));
                m_gameEngine->vehicle()->setSpeed(0.0);
            }
            statusBar()->showMessage("Simulation reset");
        }
    });
    
   
    QMenu *viewMenu = menuBar->addMenu("&View");
    
    QAction *centerVehicleAction = viewMenu->addAction("&Center on Vehicle");
    centerVehicleAction->setShortcut(QKeySequence("Ctrl+C"));
    connect(centerVehicleAction, &QAction::triggered, this, [this]() {
        if (m_gameView && m_gameView->scene()) {
            
            QList<QGraphicsItem*> items = m_gameView->scene()->items();
            for (QGraphicsItem* item : items) {
                if (item->type() == QGraphicsPixmapItem::Type && item->zValue() == 10) {
                    m_gameView->centerOn(item);
                    break;
                }
            }
        }
    });
    
    QAction *fitRoadAction = viewMenu->addAction("&Fit Road in View");
    fitRoadAction->setShortcut(QKeySequence("Ctrl+F"));
    connect(fitRoadAction, &QAction::triggered, this, [this]() {
        if (m_gameView && m_gameView->scene()) {
            m_gameView->fitInView(m_gameView->scene()->sceneRect(), Qt::KeepAspectRatio);
        }
    });
    
    viewMenu->addSeparator();
    
    QAction *zoomInAction = viewMenu->addAction("Zoom &In");
    zoomInAction->setShortcut(QKeySequence("Ctrl++"));
    connect(zoomInAction, &QAction::triggered, this, [this]() {
        if (m_gameView) {
            m_gameView->scale(1.2, 1.2);
        }
    });
    
    QAction *zoomOutAction = viewMenu->addAction("Zoom &Out");
    zoomOutAction->setShortcut(QKeySequence("Ctrl+-"));
    connect(zoomOutAction, &QAction::triggered, this, [this]() {
        if (m_gameView) {
            m_gameView->scale(0.8, 0.8);
        }
    });
    
    QMenu *helpMenu = menuBar->addMenu("&Help");
    
    QAction *aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    
    QAction *lgplAction = helpMenu->addAction("&LGPL License Information");
    connect(lgplAction, &QAction::triggered, this, &MainWindow::showLGPLInfo);
}

void MainWindow::createCentralWidget()
{
    m_centralWidget = new QWidget();
    setCentralWidget(m_centralWidget);
    
    m_mainSplitter = new QSplitter(Qt::Horizontal, m_centralWidget);
    
    m_gameView = new GameView();
    m_gameView->setMinimumSize(800, 600);
    
    m_controlPanel = new ControlPanel();
    m_controlPanel->setMinimumWidth(300);
    m_controlPanel->setMaximumWidth(400);
    
    m_mainSplitter->addWidget(m_gameView);
    m_mainSplitter->addWidget(m_controlPanel);
    
    m_mainSplitter->setSizes(QList<int>() << 1000 << 300);
    
    QHBoxLayout *layout = new QHBoxLayout(m_centralWidget);
    layout->addWidget(m_mainSplitter);
    layout->setContentsMargins(0, 0, 0, 0);
}

void MainWindow::setupGameEngine()
{
    m_gameEngine = new GameEngine(this);
    if (m_gameView) {
        m_gameView->setGameEngine(m_gameEngine);
    }
    if (m_controlPanel) {
        m_controlPanel->setGameEngine(m_gameEngine);
        m_controlPanel->setGameView(m_gameView);
    }
}

void MainWindow::setupConnections()
{
    if (m_gameEngine) {
        
        connect(m_gameEngine, &GameEngine::gameStarted, this, &MainWindow::onGameStarted);
        connect(m_gameEngine, &GameEngine::gameStopped, this, &MainWindow::onGameStopped);
        connect(m_gameEngine, &GameEngine::vehicleOffRoad, this, &MainWindow::onVehicleOffRoad);
        
       
        connect(m_gameEngine, &GameEngine::speedChanged, this, [this](double speed) {
            QString message = QString("Current Speed: %1 km/h").arg(qRound(speed));
            statusBar()->showMessage(message);
        });
    }
}

void MainWindow::onGameStarted()
{
   
    if (m_startAction) m_startAction->setEnabled(false);
    if (m_stopAction) m_stopAction->setEnabled(true);
    if (m_pauseAction) m_pauseAction->setEnabled(true);
    
    statusBar()->showMessage("Simulation started");
}

void MainWindow::onGameStopped()
{
   
    if (m_startAction) m_startAction->setEnabled(true);
    if (m_stopAction) m_stopAction->setEnabled(false);
    if (m_pauseAction) m_pauseAction->setEnabled(false);
    
    statusBar()->showMessage("Simulation stopped");
}

void MainWindow::onVehicleOffRoad()
{
    statusBar()->showMessage("Warning: Vehicle off road!", 3000);
}

void MainWindow::showAboutDialog()
{
    QMessageBox::about(this, "About Vehicle Speed Simulation System",
        "<h3>Vehicle Speed Simulation System v1.0.0</h3>"
        "<p>A professional Qt-based application for vehicle speed monitoring and simulation.</p>"
        "<p><b>Features:</b></p>"
        "<ul>"
        "<li>2D vehicle simulation with physics</li>"
        "<li>Dynamic road system with scrolling</li>"
        "<li>Real-time speed control and monitoring</li>"
        "<li>Collision detection and road boundaries</li>"
        "<li>Professional UI with control panel</li>"
        "</ul>"
        "<p><b>License:</b> MIT License</p>"
        "<p><b>Qt Version:</b> 6.9.1</p>"
        "<p><b>Qt License:</b> LGPL v3</p>"
        "<p>This application uses Qt libraries under the GNU Lesser General Public License v. 3 (LGPL v3).</p>"
        "<p>For complete license information, see Help → LGPL License Information</p>");
}

void MainWindow::showLGPLInfo()
{
    QString lgplText;
    
    // Try to load from resource file
    QFile file(":/QT_LGPL_NOTICE.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        lgplText = in.readAll();
        file.close();
    } else {
        lgplText = "Qt LGPL v3 License Notice\n"
                  "========================\n\n"
                  "This application uses Qt libraries under the GNU Lesser General Public License v. 3 (LGPL v3).\n\n"
                  "USER RIGHTS UNDER LGPL v3:\n"
                  "==========================\n\n"
                  "You have the following rights regarding this application and the Qt libraries it uses:\n\n"
                  "1. FREEDOM TO RUN: You may run this program for any purpose.\n"
                  "2. FREEDOM TO STUDY: You may study how the program works and adapt it to your needs.\n"
                  "3. FREEDOM TO REDISTRIBUTE: You may redistribute copies of this program.\n"
                  "4. FREEDOM TO IMPROVE: You may improve the program and release your improvements to the public.\n\n"
                  "For complete Qt LGPL compliance information, visit:\n"
                  "https://www.qt.io/licensing/open-source-lgpl-obligations\n\n"
                  "For the complete LGPL v3 license text, visit:\n"
                  "https://www.gnu.org/licenses/lgpl-3.0.html";
    }
    
    QMessageBox::information(this, "Qt LGPL v3 License Information", lgplText);
}
