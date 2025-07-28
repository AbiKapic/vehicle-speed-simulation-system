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

void MainWindow::setupGameEngine()
{
   
    
    m_gameEngine = new GameEngine(this);
    
   
    if (m_gameView) {
        m_gameView->setGameEngine(m_gameEngine);
    } 
    
    if (m_controlPanel) {
        m_controlPanel->setGameEngine(m_gameEngine);
    } 
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