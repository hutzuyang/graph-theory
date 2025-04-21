cmake_minimum_required(VERSION 3.16)
project(GraphGUI)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cpp
    graph/Graph.cpp
    gui/MainWindow.cpp
    gui/GraphWidget.cpp
)

set(HEADERS
    gui/MainWindow.hpp
    gui/GraphWidget.hpp
)

add_executable(GraphGUI
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(GraphGUI
    Qt6::Widgets
)

set_target_properties(GraphGUI PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)
