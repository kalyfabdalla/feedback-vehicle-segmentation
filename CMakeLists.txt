cmake_minimum_required(VERSION 2.8)
project( zivkovic_descriptor C CXX)
find_package( OpenCV REQUIRED )
FIND_PACKAGE(Boost REQUIRED COMPONENTS python )
enable_language(CXX)
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "/usr/include/python2.7")
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
set(ZIV_SRC src/include/descriptor_zivkovic.hpp)
add_library( zivkovic_descriptor SHARED ${ZIV_SRC} )
set_target_properties(zivkovic_descriptor PROPERTIES
	LINKER_LANGUAGE CXX
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
TARGET_LINK_LIBRARIES(zivkovic_descriptor ${Boost_LIBRARIES})
install (TARGETS zivkovic_descriptor
         LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python2.7/site-packages/)
