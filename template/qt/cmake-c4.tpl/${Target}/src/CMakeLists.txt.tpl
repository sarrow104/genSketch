cmake_minimum_required( VERSION 3.0.0 )

#if(POLICY CMP0020)
#    cmake_policy(SET CMP0020 OLD)
#endif()
#if(POLICY CMP0043)
#    cmake_policy(SET CMP0043 OLD)
#endif()

set(target_name "${Target}")
if ("$${CMAKE_BUILD_TYPE}" STREQUAL "Release")
 add_definitions(-DNODEBUG -O2 -s)
else()
 set(target_name "$${target_name}D")
 add_definitions(-O0 -g -ggdb)
endif()

project($${target_name})
set(EXECUTABLE_OUTPUT_PATH "$${CMAKE_SOURCE_DIR}")

find_package(Qt5 5.4 COMPONENTS Core Widgets LinguistTools)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

file(GLOB_RECURSE SRC_LIST "*.cpp")

file(GLOB_RECURSE UI_FILES "*.ui")
file(GLOB_RECURSE QRC_FILES "*.qrc")
file(GLOB_RECURSE TS_FILES "*.ts")

#add_definitions(-fPIC)
add_definitions(-std=c++11)
add_definitions(-W -fexceptions -Wunused-variable -Wfatal-errors)

# Runs UIC on specified files
qt5_wrap_ui( UI_FILES_H $${UI_FILES} )
# Runs RCC on specified files
qt5_add_resources( QRC_FILES_CPP $${QRC_FILES} )
# Runs lrelease on the specified files
qt5_add_translation( QM_FILES $${TS_FILES} )
# Runs MOC on specified files
#qt5_wrap_cpp( MOC_CPP_FILES ${MOC_HEADERS})

# find_xxxx 语句，必须在project()之后！
find_package(Threads REQUIRED)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

add_executable($${PROJECT_NAME} $${SRC_LIST} $${UI_FILES_H} $${QRC_FILES_CPP} $${QM_FILES} $${MOC_CPP_FILES})
target_link_libraries($${PROJECT_NAME} Qt5::Widgets Qt5::Core Threads::Threads)

qt5_use_modules($${PROJECT_NAME} Widgets Network Concurrent)
#qt5_use_modules(${PROJECT_NAME} Widgets Xml XmlPatterns PrintSupport Svg WebKit WebKitWidgets Network Concurrent)
