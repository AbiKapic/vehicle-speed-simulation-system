#include "core/GameEngine.h"

GameEngine::GameEngine(QObject *parent)
    : QObject(parent)
    , m_vehicle(nullptr)
    , m_isRunning(false)
    , m_isPaused(false)
    , m_gameTimer(nullptr)
    , m_elapsedTimer(nullptr)
    , m_lastUpdateTime(0.0)
    , m_gameSpeed(1.0)
    , m_speedMultiplier(1.0)
    , m_targetFPS(60)
    , m_gravity(0.0)
    , m_friction(0.98)
{
    
    m_gameTimer = new QTimer(this);
    m_elapsedTimer = new QElapsedTimer();
    
   
    m_gameTimer->setInterval(1000 / m_targetFPS);
    connect(m_gameTimer, &QTimer::timeout, this, &GameEngine::gameLoop);
    
   
    m_vehicle = new VehicleModel(this);
  
    
   
    setupConnections();
    
}

GameEngine::~GameEngine()
{
    stopGame();
}

void GameEngine::startGame()
{
    if (!m_isRunning) {
        m_isRunning = true;
        m_isPaused = false;
        m_elapsedTimer->start();
        m_lastUpdateTime = 0.0;
        
       
        if (m_vehicle) {
            m_vehicle->start();
        }
        
       
        m_gameTimer->start();
        
        emit gameStarted();
    }
}

void GameEngine::stopGame()
{
    if (m_isRunning) {
        m_isRunning = false;
        m_isPaused = false;
        m_gameTimer->stop();
        
       
        if (m_vehicle) {
            m_vehicle->stop();
        }
        
        emit gameStopped();
    }
}

void GameEngine::pauseGame()
{
    if (m_isRunning && !m_isPaused) {
        m_isPaused = true;
        m_gameTimer->stop();
        emit gamePaused();
      
    }
}

void GameEngine::resumeGame()
{
    if (m_isRunning && m_isPaused) {
        m_isPaused = false;
        m_gameTimer->start();
        emit gameResumed();
       
    }
}

void GameEngine::update(double deltaTime)
{
    if (!m_isRunning || m_isPaused) {
        return;
    }
    
   
    deltaTime *= m_speedMultiplier;
    
    updateGameObjects(deltaTime);
    updatePhysics(deltaTime);
    checkCollisions();
}

void GameEngine::render()
{
    // Rendering is handled by the GameView
    // This method can be used for any engine-level rendering if needed
}

void GameEngine::updatePhysics(double deltaTime)
{
    if (!m_vehicle) {
        return;
    }
    
   
    m_vehicle->updatePosition(deltaTime);
    
    
    if (m_vehicle->speed() > 0) {
        double newSpeed = m_vehicle->speed() * m_friction;
        m_vehicle->setSpeed(newSpeed);
    }
}

void GameEngine::checkCollisions()
{
   
    return;
    
    if (!m_vehicle) {
        return;
    }
    QPointF vehiclePos = m_vehicle->position();
    QSizeF vehicleSize = m_vehicle->size();
    QRectF roadArea(0, 250, 2000, 100); // Wider road area for collision
    QRectF vehicleBounds(vehiclePos.x() - vehicleSize.width() / 2,
                        vehiclePos.y() - vehicleSize.height() / 2,
                        vehicleSize.width(), vehicleSize.height());
    if (!roadArea.contains(vehicleBounds)) {
        handleVehicleOffRoad();
    }
}

void GameEngine::setSpeedMultiplier(double multiplier)
{
    m_speedMultiplier = qBound(0.1, multiplier, 5.0);
}

void GameEngine::setGameSpeed(double speed)
{
    m_gameSpeed = qBound(0.1, speed, 10.0);
}

void GameEngine::gameLoop()
{
    if (!m_isRunning || m_isPaused) {
        return;
    }
    
   
    qint64 currentTime = m_elapsedTimer->elapsed();
    double deltaTime = (currentTime - m_lastUpdateTime) / 1000.0; 
    m_lastUpdateTime = currentTime;
    
    
    deltaTime = qMin(deltaTime, 0.1);
    
    
    update(deltaTime);
}

void GameEngine::onVehiclePositionChanged(const QPointF &position)
{
    
    if ( position.x() > 400) { 
        double scrollOffset = position.x() - 400;
       
        
        
        m_vehicle->setPosition(QPointF(400, position.y()));
    }
}

void GameEngine::onVehicleSpeedChanged(double speed)
{
    emit speedChanged(speed);
}

void GameEngine::initializeGame()
{
   
    if (m_vehicle) {
        m_vehicle->loadSprites();
      
    }
    
    

    
    
    if (m_vehicle) {
        m_vehicle->setPosition(QPointF(100, 700)); 
        m_vehicle->setSpeed(0.0);
    }
    
 
}

void GameEngine::setupConnections()
{
    if (m_vehicle) {
        connect(m_vehicle, &VehicleModel::positionChanged,
                this, &GameEngine::onVehiclePositionChanged);
        connect(m_vehicle, &VehicleModel::speedChanged,
                this, &GameEngine::onVehicleSpeedChanged);
    }
    
   
}

void GameEngine::updateGameObjects(double deltaTime)
{
    // Update any additional game objects here
    // For now, vehicle updates are handled in updatePhysics
}

void GameEngine::handleVehicleOffRoad()
{
    emit vehicleOffRoad();
    
    if (m_vehicle) {
        m_vehicle->setSpeed(m_vehicle->speed() * 0.5);
    }
    
    
} 