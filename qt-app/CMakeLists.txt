cmake_minimum_required(VERSION 3.16)

project(VehicleSpeedCheckout VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt LGPL Compliance: Ensure dynamic linking

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
if(Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_VERSION 6)
else()
    message(FATAL_ERROR "Qt6 not found!")
endif()


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)


    if(MSVC)
        add_compile_options(/Zc:__cplusplus /permissive-)
    endif()
endif()


set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/models/VehicleModel.cpp
    src/core/GameEngine.cpp
    src/views/GameView.cpp
    src/views/ControlPanel.cpp
)


set(HEADERS
    include/views/MainWindow.h
    include/views/GameView.h
    include/views/ControlPanel.h
    include/models/VehicleModel.h
    include/core/GameEngine.h
)


add_executable(VehicleSpeedCheckout
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(VehicleSpeedCheckout PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

target_link_libraries(VehicleSpeedCheckout
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

if(QT_VERSION EQUAL 6)
    set_target_properties(VehicleSpeedCheckout PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
    )
endif()

set_target_properties(VehicleSpeedCheckout PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin)



install(TARGETS VehicleSpeedCheckout
    RUNTIME DESTINATION bin
)

install(DIRECTORY assets/
    DESTINATION bin/assets
)

install(FILES LICENSE
    DESTINATION bin
    RENAME "LICENSE.txt"
)

install(FILES resources/QT_LGPL_NOTICE.txt
    DESTINATION bin
    RENAME "QT_LGPL_NOTICE.txt"
)
