# Vehicle Speed Checkout

A professional Qt-based application for vehicle speed monitoring and analysis.

## Features

- Real-time speed monitoring
- Data visualization and analysis
- Modular architecture
- Professional UI design
- Cross-platform compatibility

## Project Structure

```
VehicleSpeedCheckout/
├── assets/              # Application assets (images, icons)
├── docs/               # Documentation
├── external/           # External dependencies
├── include/            # Header files
│   ├── core/          # Core application classes
│   ├── controllers/   # Controller classes
│   ├── models/        # Data models
│   ├── views/         # UI components
│   └── utils/         # Utility classes
├── src/               # Source files
│   ├── core/          # Core implementation
│   ├── controllers/   # Controller implementation
│   ├── models/        # Model implementation
│   ├── views/         # View implementation
│   └── utils/         # Utility implementation
├── ui/                # Qt Designer UI files
├── resources/         # Qt resources
├── tests/             # Test files
├── scripts/           # Build and deployment scripts
├── build/             # Build output directory
└── installers/        # Installation packages
```

## Requirements

- **Qt**: 6.9.1 or later (MinGW 64-bit version)
- **C++**: C++17 or later
- **CMake**: 3.16 or later
- **Compiler**: MinGW 13.1.0 64-bit (included with Qt installation)

## Prerequisites

### Installing Qt 6.9.1

1. Download Qt 6.9.1 from [Qt Downloads](https://www.qt.io/download-open-source)
2. During installation, select:
   - Qt 6.9.1 → MinGW 13.1.0 64-bit
   - Qt Creator
   - CMake
   - Ninja
3. The default installation path should be `C:\Qt\6.9.1\mingw_64\`

## Build Instructions

### Using CMake

```bash
# Configure with Qt 6.9.1 MinGW
mkdir build && cd build
cmake .. -G "Ninja" -DCMAKE_PREFIX_PATH="C:/Qt/6.9.1/mingw_64" -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release

# Run (make sure Qt is in your PATH)
./bin/VehicleSpeedCheckout.exe
```

### Setting up PATH for Qt

To run the application, you need Qt libraries in your PATH:

```bash
# Add Qt to PATH (Windows PowerShell)
$env:PATH = "C:\Qt\6.9.1\mingw_64\bin;$env:PATH"

# Or permanently add to system PATH:
# 1. Open System Properties → Environment Variables
# 2. Add C:\Qt\6.9.1\mingw_64\bin to PATH
```

### Using Qt Creator

1. Open Qt Creator
2. File → Open File or Project
3. Select `CMakeLists.txt`
4. Configure and build

### Using Build Script (Windows)

```bash
# Run the build script (automatically sets up Qt environment)
scripts/build.bat
```

This script will:
- Set up the Qt environment
- Configure the project with CMake
- Build the application
- Provide instructions for running

## Usage

1. Launch the application
2. Configure your settings
3. Connect to your vehicle sensors
4. Monitor speed data in real-time

## Development

### Architecture

This project follows the Model-View-Controller (MVC) pattern:

- **Models**: Data structures and business logic
- **Views**: User interface components
- **Controllers**: Application logic and data flow

### Adding Features

1. Create header file in `include/`
2. Create implementation in `src/`
3. Update `CMakeLists.txt` if needed
4. Add tests in `tests/`

### Code Style

- Follow Qt coding conventions
- Use meaningful variable names
- Add comments for complex logic
- Keep functions small and focused

## Testing

```bash
# Run unit tests
cd build
ctest --output-on-failure

# Run specific test
./bin/Release/tests/unit_tests/test_name
```

## Deployment

### Windows

```bash
# Create installer
scripts/create_installer.bat

# Package application
scripts/package.bat
```

### Linux

```bash
# Create AppImage
scripts/create_appimage.sh

# Create DEB package
scripts/create_deb.sh
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests
5. Submit a pull request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Qt LGPL Compliance

This application uses Qt libraries under the GNU Lesser General Public License v. 3 (LGPL v3).

### Compliance Requirements:

- **Dynamic Linking**: Uses Qt via dynamic linking for LGPL compliance
- **Source Availability**: Qt source code available at https://www.qt.io/download-open-source
- **User Rights**: Users can modify and re-link Qt libraries
- **License Notice**: LGPL v3 license included with Qt installation

For complete LGPL compliance information, visit: https://www.qt.io/licensing/open-source-lgpl-obligations

## Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/VehicleSpeedCheckout/issues)
- **Documentation**: [Wiki](https://github.com/yourusername/VehicleSpeedCheckout/wiki)
- **Forum**: [Qt Forum](https://forum.qt.io/)

## Changelog

### Version 1.0.0
- Initial release
- Basic speed monitoring functionality
- Professional UI design
- Modular architecture

## Acknowledgments

- Qt Framework and community
- Open source contributors
- Vehicle sensor technology providers

---

**Note**: This application is designed for educational and non-commercial use. For commercial applications, please ensure compliance with Qt licensing requirements.
