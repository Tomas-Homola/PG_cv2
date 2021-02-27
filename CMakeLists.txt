
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
  cmake_policy(SET CMP0071 NEW)
endif()

PROJECT(ImageViewer)

if (MSVC)
	add_definitions( "/D_CRT_SECURE_NO_WARNINGS /wd4996 /wd4244 /wd4267 /wd4334 /MP /openmp" )
endif(MSVC)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
	
find_package(Qt5Widgets REQUIRED QUIET)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB UI_FILES src/*.ui)
file(GLOB QT_WRAP src/*.h)
file(GLOB CXX_FILES src/*.cpp)

qt5_wrap_ui(UISrcs ${UI_FILES})

# CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
add_executable(ImageViewer MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${QT_WRAP})
qt5_use_modules(ImageViewer Core Gui Widgets)
target_link_libraries(ImageViewer)

add_custom_command(TARGET ${PROJECT_NAME}
                   POST_BUILD
                   COMMAND "windeployqt.exe" "$(OutDir)$(TargetName)$(TargetExt)"
                   COMMENT "Win deploy qt")