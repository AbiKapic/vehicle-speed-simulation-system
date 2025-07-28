#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include "../models/VehicleModel.h"

class GameEngine : public QObject
{
    Q_OBJECT

public:
    explicit GameEngine(QObject *parent = nullptr);
    ~GameEngine();

    bool isRunning() const { return m_isRunning; }
    void startGame();
    void stopGame();
    void pauseGame();
    void resumeGame();
    
    VehicleModel* vehicle() const { return m_vehicle; }
   
    
    void update(double deltaTime);
    void render();
    
    void updatePhysics(double deltaTime);
    void checkCollisions();
    
    void setSpeedMultiplier(double multiplier);
    double speedMultiplier() const { return m_speedMultiplier; }
    
    void setGameSpeed(double speed);
    double gameSpeed() const { return m_gameSpeed; }

signals:
    void gameStarted();
    void gameStopped();
    void gamePaused();
    void gameResumed();
    void collisionDetected();
    void vehicleOffRoad();
    void speedChanged(double speed);

private slots:
    void gameLoop();
    void onVehiclePositionChanged(const QPointF &position);
    void onVehicleSpeedChanged(double speed);

private:
    VehicleModel *m_vehicle;
    
    
    bool m_isRunning;
    bool m_isPaused;
    
    QTimer *m_gameTimer;
    QElapsedTimer *m_elapsedTimer;
    double m_lastUpdateTime;
    
    double m_gameSpeed;
    double m_speedMultiplier;
    int m_targetFPS;
    
    double m_gravity;
    double m_friction;
    
    void initializeGame();
    void setupConnections();
    void updateGameObjects(double deltaTime);
    void handleVehicleOffRoad();
};

#endif 