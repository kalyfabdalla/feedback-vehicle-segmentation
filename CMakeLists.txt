cmake_minimum_required(VERSION 2.8)
project( background_estimator C CXX)
find_package( OpenCV REQUIRED )
enable_language(CXX)
set(ZIV_SRC src/include/background_estimator.hpp src/include/background_estimator.cpp)
add_library( background_estimator SHARED ${ZIV_SRC} )
set_target_properties(background_estimator PROPERTIES
	LINKER_LANGUAGE CXX
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
TARGET_LINK_LIBRARIES(background_estimator ${Boost_LIBRARIES})
install (TARGETS background_estimator
         LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python2.7/site-packages/)


project( background_estimator_video C CXX)
find_package( OpenCV REQUIRED )
enable_language(CXX)
INCLUDE_DIRECTORIES("src/include")
set(BEV_SRC src/background_estimator_video.cpp )
add_executable( background_estimator_video ${BEV_SRC} ${ZIV_SRC})
set_target_properties(background_estimator_video PROPERTIES
	LINKER_LANGUAGE CXX
   ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
TARGET_LINK_LIBRARIES(background_estimator_video ${OpenCV_LIBS})
