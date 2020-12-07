cmake_minimum_required(VERSION 3.16)

project(xRipEncode)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -ggdb -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -pedantic -O3 -DQT_NO_DEBUG_OUTPUT")

find_package(KF5Cddb REQUIRED)
find_package(KF5Config REQUIRED)
find_package(Qt5 COMPONENTS Widgets DBus REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Boost 1.70 REQUIRED)
pkg_check_modules(LIBMUSICBRAINZ5CC libmusicbrainz5cc)
pkg_check_modules(LIBCDIO libcdio)
pkg_check_modules(LIBCDIO_CDDA libcdio_cdda)
pkg_check_modules(LIBCDIO_PARANOIA libcdio_paranoia)
set(CMAKE_REQUIRED_INCLUDES "${LIBMUSICBRAINZ5CC_INCLUDE_DIRS} ${LIBCDIO_INCLUDE_DIRS} ${LIBCDIO_CDDA_INCLUDE_DIRS} ${LIBCDIO_PARANOIA_INCLUDE_DIRS}")
set(CMAKE_REQUIRED_LIBRARIES "${LIBMUSICBRAINZ5CC_LIBRARIES} ${LIBCDIO_LIBRARIES} ${LIBCDIO_CDDA_LIBRARIES} ${LIBCDIO_PARANOIA_LIBRARIES}")

add_executable(xRipEncode
        xRipEncodeConfiguration.cpp
        xRipEncodeConfigurationDialog.cpp
        xReplaceWidget.cpp
        xAudioFile.cpp
        xAudioTracksWidget.cpp
        xAudioCD.cpp
        xMainAudioCDWidget.cpp
        xMovieFile.cpp
        xMovieFileTrack.cpp
        xMainMovieFileWidget.cpp
        xEncodingTracksWidget.cpp
        xMainEncodingWidget.cpp
        xApplication.cpp
        xRipEncode.cpp)

target_link_libraries(xRipEncode KF5::Cddb Qt5::Widgets Qt5::DBus ${Boost_LIBRARIES} ${LIBMUSICBRAINZ5CC_LIBRARIES} ${LIBCDIO_PARANOIA_LIBRARIES} ${LIBCDIO_CDDA_LIBRARIES} ${LIBCDIO_LIBRARIES})
