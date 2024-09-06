# libclix

```
  ,,    ,,  ,,                 ,,    ,,             
`7MM    db *MM               `7MM    db             
  MM        MM                 MM                   
  MM  `7MM  MM,dMMb.   ,p6"bo  MM  `7MM  `7M'   `MF'
  MM    MM  MM    `Mb 6M'  OO  MM    MM    `VA ,V'  
  MM    MM  MM     M8 8M       MM    MM      XMX    
  MM    MM  MM.   ,M9 YM.    , MM    MM    ,V' VA.  
.JMML..JMML.P^YbmdP'   YMbmd'.JMML..JMML..AM.   .MA.
```

## Mac Platform (clix-mac)

## Computer Vision (clix-cv)

## Optical Character Recognition (clix-ocr)

## Test

```bash

make -f test/Makefile

export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:../opencv-4.8.1/build/darwin/lib
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:../libpng-1.6.40/build/darwin

bin/test_clix_mac

```

# OpenCV (4.8.1)

## Build

Add the lines below in CMakeLists.txt.

```
if (APPLE)
  set(CMAKE_FIND_FRAMEWORK NEVER)
endif()
```

