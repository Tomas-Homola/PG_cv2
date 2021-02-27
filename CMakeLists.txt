
cmake_minimum_required(VERSION 3.16.0)

PROJECT(ImageViewer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_definitions( "/D_CRT_SECURE_NO_WARNINGS /MP /openmp" )
endif(MSVC)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
	
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED QUIET)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB UI_FILES src/*.ui)
file(GLOB H_FILES src/*.h)
file(GLOB CXX_FILES src/*.cpp)
file(GLOB QRC_FILES src/*.qrc)

add_executable(${PROJECT_NAME} ${CXX_FILES} ${UI_FILES} ${H_FILES} ${QRC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)

add_custom_command(TARGET ${PROJECT_NAME}
                   POST_BUILD
                   COMMAND "windeployqt.exe" "$(OutDir)$(TargetName)$(TargetExt)"
                   COMMENT "Win deploy qt")