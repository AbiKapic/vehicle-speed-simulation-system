# Vehicle Speed Simulation System

A Qt-based 2D vehicle simulation application featuring a car driving on professional road graphics with real-time speed control and future MQTT integration capabilities.

## 🚗 Features

- **2D Road Simulation**: Vehicle automatically drives on a professional road background
- **Real-time Speed Control**: Adjust vehicle speed using slider and buttons
- **Start/Stop Controls**: Start and stop the vehicle simulation
- **Waypoint Navigation**: Car follows a predefined path with smooth movement
- **Professional Graphics**: Uses high-quality road assets for realistic appearance
- **LGPL Compliant**: Fully compliant with Qt LGPL v3 licensing requirements
- **MQTT Speed Reporting**: Real-time MQTT speed data transmission when speed exceeds 80 km/h

## 📋 Requirements

- Qt 6.9.1 or later
- MinGW 13.1.0 64-bit compiler (Windows)
- C++17 compatible compiler
- CMake 3.16 or later

## 🏗️ Building the Project

### Using Qt Creator (Recommended)
1. Open Qt Creator
2. Open the project file `CMakeLists.txt`
3. Configure the project to use MinGW 13.1.0 64-bit kit
4. Build and run the project

### Using Command Line
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -G "Ninja" -DCMAKE_PREFIX_PATH="C:/Qt/6.9.1/mingw_64" -DCMAKE_BUILD_TYPE=Debug

# Build the project
cmake --build . --config Debug

# Run the application
./bin/VehicleSpeedCheckout.exe
```

## 🎮 Usage

1. **Start Simulation**: Click the "Start" button to begin vehicle movement
2. **Speed Control**: Use the slider or +/- buttons to control vehicle speed
3. **Stop Simulation**: Click "Stop" to halt the vehicle
4. **Real-time Display**: Watch the vehicle move along the road path
5. **MQTT Speed Reporting**: When speed exceeds 80 km/h, data is automatically sent via MQTT
6. **Interactive Waypoints**: Click on the road to add custom waypoints (right-click to reset)

### Testing MQTT Speed Reporting
To test the MQTT speed reporting functionality:
1. Use MQTTX or any MQTT client to connect to `broker.hivemq.com:1883`
2. Subscribe to topic `vehicle/speed/alert`
3. Run the simulation and accelerate the vehicle above 80 km/h
4. You should see JSON messages with speed data in your MQTT client

## 📁 Project Structure

```
qt-app/
├── src/
│   ├── main.cpp              # Main application entry point
│   ├── mainwindow.cpp        # Main window implementation
│   ├── models/
│   │   ├── VehicleModel.cpp  # Vehicle physics and movement
│   │   └── RoadModel.cpp     # Road generation and management
│   ├── core/
│   │   └── GameEngine.cpp    # Main game loop and physics
│   └── views/
│       ├── GameView.cpp      # 2D graphics rendering
│       └── ControlPanel.cpp  # UI controls
├── include/
│   ├── views/
│   │   ├── MainWindow.h      # Main window interface
│   │   ├── GameView.h        # Graphics view interface
│   │   └── ControlPanel.h    # Control panel interface
│   ├── models/
│   │   ├── VehicleModel.h    # Vehicle model interface
│   │   └── RoadModel.h       # Road model interface
│   └── core/
│       └── GameEngine.h      # Game engine interface
├── assets/
│   └── road_textures/
│       └── road.png          # Main road background
├── resources/
│   └── resources.qrc         # Qt resource definitions
├── CMakeLists.txt            # CMake build configuration
├── LICENSE                   # MIT license with Qt LGPL notice
└── README.md                 # This file
```

## 🔧 Technical Details

### Architecture
- **Framework**: Qt 6.9.1 (LGPL v3)
- **Language**: C++17
- **Graphics**: QGraphicsView/QGraphicsScene for 2D rendering
- **Animation**: QTimer-based smooth vehicle movement
- **Build System**: CMake with Ninja generator

### MQTT Speed Reporting Configuration
- **Broker**: HiveMQ public broker (broker.hivemq.com:1883)
- **Topic**: `vehicle/speed/alert`
- **Client ID**: `mqttx_ada8b259`
- **Threshold**: 80 km/h
- **Message Format**: JSON with timestamp, speed, unit, and vehicle ID

Example MQTT message:
```json
{
  "timestamp": "2024-01-15T10:30:45.123Z",
  "speed": 85.5,
  "unit": "km/h",
  "threshold_exceeded": true,
  "vehicle_id": "mqttx_ada8b259"
}
```

### Key Components
- **GameView**: Handles 2D graphics rendering and car animation
- **VehicleModel**: Manages vehicle physics and sprite animation
- **GameEngine**: Controls game loop and timing
- **ControlPanel**: Provides user interface controls
- **RoadModel**: Manages road assets and collision detection

### Vehicle Movement
- **Waypoint System**: Car follows predefined waypoints for realistic road navigation
- **Smooth Animation**: Interpolated movement between waypoints
- **Rotation**: Car automatically rotates to face movement direction
- **Speed Control**: Real-time speed adjustment with physics-based movement

## 📱 Future Features

- **MQTT Integration**: Real-time speed data transmission to mobile apps
- **Flutter Mobile App**: Real-time notifications and speed display
- **Enhanced Graphics**: More detailed vehicle sprites and road variety
- **Sound Effects**: Engine sounds and environmental audio
- **Multiple Vehicles**: Support for multiple cars on the road
- **Traffic Simulation**: AI-controlled vehicles and traffic lights

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

### Third-Party Components

1. **Qt 6.9.1** (LGPL v3)
   - Copyright (c) The Qt Company Ltd.
   - https://www.qt.io/licensing/open-source-lgpl-obligations

2. **Road Assets** (CC0)
   - Professional road graphics for realistic simulation
   - Creative Commons Zero license

### LGPL Compliance

This application uses Qt libraries under LGPL v3, which allows you to:
- Run this program for any purpose
- Study how the program works and adapt it
- Redistribute copies
- Improve the program and release improvements

For complete LGPL compliance information, visit:
https://www.qt.io/licensing/open-source-lgpl-obligations

## 🤝 Contributing

Contributions are welcome! Please ensure your contributions comply with the MIT license and Qt LGPL requirements.

## 📞 Support

For questions about:
- **LGPL compliance**: https://www.qt.io/licensing/open-source-lgpl-obligations
- **Qt development**: https://forum.qt.io/
- **Project issues**: Create an issue in the repository

---

**Vehicle Speed Simulation System** - MIT Licensed
Copyright (c) 2025 VehicleSpeedCheckout
