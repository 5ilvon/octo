cmake_minimum_required(VERSION 3.16)
project(octo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui Network)

qt_add_executable(octo
        main.cpp
        NetworkManagerWrapper.cpp NetworkManagerWrapper.hpp
        themewidget.cpp themewidget.h themewidget.ui
        )

set_target_properties(octo PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
        )

target_link_libraries(octo PUBLIC
        Qt::Charts
        Qt::Core
        Qt::Gui
        Qt::Network
        )

install(TARGETS octo
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
        )
