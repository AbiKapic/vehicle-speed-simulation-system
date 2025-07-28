#include "views/GameView.h"
#include <QResizeEvent>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QLineF>
#include <cmath>
#include <QMouseEvent>

GameView::GameView(QWidget *parent)
    : QGraphicsView(parent)
    , m_gameEngine(nullptr)
    , m_scene(nullptr)
    , m_roadBackground(nullptr)
    , m_carSprite(nullptr)
    , m_animationTimer(nullptr)
    , m_currentWaypointIndex(0)
    , m_carSpeed(1.0)
    , m_isAnimating(false)
    , m_carProgress(0.0)
{
    setupScene();
    defineWaypoints();
    createCarSprite();
    
    // Setup animation timer
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(16); // ~60 FPS
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
    m_scene->setSceneRect(0, 0, 1200, 800); // Large scene for road
    setScene(m_scene);
    
    // Set view properties
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Load road background
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
        
       
        m_roadBackground->setPos(0, 0);
        
        
    } else {
       
           
        QPixmap roadBackground(1200, 800);
        roadBackground.fill(QColor(50, 50, 50)); 
        
        QPainter painter(&roadBackground);
        painter.setRenderHint(QPainter::Antialiasing);
        
      
        painter.setBrush(QBrush(QColor(80, 80, 80)));
        painter.setPen(QPen(QColor(60, 60, 60), 2));
        painter.drawRect(0, 300, 1200, 200); // Main road strip
        
       
        painter.setPen(QPen(Qt::white, 3, Qt::DashLine));
        painter.drawLine(600, 300, 600, 500); // Center line
        
        painter.setPen(QPen(Qt::white, 2, Qt::SolidLine));
        painter.drawLine(0, 350, 1200, 350); // Top lane
        painter.drawLine(0, 450, 1200, 450); // Bottom lane
        
        m_roadBackground = m_scene->addPixmap(roadBackground);
        m_roadBackground->setZValue(-1); // Keep background below all items
        m_roadBackground->setPos(0, 0);
    }
    
  
}

void GameView::createCarSprite()
{
   
    QPixmap carPixmap(40, 20);
    carPixmap.fill(Qt::transparent);
    
    QPainter painter(&carPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
   
    painter.setBrush(QBrush(QColor(255, 0, 0))); 
    painter.setPen(QPen(QColor(200, 0, 0), 2));
    painter.drawRoundedRect(2, 2, 36, 16, 4, 4);
    
   
    painter.setBrush(QBrush(QColor(100, 150, 255)));
    painter.drawRect(6, 4, 8, 6);
    painter.drawRect(26, 4, 8, 6);
    
    
    painter.setBrush(QBrush(QColor(20, 20, 20)));
    painter.drawEllipse(4, 12, 6, 6);
    painter.drawEllipse(30, 12, 6, 6);
    
    m_carSprite = m_scene->addPixmap(carPixmap);
    m_carSprite->setTransformOriginPoint(carPixmap.width() / 2, carPixmap.height() / 2);
    m_carSprite->setZValue(10); 
    
    
    QPointF startPos(100, 700);
    m_carSprite->setPos(startPos);
    m_carCurrentPos = startPos;
    
}

void GameView::defineWaypoints()
{
    m_waypoints.clear();
    
    QPointF startPos(100, 700); 
    
    m_waypoints.append(startPos);
    
  
    m_waypoints.append(QPointF(123.711, 670.291));   
    m_waypoints.append(QPointF(129.335, 264.292));   
    m_waypoints.append(QPointF(328.397, 251.921));   
    m_waypoints.append(QPointF(305.904, 419.494));   
    m_waypoints.append(QPointF(438.613, 511.715));   
    m_waypoints.append(QPointF(533.083, 373.383));   
    m_waypoints.append(QPointF(397.001, 269.916));   
    m_waypoints.append(QPointF(326.148, 174.321));   
    m_waypoints.append(QPointF(483.599, 113.59));    
    m_waypoints.append(QPointF(681.537, 124.836));   
    m_waypoints.append(QPointF(670.291, 295.783));   
    m_waypoints.append(QPointF(621.931, 445.361));   
    m_waypoints.append(QPointF(501.593, 499.344));   
    m_waypoints.append(QPointF(474.602, 624.18));    
    m_waypoints.append(QPointF(338.519, 675.914));   
    m_waypoints.append(QPointF(127.085, 672.54));    
    
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
    
    QLineF line(current, next);
    double angle = line.angle();
    
    
    m_carSprite->setRotation(-angle);
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
        
       
        QPointF startPos(100, 700);
        m_waypoints.append(startPos);
        if (m_carSprite) {
            m_carSprite->setPos(startPos);
            m_carCurrentPos = startPos;
        }
        
       
    }
    
    QGraphicsView::mousePressEvent(event);
} 