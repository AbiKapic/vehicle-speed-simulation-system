#ifndef VEHICLEMODEL_H
#define VEHICLEMODEL_H

#include <QObject>
#include <QPointF>
#include <QPixmap>
#include <QTimer>
#include <QPropertyAnimation>

class VehicleModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged)

public:
    explicit VehicleModel(QObject *parent = nullptr);
    ~VehicleModel();

    
    QPointF position() const;
    void setPosition(const QPointF &pos);
    
    double speed() const;
    void setSpeed(double newSpeed);
    
   
    double maxSpeed() const { return m_maxSpeed; }
    double acceleration() const { return m_acceleration; }
    double deceleration() const { return m_deceleration; }
    
   
    QSizeF size() const { return m_size; }
    QRectF boundingRect() const;
    
    void start();
    void stop();
    void accelerate();
    void decelerate();
    void updatePosition(double deltaTime);
    
    bool isOnRoad(const QRectF &roadBounds) const;
    
    QPixmap currentSprite() const;
    void loadSprites();

signals:
    void positionChanged(const QPointF &position);
    void speedChanged(double speed);
    void vehicleStopped();

private slots:
    void updateAnimation();

private:
    QPointF m_position;
    double m_speed;
    double m_maxSpeed;
    double m_acceleration;
    double m_deceleration;
    QSizeF m_size;
    
    QTimer *m_animationTimer;
    int m_currentFrame;
    QVector<QPixmap> m_sprites;
    bool m_isMoving;
    
    void initializeVehicle();
    void createSimpleCarSprites();
};

#endif 