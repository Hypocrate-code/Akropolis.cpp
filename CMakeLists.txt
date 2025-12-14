cmake_minimum_required(VERSION 3.16)
project(Akropolis)

# ========== MSVC SPECIFIC SETTINGS ==========
if(MSVC)
    # Ces options sont REQUISES pour MSVC avec Qt
    add_compile_options("/Zc:__cplusplus")
    add_compile_options("/permissive-")
    message(STATUS "Configuring for MSVC with Qt compatibility")
endif()

set(DEFAULT_QT_PATH "C:\\Qt\\6.9.3\\mingw_64") # Adjust this path as necessary
				#ex "C:\\Qt\\6.9.3\\msvc2019_64" 


# Check if Qt is already found, if not use default path
if(NOT Qt6_FOUND)
    message(STATUS "Qt not found, trying default path: ${DEFAULT_QT_PATH}")
    set(CMAKE_PREFIX_PATH "${DEFAULT_QT_PATH}" ${CMAKE_PREFIX_PATH})
endif()

# Enable Qt auto features (MOC, UIC, RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_CXX_FLAGS "-g")
# Fichiers sources
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.hpp")

# Créer l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Link Qt
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Qt6::Core
        Qt6::Widgets
)


# Force moc generation for MainWindow
qt_generate_moc(
    include/mainwindow.h
    ${CMAKE_CURRENT_BINARY_DIR}/moc_mainwindow.cpp
)

# Add the generated moc file to sources
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/moc_mainwindow.cpp
)

if(WIN32)
    # Obtenir le chemin d'installation de Qt
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    get_filename_component(QT_ROOT_DIR "${QT_BIN_DIR}" DIRECTORY)
    
    # Fonction pour copier les fichiers
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMENT "Déploiement des DLLs Qt..."
        # DLLs principales
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_BIN_DIR}/Qt6Core.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_BIN_DIR}/Qt6Gui.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_BIN_DIR}/Qt6Widgets.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        
        # Créer dossier platforms
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms"
        
        # Plugin Windows
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_ROOT_DIR}/plugins/platforms/qwindows.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/"
    )
endif()