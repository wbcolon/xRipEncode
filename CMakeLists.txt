cmake_minimum_required(VERSION 3.16)

project(xRipEncode)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -ggdb -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -pedantic -O3 -DQT_NO_DEBUG_OUTPUT")

find_package(Qt5 COMPONENTS Widgets DBus REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBCDIO libcdio)
pkg_check_modules(LIBCDIO_CDDA libcdio_cdda)
pkg_check_modules(LIBCDIO_PARANOIA libcdio_paranoia)
set(CMAKE_REQUIRED_INCLUDES "${LIBCDIO_INCLUDE_DIRS} ${LIBCDIO_CDDA_INCLUDE_DIRS} ${LIBCDIO_PARANOIA_INCLUDE_DIRS}")
set(CMAKE_REQUIRED_LIBRARIES "${LIBCDIO_LIBRARIES} ${LIBCDIO_CDDA_LIBRARIES} ${LIBCDIO_PARANOIA_LIBRARIES}")

add_executable(xRipEncode
        xApplication.cpp
        xMainAudioRipperWidget.cpp
        xAudioRipper.cpp
        xRipEncode.cpp)

target_link_libraries(xRipEncode Qt5::Widgets Qt5::DBus ${LIBCDIO_PARANOIA_LIBRARIES} ${LIBCDIO_CDDA_LIBRARIES} ${LIBCDIO_LIBRARIES})
