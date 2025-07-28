#include "models/VehicleModel.h"
#include <QPainter>
#include <QDir>

VehicleModel::VehicleModel(QObject *parent)
    : QObject(parent)
    , m_position(100, 300)
    , m_speed(0.0)
    , m_maxSpeed(200.0)
    , m_acceleration(50.0)
    , m_deceleration(30.0)
    , m_size(60, 30)
    , m_currentFrame(0)
    , m_isMoving(false)
{
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(100); 
    connect(m_animationTimer, &QTimer::timeout, this, &VehicleModel::updateAnimation);
    
    initializeVehicle();
    loadSprites();
}

VehicleModel::~VehicleModel()
{
}

QPointF VehicleModel::position() const
{
    return m_position;
}

void VehicleModel::setPosition(const QPointF &pos)
{
    if (m_position != pos) {
        m_position = pos;
        emit positionChanged(m_position);
    }
}

double VehicleModel::speed() const
{
    return m_speed;
}

void VehicleModel::setSpeed(double newSpeed)
{
    newSpeed = qBound(0.0, newSpeed, m_maxSpeed);
    if (qAbs(m_speed - newSpeed) > 0.1) {
        m_speed = newSpeed;
        emit speedChanged(m_speed);
    }
}

QRectF VehicleModel::boundingRect() const
{
    return QRectF(m_position.x() - m_size.width() / 2,
                  m_position.y() - m_size.height() / 2,
                  m_size.width(), m_size.height());
}

void VehicleModel::start()
{
    m_isMoving = true;
    m_animationTimer->start();
}

void VehicleModel::stop()
{
    m_isMoving = false;
    m_animationTimer->stop();
    setSpeed(0.0);
    emit vehicleStopped();
}

void VehicleModel::accelerate()
{
    if (m_isMoving) {
        double newSpeed = m_speed + m_acceleration * 0.016;
        setSpeed(newSpeed);
    }
}

void VehicleModel::decelerate()
{
    if (m_isMoving) {
        double newSpeed = m_speed - m_deceleration * 0.016;
        setSpeed(newSpeed);
    }
}

void VehicleModel::updatePosition(double deltaTime)
{
    if (m_isMoving && m_speed > 0) {
       
        double deltaX = m_speed * deltaTime;
        QPointF newPos = m_position + QPointF(deltaX, 0);
        setPosition(newPos);
    }
}

bool VehicleModel::isOnRoad(const QRectF &roadBounds) const
{
    return roadBounds.contains(boundingRect());
}

QPixmap VehicleModel::currentSprite() const
{
    if (m_sprites.isEmpty()) {
        QPixmap defaultSprite(m_size.toSize());
        defaultSprite.fill(Qt::transparent);
        
        QPainter painter(&defaultSprite);
        painter.setRenderHint(QPainter::Antialiasing);
        
        painter.setBrush(QBrush(Qt::blue));
        painter.setPen(QPen(Qt::darkBlue, 2));
        
        painter.drawRoundedRect(5, 5, m_size.width() - 10, m_size.height() - 10, 5, 5);
        
        painter.setBrush(QBrush(Qt::lightGray));
        painter.drawRect(8, 8, 8, 4);
        painter.drawRect(m_size.width() - 16, 8, 8, 4);
        
        painter.setBrush(QBrush(Qt::black));
        painter.drawEllipse(8, m_size.height() - 8, 6, 6);
        painter.drawEllipse(m_size.width() - 14, m_size.height() - 8, 6, 6);
        
        return defaultSprite;
    }
    
    return m_sprites.value(m_currentFrame, m_sprites.first());
}

void VehicleModel::loadSprites()
{
    m_sprites.clear();
    
    
    QString spritePath = ":/assets/vehicles/";
    QDir spriteDir(spritePath);
    
    if (spriteDir.exists()) {
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.jpeg";
        QStringList spriteFiles = spriteDir.entryList(filters, QDir::Files);
        
        for (const QString &file : spriteFiles) {
            QPixmap sprite(spritePath + file);
            if (!sprite.isNull()) {
                m_sprites.append(sprite.scaled(m_size.toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    }
    
  
    if (m_sprites.isEmpty()) {
        createSimpleCarSprites();
    }
}

void VehicleModel::initializeVehicle()
{
    
    m_position = QPointF(100, 300);
    m_speed = 0.0;
    m_currentFrame = 0;
    m_isMoving = false;
}

void VehicleModel::updateAnimation()
{
    if (m_isMoving && !m_sprites.isEmpty()) {
        m_currentFrame = (m_currentFrame + 1) % m_sprites.size();
    }
}

void VehicleModel::createSimpleCarSprites()
{
    
    for (int i = 0; i < 4; ++i) {
        QPixmap sprite(m_size.toSize());
        sprite.fill(Qt::transparent);
        
        QPainter painter(&sprite);
        painter.setRenderHint(QPainter::Antialiasing);
        
        
        QColor carColor = QColor::fromHsv(240 + i * 10, 200, 200); 
        painter.setBrush(QBrush(carColor));
        painter.setPen(QPen(carColor.darker(), 3)); 
        
       
        int offset = i * 2;
        painter.drawRoundedRect(5 + offset, 5, m_size.width() - 10, m_size.height() - 10, 8, 8); 
        
       
        painter.setBrush(QBrush(Qt::lightGray));
        painter.setPen(QPen(Qt::darkGray, 1));
        painter.drawRect(8 + offset, 8, 10, 6);
        painter.drawRect(m_size.width() - 18 + offset, 8, 10, 6);
        
        
        painter.setBrush(QBrush(Qt::black));
        painter.setPen(QPen(Qt::darkGray, 2));
        int wheelOffset = (i % 2) * 3;
        painter.drawEllipse(6 + wheelOffset, m_size.height() - 10, 8, 8);
        painter.drawEllipse(m_size.width() - 14 + wheelOffset, m_size.height() - 10, 8, 8);
        
        m_sprites.append(sprite);
    }
    
  
} 