cmake_minimum_required(VERSION 3.16)
project(Akropolis)

set(CMAKE_CXX_STANDARD 20)

# Disable AUTOMOC - we'll handle it manually
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set MinGW Qt path (MUST be MinGW version!)
set(QT_MINGW_PATH "C:/Qt/6.9.3/mingw_64")
set(CMAKE_PREFIX_PATH "${QT_MINGW_PATH}")

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Get moc executable path
get_target_property(QT_MOC_EXECUTABLE Qt6::moc IMPORTED_LOCATION)
message(STATUS "MOC executable: ${QT_MOC_EXECUTABLE}")

# Source files
file(GLOB SOURCES "src/*.cpp" "src/UI/*.cpp")
file(GLOB HEADERS "include/*.hpp" "include/UI/*.h")

# Create list of headers that need MOC (those with Q_OBJECT)
set(MOC_HEADERS 
    #"include/UI/HexagonalButton.hpp"
    "include/UI/mainwindow.h"
    "include/UI/GamePushButton.hpp"
    "include/UI/StartMenu.hpp" 
    "include/UI/HexView.hpp"
    "include/UI/HexItem.hpp"
    "include/UI/GameMenu.hpp"
    "include/UI/Dashboard.hpp"
)

# Generate moc files manually
foreach(header ${MOC_HEADERS})
    get_filename_component(header_name ${header} NAME_WE)
    set(moc_output "${CMAKE_CURRENT_BINARY_DIR}/moc_${header_name}.cpp")
    
    # Generate moc file
    add_custom_command(
        OUTPUT ${moc_output}
        COMMAND ${QT_MOC_EXECUTABLE}
        ARGS "${CMAKE_CURRENT_SOURCE_DIR}/${header}" -o "${moc_output}"
        DEPENDS "${header}"
        COMMENT "Generating moc_${header_name}.cpp"
    )
    
    list(APPEND MOC_SOURCES ${moc_output})
endforeach()

# Debug output
message(STATUS "Sources: ${SOURCES}")
message(STATUS "MOC Sources: ${MOC_SOURCES}")

# Create executable with ALL sources including moc files
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${MOC_SOURCES}
    ${HEADERS}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Link Qt
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
)

# MinGW specific
if(MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
    )
endif()

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