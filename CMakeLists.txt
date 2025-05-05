cmake_minimum_required(VERSION 3.19)
project(GCodeForge LANGUAGES CXX)

# config the local shared lib paths
set(LOCAL_LIB_DIR "D:/LibsCpp/msvc-2022")
set(LOCAL_INCLUDE_DIR "D:/LibsCpp/include")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# find docking lib
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_library(ADV_DOCKING_LIB NAMES qtadvanceddockingd.lib PATHS ${LOCAL_LIB_DIR} REQUIRED NO_DEFAULT_PATH)
else()
    find_library(ADV_DOCKING_LIB NAMES qtadvanceddocking.lib PATHS ${LOCAL_LIB_DIR} REQUIRED NO_DEFAULT_PATH)
endif()

qt_add_resources(RESOURCES resources.qrc)

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
    #log messages
    Log/logmessage.cpp
    Log/logmessage.h
    Log/logmessagemodel.cpp
    Log/logmessagemodel.h
    Log/cdockmessages.cpp
    Log/cdockmessages.h
    #resource file
    ${RESOURCES}
)

# Adiciona diretórios de include
target_include_directories(GCodeForge PRIVATE
    ${LOCAL_INCLUDE_DIR}
    # Adicione outros diretórios de include aqui se necessário
)

# Adiciona diretórios de bibliotecas
target_link_directories(GCodeForge PRIVATE ${LOCAL_LIB_DIR})

target_link_libraries(GCodeForge
    PRIVATE
        Qt::Core
        Qt::Widgets
)

target_link_libraries(GCodeForge PRIVATE ${ADV_DOCKING_LIB})

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

# Copia as DLLs necessárias para o diretório de output
add_custom_command(TARGET GCodeForge POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${LOCAL_LIB_DIR}/$<IF:$<CONFIG:Debug>,qtadvanceddockingd.dll,qtadvanceddocking.dll>
        $<TARGET_FILE_DIR:GCodeForge>

)

install(SCRIPT ${deploy_script})
