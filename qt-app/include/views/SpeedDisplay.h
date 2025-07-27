#ifndef SPEEDDISPLAY_H
#define SPEEDDISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class QDial;
class QLCDNumber;
class QSlider;
class QPushButton;

class SpeedDisplay : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double currentSpeed READ getCurrentSpeed WRITE setCurrentSpeed)
    Q_PROPERTY(double targetSpeed READ getTargetSpeed WRITE setTargetSpeed)

public:
    explicit SpeedDisplay(QWidget* parent = nullptr);
    ~SpeedDisplay();

  
    void setCurrentSpeed(double speed);
    double getCurrentSpeed() const;
    void setTargetSpeed(double speed);
    double getTargetSpeed() const;
    void setMaxSpeed(double maxSpeed);
    double getMaxSpeed() const;
    void setSpeedUnit(const QString& unit);
    QString getSpeedUnit() const;
    
    
    void setDisplayMode(const QString& mode);
    QString getDisplayMode() const;
    void setTheme(const QString& theme);
    QString getTheme() const;
    void setShowTargetSpeed(bool show);
    bool isShowTargetSpeed() const;
    void setShowSpeedGraph(bool show);
    bool isShowSpeedGraph() const;
    
   
    void enableAnimations(bool enable);
    bool areAnimationsEnabled() const;
    void setAnimationSpeed(int speed);
    int getAnimationSpeed() const;
    
   
    void setSpeedLimit(double limit);
    double getSpeedLimit() const;
    void setSpeedThreshold(double threshold);
    double getSpeedThreshold() const;
    void enableSpeedAlerts(bool enable);
    bool areSpeedAlertsEnabled() const;
    
    
    void setShowAverageSpeed(bool show);
    bool isShowAverageSpeed() const;
    void setShowMaxSpeed(bool show);
    bool isShowMaxSpeed() const;
    void setShowMinSpeed(bool show);
    bool isShowMinSpeed() const;
    
   
    void reset();
    void updateDisplay();
    void refresh();

signals:
    void speedChanged(double speed);
    void targetSpeedChanged(double targetSpeed);
    void speedLimitExceeded(double speed, double limit);
    void speedThresholdReached(double speed, double threshold);
    void displayModeChanged(const QString& mode);
    void themeChanged(const QString& theme);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    void onUpdateTimer();
    void onAnimationFinished();
    void onSpeedChanged(double speed);
    void onTargetSpeedChanged(double targetSpeed);

private:
   
    void setupUI();
    void setupConnections();
    void createSpeedometer();
    void createDigitalDisplay();
    void createAnalogDisplay();
    void createGraphDisplay();
    void createControls();
    
    
    void drawSpeedometer(QPainter& painter);
    void drawDigitalDisplay(QPainter& painter);
    void drawAnalogDisplay(QPainter& painter);
    void drawGraphDisplay(QPainter& painter);
    void drawSpeedNeedle(QPainter& painter, double speed, double angle);
    void drawSpeedArc(QPainter& painter, double startAngle, double endAngle, const QColor& color);
    void drawSpeedText(QPainter& painter, double speed, const QPointF& position);
    void drawSpeedUnit(QPainter& painter, const QPointF& position);
    
   
    void animateSpeedChange(double fromSpeed, double toSpeed);
    void animateColorChange(const QColor& fromColor, const QColor& toColor);
    void startPulseAnimation();
    void stopPulseAnimation();
    
    
    double speedToAngle(double speed) const;
    QColor getSpeedColor(double speed) const;
    QString formatSpeed(double speed) const;
    QRectF getSpeedometerRect() const;
    QPointF getCenterPoint() const;
    double getRadius() const;
    
   
    double m_currentSpeed;
    double m_targetSpeed;
    double m_maxSpeed;
    double m_speedLimit;
    double m_speedThreshold;
    QString m_speedUnit;
    
    
    QString m_displayMode;
    QString m_theme;
    bool m_showTargetSpeed;
    bool m_showSpeedGraph;
    bool m_showAverageSpeed;
    bool m_showMaxSpeed;
    bool m_showMinSpeed;
    bool m_speedAlertsEnabled;
    
   
    bool m_animationsEnabled;
    int m_animationSpeed;
    QPropertyAnimation* m_speedAnimation;
    QPropertyAnimation* m_colorAnimation;
    QPropertyAnimation* m_pulseAnimation;
    QGraphicsOpacityEffect* m_opacityEffect;
    
    
    QDial* m_speedDial;
    QLCDNumber* m_digitalDisplay;
    QSlider* m_targetSpeedSlider;
    QPushButton* m_resetButton;
    QPushButton* m_modeButton;
    QLabel* m_speedUnitLabel;
    QLabel* m_targetSpeedLabel;
    QLabel* m_averageSpeedLabel;
    QLabel* m_maxSpeedLabel;
    QLabel* m_minSpeedLabel;
    QProgressBar* m_speedProgressBar;
    
    
    QTimer* m_updateTimer;
    QTimer* m_pulseTimer;
    
    
    bool m_isMousePressed;
    QPointF m_lastMousePos;
    double m_currentAngle;
    double m_targetAngle;
    QColor m_currentColor;
    QColor m_targetColor;
    
    
    static const double MIN_SPEED;
    static const double DEFAULT_MAX_SPEED;
    static const QString DEFAULT_SPEED_UNIT;
    static const QString DEFAULT_DISPLAY_MODE;
    static const QString DEFAULT_THEME;
    static const int DEFAULT_ANIMATION_SPEED;
    static const int UPDATE_INTERVAL;
    static const int PULSE_INTERVAL;
};

#endif 