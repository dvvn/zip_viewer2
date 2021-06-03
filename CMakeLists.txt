cmake_minimum_required(VERSION 3.19)
project(QtZipViewerApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(MINIZIP_SRC
       minizip/mz_compat.c
        minizip/mz_crypt.c
       minizip/mz_os.c
        minizip/mz_strm.c
        #[[mz_strm_buf.c
        mz_zip_rw.c
        mz_strm_split.c]]
       minizip/mz_strm_mem.c
        minizip/mz_zip.c)

if(UNIX)
    list(APPEND MINIZIP_SRC minizip/mz_os_posix.c minizip/mz_strm_os_posix.c)
elseif(WIN32)
    list(APPEND MINIZIP_SRC minizip/mz_os_win32.c minizip/mz_strm_os_win32.c)
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME} main.cpp 
        QtZipViewerApp.cpp QtZipViewerApp.h
        QtZipViewerApp.ui QtZipViewerApp.qrc
        ZipVisualizer.cpp ZipVisualizer.h
        ${MINIZIP_SRC})
        
#qt5_use_modules(${PROJECT_NAME} Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)

