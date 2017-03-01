cmake_minimum_required( VERSION 3.0.0 )

#set(CMAKE_VERBOSE_MAKEFILE on)

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

find_package(Qt5 5.4 COMPONENTS Core Widgets LinguistTools)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

file(GLOB_RECURSE SRC_LIST "*.cpp")

file(GLOB_RECURSE UI_FILES "*.ui")
file(GLOB_RECURSE QRC_FILES "*.qrc")
file(GLOB_RECURSE TS_FILES "*.ts")

file(GLOB RC_FILES "*.rc")

if(MINGW)
	SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O COFF -i <SOURCE> -o <OBJECT>")
endif(MINGW)

#add_definitions(-fPIC)
add_definitions(-std=c++11)
add_definitions(-W -fexceptions -Wunused-variable -Wfatal-errors)

set(EXECUTABLE_OUTPUT_PATH "$${CMAKE_SOURCE_DIR}")

# find_xxxx 语句，必须在project()之后！
find_package(Threads          REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Widgets       REQUIRED)
find_package(Qt5Core          REQUIRED)

# Runs UIC on specified files
qt5_wrap_ui( UI_FILES_H $${UI_FILES} )
# Runs RCC on specified files
qt5_add_resources( QRC_FILES_CPP $${QRC_FILES} )
# Runs lrelease on the specified files

# qt5_create_translation(QM_FILES directory... sources... ts_files... OPTIONS)
#qt5_create_translation(QM_FILES $${TS_FILES})

#http://stackoverflow.com/questions/3125327/how-to-integrate-qt4-qm-files-into-binary-using-cmake-and-qrc

set(lang_qrc "i18n.qrc")
set(TRANSLATIONS_QRC $${CMAKE_BINARY_DIR}/i18n.qrc)
qt5_add_translation(QM_FILES $${TS_FILES})

#configure_file(${lang_qrc} ${lang_qrc} COPYONLY)
if(NOT EXISTS "$${TRANSLATIONS_QRC}")
    file(WRITE $${TRANSLATIONS_QRC} "<RCC>\n\t<qresource prefix=\"/translators\">")
    foreach(QM_FILE $${QM_FILES})
        get_filename_component(QM_FILE_NAME $${QM_FILE} NAME)
        file(APPEND $${TRANSLATIONS_QRC} "\n\t\t<file alias=\"$${QM_FILE_NAME}\">$${QM_FILE_NAME}</file>")
    endforeach()
    file(APPEND $${TRANSLATIONS_QRC} "\n\t</qresource>\n</RCC>")
endif()
qt5_add_resources(QM_QRC $${TRANSLATIONS_QRC})

# Runs MOC on specified files
#qt5_wrap_cpp( MOC_CPP_FILES $${MOC_HEADERS})

set(ALL_SOURCES $${SRC_LIST} $${UI_FILES_H} $${QRC_FILES_CPP} $${QM_QRC} $${MOC_CPP_FILES})

if(WIN32)
    list( APPEND ALL_SOURCES $${RC_FILES} )
endif(WIN32)

if(WIN32)
    add_executable($${PROJECT_NAME} WIN32 $${ALL_SOURCES})
else(WIN32)
    add_executable($${PROJECT_NAME} $${ALL_SOURCES})
endif(WIN32)

target_link_libraries($${PROJECT_NAME} Qt5::Widgets Qt5::Core Threads::Threads)

qt5_use_modules($${PROJECT_NAME} Widgets Network Concurrent)
#qt5_use_modules($${PROJECT_NAME} Widgets Xml XmlPatterns PrintSupport Svg WebKit WebKitWidgets Network Concurrent)
