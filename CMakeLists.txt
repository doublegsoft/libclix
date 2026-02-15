cmake_minimum_required(VERSION 3.13)

project(libclix LANGUAGES CXX)

set(LIBNAME clix)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
elseif (APPLE)
  set(PLATFORM                        "darwin")
elseif (UNIX)
  set(PLATFORM                        "linux")
endif()

set(BUILD                             "build/${PLATFORM}")

################################################################################
##                                                                            ##
##                                    opencv                                  ##
##                                                                            ##
################################################################################
if (WIN32)
elseif (APPLE)
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(OPENCV_DIR                    "/opt/homebrew/Cellar/opencv/4.13.0_2")
  elseif(DARWIN_MAJOR GREATER_EQUAL 21)
    set(OPENCV_DIR                    "/usr/local/Cellar/opencv/4.11.0_1")
  else()  
    set(OPENCV_DIR                    "/usr/local")
  endif()
elseif (UNIX)
endif()
set(OPENCV_INCLUDE_DIR                "${OPENCV_DIR}/include/opencv4")
set(OPENCV_LIBRARY_DIR                "${OPENCV_DIR}/lib")
set(OPENCV_LIBRARIES                  opencv_core
                                      opencv_imgproc
                                      opencv_imgcodecs)
                                      
################################################################################
##                                                                            ##
##                                 tesseract                                  ##
##                                                                            ##
################################################################################
if (WIN32)
elseif (APPLE)
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(TESSERACT_DIR                 "/opt/homebrew/Cellar/tesseract/5.5.2")
  else()
    set(TESSERACT_DIR                 "/usr/local/Cellar/tesseract/5.5.1")
  endif()
elseif (UNIX)
endif()

set(TESSERACT_INCLUDE_DIR             "${TESSERACT_DIR}/include")
set(TESSERACT_LIBRARY_DIR             "${TESSERACT_DIR}/lib")
set(TESSERACT_LIBRARIES               tesseract)

################################################################################
##                                                                            ##
##                                 leptonica                                  ##
##                                                                            ##
################################################################################
if (WIN32)
elseif (APPLE)
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(LEPTONICA_DIR                 "/opt/homebrew/Cellar/leptonica/1.86.0")
  else()
    set(LEPTONICA_DIR                 "/usr/local/Cellar/leptonica/1.85.0")
  endif()
elseif (UNIX)
endif()
set(LEPTONICA_INCLUDE_DIR             "${LEPTONICA_DIR}/include")
set(LEPTONICA_LIBRARY_DIR             "${LEPTONICA_DIR}/lib")
set(LEPTONICA_LIBRARIES               leptonica)

################################################################################
##                                                                            ##
##                                   gfc                                      ##
##                                                                            ##
################################################################################
set(GFC_DIR                           "3rd/gfc")
set(GFC_INCLUDE_DIR                   "${GFC_DIR}/include")
set(GFC_LIBRARY_DIR                   "${GFC_DIR}/${BUILD}")
set(GFC_LIBRARIES                     gfc)

set(LIBCLIX_SOURCES
  "src/clix-cv.cpp"
  "src/clix-ocr.cpp"
  "src/clix-fmt.cpp"
  "src/clix-mac.mm"
)

set(LIBCLIX_LIBRARIES
  ${GFC_LIBRARIES}
  ${OPENCV_LIBRARIES}
  ${TESSERACT_LIBRARIES}
  ${LEPTONICA_LIBRARIES}
  "-framework foundation"
  "-framework carbon"
  "-framework cocoa"
  "-framework accelerate"
  "-framework opencl"
  "-framework screencapturekit"
)

set(WITH_GFC_CRYPTO OFF CACHE BOOL "gfc crypto" FORCE)
set(WITH_GFC_jVM OFF CACHE BOOL "gfc jvm" FORCE)
set(WITH_GFC_LUA OFF CACHE BOOL "gfc lua" FORCE)
add_subdirectory(${GFC_DIR})

include_directories(
  src
  ${GFC_INCLUDE_DIR}
  ${OPENCV_INCLUDE_DIR}
  ${TESSERACT_INCLUDE_DIR}
  ${LEPTONICA_INCLUDE_DIR}
)

link_directories(
  ${OPENCV_LIBRARY_DIR}
  ${TESSERACT_LIBRARY_DIR}
  ${LEPTONICA_LIBRARY_DIR}
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
  clix
)

add_executable(test_clix_cv
  test/test_clix_cv.cpp
)

target_link_libraries(test_clix_cv PUBLIC
  clix
)
