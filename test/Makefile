CXX = clang++

OPENCV_VERSION = 4.8.1
TESSERACT_VERSION = 5.3.3
PNG_VERSION = 1.6.40

CFLAGS = -Wno-nonnull 																				\
         -Wno-deprecated-declarations             						\
         -I3rd/gfc/include 																		\
         -I../opencv-$(OPENCV_VERSION)/include 								\
				 -I../tesseract-$(TESSERACT_VERSION)/include 					\
				 -I/usr/local/Cellar/leptonica/1.83.1/include 				\
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

all: test_clix_mac test_clix_ocr test_clix_cv export_env

test_clix_mac:
	$(CXX)  $(CFLAGS) $(LDFLAGS) $(FRAMEWORKS)      					\
			$(OPENCV_LIBS)																				\
			-lclix -lgfc			  																	\
  		-o bin/test_clix_mac test/test_clix_mac.mm
	
test_clix_ocr:
	$(CXX)  $(CFLAGS) $(LDFLAGS) 							     						\
			-lgfc -lclix -ltesseract -lleptonica									\
  		-o bin/test_clix_ocr test/test_clix_ocr.cpp
  		
test_clix_cv:
	$(CXX)  $(CFLAGS) $(LDFLAGS) 							     						\
			$(OPENCV_LIBS)																				\
			-lclix -lgfc -lpng			  														\
  		-o bin/test_clix_cv test/test_clix_cv.cpp

export_env:
	export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:../opencv-4.8.1/build/darwin/lib:../libpng-1.6.40/build/darwin
	export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/Volumes/EXPORT/local/works/doublegsoft.open/gfc/03.Development/gfc/build/darwin
#	settings set target.env-vars DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:../opencv-4.8.1/build/darwin/lib:../libpng-1.6.40/build/darwin:/Volumes/EXPORT/local/works/doublegsoft.open/gfc/03.Development/gfc/build/darwin


