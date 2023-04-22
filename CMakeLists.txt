cmake_minimum_required(VERSION 3.16)
project(octo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find architecture property
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PROJECT_ARCH_TARGET "amd64")    # x64
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(PROJECT_ARCH_TARGET "i386")     # x86
else ()
    message(FATAL_ERROR "Unknown architecture, CMake will exit.")
endif ()

find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui Network)

add_subdirectory(QCustomPlot)

include_directories(QCustomPlot)

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
        qcustomplot
        )

install(TARGETS octo
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
        )

target_compile_definitions(${PROJECT_NAME} PRIVATE QCUSTOMPLOT_USE_LIBRARY)