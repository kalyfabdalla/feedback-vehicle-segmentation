cmake_minimum_required(VERSION 2.8)
project( background_estimator C CXX)
find_package( OpenCV REQUIRED )
FIND_PACKAGE(Boost REQUIRED COMPONENTS python )
enable_language(CXX)
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "/usr/include/python2.7")
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
set(ZIV_SRC src/include/background_estimator.hpp)
add_library( background_estimator SHARED ${ZIV_SRC} )
set_target_properties(background_estimator PROPERTIES
	LINKER_LANGUAGE CXX
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
TARGET_LINK_LIBRARIES(background_estimator ${Boost_LIBRARIES})
install (TARGETS background_estimator
         LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python2.7/site-packages/)
