CXX = clang++

OSS_ROOT = /export/local/opens

OPENCV_VERSION = 3.4.16
TESSERACT_VERSION = 5.3.3
PNG_VERSION = 1.6.40


CXXFLAGS = -Wno-nonnull 																														\
         	 -Wno-deprecated-declarations             																\
           -I3rd/gfc/include 																												\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/include 													\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/core/include 							\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/calib3d/include 					\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/features2d/include 				\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/flann/include 						\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/dnn/include		 						\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/highgui/include 					\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/imgproc/include 					\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/imgcodecs/include 				\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/videoio/include 					\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/ml/include 								\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/objdetect/include 				\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/photo/include 						\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/shape/include 						\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/stitching/include 				\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/superres/include 					\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/video/include 						\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/modules/videostab/include 				\
           -I$(OSS_ROOT)/opencv-$(OPENCV_VERSION)/build/darwin 											\
				   -I/usr/local/Cellar/leptonica/1.83.1/include 														\
           -Isrc

LDFLAGS = -L3rd/gfc/build/darwin 									        		\
          -L../opencv-$(OPENCV_VERSION)/build/darwin/lib			\
          -L../libpng-$(PNG_VERSION)/build/darwin             \
					-L../tesseract-5.3.3/.libs													\
          -L./build/Debug
          
OPENCV_LIBS = -lopencv_core -lopencv_highgui									\
							-lopencv_imgcodecs -lopencv_imgproc

FRAMEWORKS = -framework foundation -framework carbon -framework cocoa -framework appkit


.PHONY: all

all: prerequisite libclix.dylib

prerequisite:
	rm -rf build
	mkdir build	

clix-cv.o: 
	$(CXX) $(CXXFLAGS) -c -o build/clix-cv.o src/clix-cv.cpp

clix-fmt.o: 
	$(CXX) $(CXXFLAGS) -c -o build/clix-fmt.o src/clix-fmt.cpp

clix-mac.o: 
	$(CXX) $(CXXFLAGS) -c -o build/clix-fmt.o src/clix-mac.mm	

libclix.dylib: clix-cv.o clix-fmt.o


