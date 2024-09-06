cmake_minimum_required(VERSION 3.13)

project(libclix LANGUAGES CXX)

set(LIBNAME clix)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OSS_ROOT "/export/local/opens")

set(GFC      "3rd/gfc")

set(OPENCV   "${OSS_ROOT}/opencv-3.4.16")

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(BUILD    "build/darwin")
endif()

set(LIBCLIX_SOURCES
  "src/clix-cv.cpp"
  "src/clix-ocr.cpp"
  "src/clix-fmt.cpp"
  "src/clix-mac.mm"
)

set(LIBCLIX_LIBRARIES 
  gfc
  opencv_core
  opencv_imgproc
  opencv_imgcodecs
  "-framework foundation"
  "-framework carbon"
  "-framework cocoa"
)

set(WITH_GFC_CRYPTO OFF CACHE BOOL "gfc crypto" FORCE)
set(WITH_GFC_jVM OFF CACHE BOOL "gfc jvm" FORCE)
set(WITH_GFC_LUA OFF CACHE BOOL "gfc lua" FORCE)
add_subdirectory(${GFC})

include_directories(
  src
  3rd/gfc/include                              
  ${OPENCV}/include                           
  ${OPENCV}/modules/core/include              
  ${OPENCV}/modules/calib3d/include           
  ${OPENCV}/modules/features2d/include        
  ${OPENCV}/modules/flann/include             
  ${OPENCV}/modules/dnn/include               
  ${OPENCV}/modules/highgui/include           
  ${OPENCV}/modules/imgproc/include           
  ${OPENCV}/modules/imgcodecs/include         
  ${OPENCV}/modules/videoio/include           
  ${OPENCV}/modules/ml/include                
  ${OPENCV}/modules/objdetect/include         
  ${OPENCV}/modules/photo/include             
  ${OPENCV}/modules/shape/include             
  ${OPENCV}/modules/stitching/include         
  ${OPENCV}/modules/superres/include          
  ${OPENCV}/modules/video/include             
  ${OPENCV}/modules/videostab/include 
  ${OPENCV}/${BUILD}  
)

link_directories(
  ${OPENCV}/${BUILD}/lib
)

add_library(${LIBNAME} STATIC ${LIBCLIX_SOURCES})
add_library(${LIBNAME}_shared SHARED ${LIBCLIX_SOURCES})
set_target_properties(${LIBNAME}_shared PROPERTIES OUTPUT_NAME ${LIBNAME})

target_link_libraries(${LIBNAME} PUBLIC
  ${LIBCLIX_LIBRARIES}
)

target_link_libraries(${LIBNAME}_shared PUBLIC
  ${LIBCLIX_LIBRARIES}
)

add_executable(test_clix_mac 
  test/test_clix_mac.mm
)

target_link_libraries(test_clix_mac PUBLIC
  ${LIBCLIX_LIBRARIES}
  clix
)
