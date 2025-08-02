#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QVector>
#include <QPointF>
#include "../core/GameEngine.h"

class GameView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GameView(QWidget *parent = nullptr);
    ~GameView();

    void setGameEngine(GameEngine *engine);
    void startCarAnimation();
    void stopCarAnimation();
    void setCarSpeed(double speed);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void onGameEngineChanged();
    void animateCar();
    void updateCarPosition();

private:
    void setupScene();
    void loadRoadBackground();
    void createCarSprite();
    void defineWaypoints();
    void updateCarRotation();
    void createWoodenBorder(int roadWidth, int roadHeight);

    GameEngine *m_gameEngine;
    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_roadBackground;
    QGraphicsPixmapItem *m_carSprite;
    QTimer *m_animationTimer;
    
  
    QVector<QPointF> m_waypoints;
    int m_currentWaypointIndex;
    double m_carSpeed;
    bool m_isAnimating;
    
  
    QPointF m_carCurrentPos;
    QPointF m_carTargetPos;
    double m_carProgress; 
};

#endif 