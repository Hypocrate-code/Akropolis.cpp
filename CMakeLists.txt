cmake_minimum_required(VERSION 3.16)
project(Akropolis)



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
