#include "views/GameView.h"
#include <QResizeEvent>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QLineF>
#include <cmath>
#include <QMouseEvent>
#include <QGraphicsRectItem>

GameView::GameView(QWidget *parent)
    : QGraphicsView(parent)
    , m_gameEngine(nullptr)
    , m_scene(nullptr)
    , m_roadBackground(nullptr)
    , m_carSprite(nullptr)
    , m_animationTimer(nullptr)
    , m_currentWaypointIndex(0)
    , m_carSpeed(0.0)
    , m_isAnimating(false)
    , m_carProgress(0.0)
{
    setupScene();
    defineWaypoints();
    createCarSprite();
    
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(16);
    connect(m_animationTimer, &QTimer::timeout, this, &GameView::animateCar);
}

GameView::~GameView()
{
    if (m_animationTimer) {
        m_animationTimer->stop();
    }
}

void GameView::setupScene()
{
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 1300, 800);
    setScene(m_scene);
    
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    setBackgroundBrush(QBrush(QColor(240, 248, 255)));
    
    loadRoadBackground();
}

void GameView::loadRoadBackground()
{
    QPixmap roadImage(":/assets/road_textures/road.png");
    
    if (roadImage.isNull()) {
        roadImage.load("assets/road_textures/road.png");
    }
    
    if (!roadImage.isNull()) {
        QPixmap scaledRoad = roadImage.scaled(1200, 800, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        
        m_roadBackground = m_scene->addPixmap(scaledRoad);
        m_roadBackground->setZValue(-1);
        m_roadBackground->setPos(100, 0);
        
        createWoodenBorder();
    } else {
        QPixmap roadBackground(1200, 800);
        roadBackground.fill(QColor(50, 50, 50));
        
        QPainter painter(&roadBackground);
        painter.setRenderHint(QPainter::Antialiasing);
        
        painter.setBrush(QBrush(QColor(80, 80, 80)));
        painter.setPen(QPen(QColor(60, 60, 60), 2));
        painter.drawRect(0, 300, 1200, 200);
        
        painter.setPen(QPen(Qt::white, 3, Qt::DashLine));
        painter.drawLine(600, 300, 600, 500);
        
        painter.setPen(QPen(Qt::white, 2, Qt::SolidLine));
        painter.drawLine(0, 350, 1200, 350);
        painter.drawLine(0, 450, 1200, 450);
        
        m_roadBackground = m_scene->addPixmap(roadBackground);
        m_roadBackground->setZValue(-1);
        m_roadBackground->setPos(100, 0);
        
        createWoodenBorder();
    }
}

void GameView::createWoodenBorder()
{
    QPixmap borderPixmap(1220, 820);
    borderPixmap.fill(Qt::transparent);
    
    QPainter painter(&borderPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QLinearGradient woodGradient(0, 0, 0, 20);
    woodGradient.setColorAt(0, QColor(139, 69, 19));
    woodGradient.setColorAt(0.5, QColor(160, 82, 45));
    woodGradient.setColorAt(1, QColor(101, 67, 33));
    
    painter.setBrush(QBrush(woodGradient));
    painter.setPen(QPen(QColor(101, 67, 33), 2));
    
    painter.drawRect(0, 0, 1220, 20);
    painter.drawRect(0, 800, 1220, 20);
    painter.drawRect(0, 0, 20, 820);
    painter.drawRect(1200, 0, 20, 820);
    
    QGraphicsPixmapItem *borderItem = m_scene->addPixmap(borderPixmap);
    borderItem->setZValue(-2);
    borderItem->setPos(90, -10);
}

void GameView::createCarSprite()
{
    QPixmap carPixmap(":/assets/vehicle/vehicle.png");
    
    if (carPixmap.isNull()) {
        carPixmap.load("assets/vehicle/vehicle.png");
    }
    
    if (!carPixmap.isNull()) {
        QPixmap scaledCar = carPixmap.scaled(80, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_carSprite = m_scene->addPixmap(scaledCar);
    } else {
        QPixmap carPixmap(80, 60);
        carPixmap.fill(Qt::transparent);
        
        QPainter painter(&carPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        
        painter.setBrush(QBrush(QColor(255, 0, 0)));
        painter.setPen(QPen(QColor(200, 0, 0), 2));
        painter.drawRoundedRect(4, 4, 72, 52, 8, 8);
        
        painter.setBrush(QBrush(QColor(100, 150, 255)));
        painter.drawRect(12, 8, 16, 12);
        painter.drawRect(52, 8, 16, 12);
        
        painter.setBrush(QBrush(QColor(20, 20, 20)));
        painter.drawEllipse(8, 24, 12, 12);
        painter.drawEllipse(60, 24, 12, 12);
        
        m_carSprite = m_scene->addPixmap(carPixmap);
    }
    
    m_carSprite->setTransformOriginPoint(40, 30);
    m_carSprite->setZValue(10);
    
    QPointF startPos(200, 660);
    m_carSprite->setPos(startPos);
    m_carCurrentPos = startPos;
}

void GameView::defineWaypoints()
{
    m_waypoints.clear();
    
    QPointF startPos(200, 660);
    
    m_waypoints.append(startPos);
    
    m_waypoints.append(QPointF(221.743, 238.8));
    m_waypoints.append(QPointF(400.521, 231.49));
 
    m_waypoints.append(QPointF(372.315, 372.821));
   
    m_waypoints.append(QPointF(410.601, 488.313));
    m_waypoints.append(QPointF(565.323, 488.44));
    m_waypoints.append(QPointF(604.058, 405.464));
    m_waypoints.append(QPointF(604.311, 327.741));
  
    m_waypoints.append(QPointF(431.762, 260.662));

    m_waypoints.append(QPointF(407.854, 103.055));
    m_waypoints.append(QPointF(748.791, 119.4));
    m_waypoints.append(QPointF(746.101, 411.809));
  //
  
 //   m_waypoints.append(QPointF(723.711, 442.268));
    m_waypoints.append(QPointF(608.425, 401.05));
 //   m_waypoints.append(QPointF(589.691, 489.784));
    m_waypoints.append(QPointF(536.289, 522.611));
    m_waypoints.append(QPointF(538.725, 664.011));
 //   m_waypoints.append(QPointF(542.174, 672.54));
 //   m_waypoints.append(QPointF(211.996, 679.85));
    
    m_currentWaypointIndex = 0;
    m_carProgress = 0.0;
}

void GameView::setGameEngine(GameEngine *engine)
{
    if (m_gameEngine != engine) {
        m_gameEngine = engine;
        onGameEngineChanged();
    }
}

void GameView::startCarAnimation()
{
    
    if (!m_isAnimating) {
        m_isAnimating = true;
        m_animationTimer->start();
      
    } 
}

void GameView::stopCarAnimation()
{
   
    
    if (m_isAnimating) {
        m_isAnimating = false;
        m_animationTimer->stop();
      
    } 
}

void GameView::setCarSpeed(double speed)
{
    m_carSpeed = qBound(0.1, speed, 5.0); 
  
}

void GameView::animateCar()
{
   
    
    if (!m_isAnimating) {
       
        return;
    }
    
    if (m_waypoints.size() < 2) {
      
        return;
    }
    
    
    m_carProgress += 0.02 * m_carSpeed; 
    
 
    
    if (m_carProgress >= 1.0) {
        m_carProgress = 0.0;
        m_currentWaypointIndex++;
        
      
        if (m_currentWaypointIndex >= m_waypoints.size()) {
            m_currentWaypointIndex = 0;
        }
        
     
    }
    
    
    int nextIndex = (m_currentWaypointIndex + 1) % m_waypoints.size();
    QPointF current = m_waypoints[m_currentWaypointIndex];
    QPointF next = m_waypoints[nextIndex];
    
    
    QPointF newPos = current + (next - current) * m_carProgress;
    m_carSprite->setPos(newPos);
    
   
    
   
    updateCarRotation();
    
   
    centerOn(m_carSprite);
}

void GameView::updateCarRotation()
{
    if (m_waypoints.isEmpty()) return;
    
    int nextIndex = (m_currentWaypointIndex + 1) % m_waypoints.size();
    QPointF current = m_waypoints[m_currentWaypointIndex];
    QPointF next = m_waypoints[nextIndex];
    
    double deltaX = next.x() - current.x();
    double deltaY = next.y() - current.y();
    
    if (m_currentWaypointIndex == 0) {
        m_carSprite->setRotation(0);
    } else if (m_currentWaypointIndex == 1) {
        m_carSprite->setRotation(90);
    } 
    else if(m_currentWaypointIndex == 2) {
        m_carSprite->setRotation(210);
    }
    else if (m_currentWaypointIndex == 3) {
        m_carSprite->setRotation(150);
    }
    else if (m_currentWaypointIndex == 4) {
        m_carSprite->setRotation(90);
    }
    else if (m_currentWaypointIndex == 5) {
        m_carSprite->setRotation(45);
    }
    else if (m_currentWaypointIndex == 6) {
        m_carSprite->setRotation(0);
     }
     else if (m_currentWaypointIndex ==7) {
        m_carSprite->setRotation(-45);
     }
     else if (m_currentWaypointIndex ==8) {
        m_carSprite->setRotation(0);
     }
     else if (m_currentWaypointIndex ==9) {
        m_carSprite->setRotation(90);
     }
     else if (m_currentWaypointIndex ==10) {
        m_carSprite->setRotation(180);
     }  else if (m_currentWaypointIndex ==11) {
        m_carSprite->setRotation(270);
     }else if (m_currentWaypointIndex ==12) {
        m_carSprite->setRotation(225);
     }else if (m_currentWaypointIndex ==13) {
        m_carSprite->setRotation(250);
     }else if (m_currentWaypointIndex ==14) {
        m_carSprite->setRotation(270);
     }
    else {
        double angle = atan2(deltaY, deltaX) * 180.0 / M_PI;
        
        if (angle < 0) {
            angle += 360;
        }
        
        m_carSprite->setRotation(angle);
    }
}

void GameView::onGameEngineChanged()
{
    if (m_gameEngine) {
     
        
       
        if (m_gameEngine->vehicle()) {
            connect(m_gameEngine->vehicle(), &VehicleModel::positionChanged,
                    this, &GameView::updateCarPosition);
        }
    }
}

void GameView::updateCarPosition()
{
    if (m_gameEngine && m_gameEngine->vehicle()) {
        QPointF vehiclePos = m_gameEngine->vehicle()->position();
       
    }
}

void GameView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    
   
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void GameView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
       
        QPointF scenePos = mapToScene(event->pos());
        
        
        m_waypoints.append(scenePos);
        
       
        QGraphicsEllipseItem *marker = m_scene->addEllipse(scenePos.x() - 3, scenePos.y() - 3, 6, 6, 
                                                          QPen(Qt::red, 2), QBrush(Qt::red));
        marker->setZValue(5); 
        
       
        
        
        if (m_waypoints.size() > 1) {
            QPointF prevPos = m_waypoints[m_waypoints.size() - 2];
            QGraphicsLineItem *line = m_scene->addLine(prevPos.x(), prevPos.y(), scenePos.x(), scenePos.y(), 
                                                      QPen(Qt::yellow, 2, Qt::DashLine));
            line->setZValue(4); 
        }
    } else if (event->button() == Qt::RightButton) {
       
        m_waypoints.clear();
        m_currentWaypointIndex = 0;
        m_carProgress = 0.0;
        
       
        QList<QGraphicsItem*> items = m_scene->items();
        for (QGraphicsItem* item : items) {
            if (item->type() == QGraphicsEllipseItem::Type || item->type() == QGraphicsLineItem::Type) {
                if (item != m_roadBackground && item != m_carSprite) {
                    m_scene->removeItem(item);
                    delete item;
                }
            }
        }
        
       
        QPointF startPos(200, 660);
        m_waypoints.append(startPos);
        if (m_carSprite) {
            m_carSprite->setPos(startPos);
            m_carCurrentPos = startPos;
        }
        
       
    }
    
    QGraphicsView::mousePressEvent(event);
} 