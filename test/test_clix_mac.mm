/*
**    ,,    ,,  ,,                 ,,    ,,
**  `7MM    db *MM               `7MM    db
**    MM        MM                 MM
**    MM  `7MM  MM,dMMb.   ,p6"bo  MM  `7MM  `7M'   `MF'
**    MM    MM  MM    `Mb 6M'  OO  MM    MM    `VA ,V'
**    MM    MM  MM     M8 8M       MM    MM      XMX
**    MM    MM  MM.   ,M9 YM.    , MM    MM    ,V' VA.
**  .JMML..JMML.P^YbmdP'   YMbmd'.JMML..JMML..AM.   .MA.
*/
#include <unistd.h>
#import <Cocoa/Cocoa.h>

#import "clix-mac.h"

int main(int argc, const char * argv[]) {
  ClixMacSimulator* sim = [[ClixMacSimulator alloc] initWithDirectory:@"/Users/christian/Paws"];
  
  // 测试点击
//  sleep(2);
//  [sim clickAtX:600 andY:400];
  
  // 测试粘贴
//  sleep(1);
//  [sim pasteFromText:@"hello, world"];
  
  // 截屏
  sleep(1);
  [sim capture];
  
  return 0;
}
