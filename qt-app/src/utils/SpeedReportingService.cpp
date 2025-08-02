#include "utils/SpeedReportingService.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>

SpeedReportingService::SpeedReportingService(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_reportingTimer(new QTimer(this))
    , m_speedThreshold(80.0)
    , m_lastSentSpeed(0.0)
    , m_isReporting(false)
    , m_isSubscribed(false)
    , m_clientId("mqttx_ada8b259")
    , m_packetId(1)
{
    setupReportingTimer();
    
    connect(m_socket, &QTcpSocket::connected, this, [this]() {
        qDebug() << "Connected to MQTT broker";
        emit reportingStatusChanged(true);
        // Add a small delay to ensure connection is fully established
        QTimer::singleShot(100, this, &SpeedReportingService::sendMqttConnect);
    });
    
    connect(m_socket, &QTcpSocket::disconnected, this, [this]() {
        qDebug() << "Disconnected from MQTT broker";
        m_isSubscribed = false;
        emit reportingStatusChanged(false);
    });
    
    connect(m_socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
        QString errorMsg = QString("Socket error: %1").arg(m_socket->errorString());
        qDebug() << errorMsg;
        emit errorOccurred(errorMsg);
    });
    
    connect(m_socket, &QTcpSocket::readyRead, this, [this]() {
        QByteArray data = m_socket->readAll();
        qDebug() << "Received data from broker:" << data.toHex();
        
        if (data.size() >= 4 && (data[0] & 0xF0) == 0x20) {
            // CONNACK packet received
            qDebug() << "CONNACK received - connection successful!";
            emit reportingStatusChanged(true);
            
            // Subscribe to the topic for forwarding
            QTimer::singleShot(500, this, [this]() {
                qDebug() << "Subscribing to topic for forwarding";
                sendMqttSubscribe("vehicle/speed/alert");
            });
            
            // Start keep-alive timer
            m_reportingTimer->start();
        } else if (data.size() >= 4 && (data[0] & 0xF0) == 0x90) {
            // SUBACK packet received
            qDebug() << "SUBACK received - subscription successful!";
            m_isSubscribed = true;
            
            // Send a test PUBLISH packet to keep connection alive
            QTimer::singleShot(200, this, [this]() {
                qDebug() << "Sending test PUBLISH packet to keep connection alive";
                sendMqttPublish("vehicle/speed/publish", "{\"test\": \"connection_keepalive\"}");
            });
        } else if (data.size() >= 2 && (data[0] & 0xF0) == 0xD0) {
            // PINGRESP packet received
            qDebug() << "PINGRESP received - keep-alive successful!";
        } else if (data.size() >= 4 && (data[0] & 0xF0) == 0x30) {
            // PUBLISH packet received - forward it to Flutter topic
            qDebug() << "PUBLISH received - forwarding to Flutter";
            handleMqttMessage(data);
        }
    });
}

SpeedReportingService::~SpeedReportingService()
{
    stopReporting();
}

void SpeedReportingService::startReporting(const QString &endpoint)
{
    m_endpoint = endpoint;
    
    QString host = "broker.hivemq.com";
    int port = 1883;
    
    if (endpoint.contains(":")) {
        QStringList parts = endpoint.split(":");
        if (parts.size() >= 2) {
            host = parts[0];
            port = parts[1].toInt();
        }
    } else {
        host = endpoint;
    }
    
    qDebug() << "Connecting to MQTT broker:" << host << ":" << port;
    m_socket->connectToHost(host, port);
    m_isReporting = true;
}

void SpeedReportingService::stopReporting()
{
    m_isReporting = false;
    m_reportingTimer->stop();
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }
    emit reportingStatusChanged(false);
}

bool SpeedReportingService::isReporting() const
{
    return m_isReporting && m_socket->state() == QAbstractSocket::ConnectedState;
}

void SpeedReportingService::onSpeedChanged(double speed)
{
    if (speed >= m_speedThreshold && qAbs(speed - m_lastSentSpeed) > 1.0) {
        // Only send if we're fully connected, subscribed, and reporting
        if (m_socket->state() == QAbstractSocket::ConnectedState && m_isReporting && m_isSubscribed) {
            sendSpeedData(speed);
            m_lastSentSpeed = speed;
        }
    }
}



QByteArray SpeedReportingService::encodeRemainingLength(int length)
{
    QByteArray encoded;
    do {
        int digit = length % 128;
        length = length / 128;
        if (length > 0) {
            digit |= 0x80;
        }
        encoded.append(char(digit));
    } while (length > 0);
    return encoded;
}

void SpeedReportingService::setupReportingTimer()
{
    m_reportingTimer->setSingleShot(false);
    m_reportingTimer->setInterval(30000); // Send PINGREQ every 30 seconds
    connect(m_reportingTimer, &QTimer::timeout, this, [this]() {
        if (m_socket->state() == QAbstractSocket::ConnectedState) {
            sendMqttPingReq();
        }
    });
}

void SpeedReportingService::sendSpeedData(double speed)
{
    if (!isReporting()) {
        return;
    }
    
    QString message = formatSpeedMessage(speed);
    sendMqttPublish("vehicle/speed/publish", message);  // Publish to different topic
    emit speedReported("vehicle/speed/publish", message);
}

QString SpeedReportingService::formatSpeedMessage(double speed)
{
    QJsonObject jsonObj;
    jsonObj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    jsonObj["speed"] = speed;
    jsonObj["unit"] = "km/h";
    jsonObj["threshold_exceeded"] = true;
    jsonObj["vehicle_id"] = m_clientId;
    
    QJsonDocument doc(jsonObj);
    return doc.toJson(QJsonDocument::Compact);
}

void SpeedReportingService::sendMqttConnect()
{
    QByteArray connectPacket;
    
    // Fixed header
    connectPacket.append(char(0x10)); // CONNECT packet type
    
    // Variable header
    QByteArray protocolName = "MQTT";
    QByteArray variableHeader;
    variableHeader.append(char(0x00)); // Protocol name length MSB
    variableHeader.append(char(0x04)); // Protocol name length LSB
    variableHeader.append(protocolName);
    variableHeader.append(char(0x04)); // Protocol level
    variableHeader.append(char(0x02)); // Connect flags (clean session)
    variableHeader.append(char(0x00)); // Keep alive MSB
    variableHeader.append(char(0x3C)); // Keep alive LSB (60 seconds)
    
    // Payload
    QByteArray clientIdBytes = m_clientId.toUtf8();
    QByteArray payload;
    payload.append(char(clientIdBytes.size() >> 8)); // Client ID length MSB
    payload.append(char(clientIdBytes.size() & 0xFF)); // Client ID length LSB
    payload.append(clientIdBytes);
    
    // Calculate remaining length and encode it
    int remainingLength = variableHeader.size() + payload.size();
    QByteArray encodedLength = encodeRemainingLength(remainingLength);
    connectPacket.append(encodedLength);
    connectPacket.append(variableHeader);
    connectPacket.append(payload);
    
    qDebug() << "Sending MQTT CONNECT packet, size:" << connectPacket.size() << "remaining length:" << remainingLength;
    qDebug() << "CONNECT packet hex:" << connectPacket.toHex();
    m_socket->write(connectPacket);
}

void SpeedReportingService::sendMqttPublish(const QString &topic, const QString &message)
{
    QByteArray publishPacket;
    
    // Fixed header
    publishPacket.append(char(0x30)); // PUBLISH packet type
    
    // Variable header
    QByteArray topicBytes = topic.toUtf8();
    QByteArray variableHeader;
    variableHeader.append(char(topicBytes.size() >> 8)); // Topic length MSB
    variableHeader.append(char(topicBytes.size() & 0xFF)); // Topic length LSB
    variableHeader.append(topicBytes);
    
    // Payload
    QByteArray messageBytes = message.toUtf8();
    
    // Calculate remaining length and encode it
    int remainingLength = variableHeader.size() + messageBytes.size();
    QByteArray encodedLength = encodeRemainingLength(remainingLength);
    publishPacket.append(encodedLength);
    publishPacket.append(variableHeader);
    publishPacket.append(messageBytes);
    
    qDebug() << "Sending MQTT PUBLISH packet to topic:" << topic << "size:" << publishPacket.size() << "remaining length:" << remainingLength;
    qDebug() << "PUBLISH packet hex:" << publishPacket.toHex();
    m_socket->write(publishPacket);
}

void SpeedReportingService::sendMqttSubscribe(const QString &topic)
{
    QByteArray subscribePacket;
    
    // Fixed header
    subscribePacket.append(char(0x82)); // SUBSCRIBE packet type
    
    // Variable header
    QByteArray variableHeader;
    variableHeader.append(char(m_packetId >> 8)); // Packet ID MSB
    variableHeader.append(char(m_packetId & 0xFF)); // Packet ID LSB
    
    // Payload
    QByteArray topicBytes = topic.toUtf8();
    QByteArray payload;
    payload.append(char(topicBytes.size() >> 8)); // Topic length MSB
    payload.append(char(topicBytes.size() & 0xFF)); // Topic length LSB
    payload.append(topicBytes);
    payload.append(char(0x00)); // QoS level
    
    // Calculate remaining length and encode it
    int remainingLength = variableHeader.size() + payload.size();
    QByteArray encodedLength = encodeRemainingLength(remainingLength);
    subscribePacket.append(encodedLength);
    subscribePacket.append(variableHeader);
    subscribePacket.append(payload);
    
    qDebug() << "Sending MQTT SUBSCRIBE packet to topic:" << topic << "size:" << subscribePacket.size();
    qDebug() << "SUBSCRIBE packet hex:" << subscribePacket.toHex();
    qDebug() << "Remaining length:" << remainingLength << "Encoded length:" << encodedLength.toHex();
    qDebug() << "Variable header size:" << variableHeader.size() << "Payload size:" << payload.size();
    qDebug() << "Packet ID:" << m_packetId;
    m_socket->write(subscribePacket);
    m_packetId++; // Increment for next packet
}

void SpeedReportingService::sendMqttPingReq()
{
    QByteArray pingPacket;
    pingPacket.append(char(0xC0)); // PINGREQ packet type
    pingPacket.append(char(0x00)); // Remaining length (0)
    
    qDebug() << "Sending MQTT PINGREQ packet";
    qDebug() << "PINGREQ packet hex:" << pingPacket.toHex();
    m_socket->write(pingPacket);
}

void SpeedReportingService::handleMqttMessage(const QByteArray &packet)
{
    // Simple logging for now - don't parse the packet
    qDebug() << "PUBLISH packet received, size:" << packet.size();
    qDebug() << "Packet hex:" << packet.toHex();
    
    // Just acknowledge receipt without parsing
    qDebug() << "Message received on vehicle/speed/alert topic";
} 