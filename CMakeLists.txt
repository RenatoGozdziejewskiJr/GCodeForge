cmake_minimum_required(VERSION 3.19)
project(GCodeForge LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(GCodeForge
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    #property browser
    qtpropertybrowser/qtbuttonpropertybrowser.cpp
    qtpropertybrowser/qtbuttonpropertybrowser_p.h
    qtpropertybrowser/qteditorfactory.cpp
    qtpropertybrowser/qteditorfactory_p.h
    qtpropertybrowser/qtgroupboxpropertybrowser.cpp
    qtpropertybrowser/qtgroupboxpropertybrowser_p.h
    qtpropertybrowser/qtpropertybrowser.cpp
    qtpropertybrowser/qtpropertybrowser_p.h
    qtpropertybrowser/qtpropertybrowserutils.cpp
    qtpropertybrowser/qtpropertybrowserutils_p.h
    qtpropertybrowser/qtpropertymanager.cpp
    qtpropertybrowser/qtpropertymanager_p.h
    qtpropertybrowser/qttreepropertybrowser.cpp
    qtpropertybrowser/qttreepropertybrowser_p.h
    qtpropertybrowser/qtvariantproperty.cpp
    qtpropertybrowser/qtvariantproperty_p.h
)

target_link_libraries(GCodeForge
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS GCodeForge
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET GCodeForge
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
