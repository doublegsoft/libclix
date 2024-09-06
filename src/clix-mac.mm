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
#import <Cocoa/Cocoa.h>
#import <ApplicationServices/ApplicationServices.h>
#import <Carbon/Carbon.h>

#import "clix-mac.h"
#include "clix-cv.hpp"
#include "clix-fmt.hpp"

@implementation ClixMacSimulator

+ (CFStringRef) createStringForKey:(CGKeyCode)keyCode {
  TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardInputSource();
  CFDataRef layoutData = (CFDataRef) TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
  const UCKeyboardLayout* keyboardLayout = (const UCKeyboardLayout *)CFDataGetBytePtr(layoutData);

  UInt32 keysDown = 0;
  UniChar chars[4];
  UniCharCount realLength;

  UCKeyTranslate(keyboardLayout,
                 keyCode,
                 kUCKeyActionDisplay,
                 0,
                 LMGetKbdType(),
                 kUCKeyTranslateNoDeadKeysBit,
                 &keysDown,
                 sizeof(chars) / sizeof(chars[0]),
                 &realLength,
                 chars);
  CFRelease(currentKeyboard);
  return CFStringCreateWithCharacters(kCFAllocatorDefault, chars, 1);
}

+ (CFMutableDictionaryRef) charAndCodeDict {
  static CFMutableDictionaryRef dict = nil;
  if (dict == nil) {
    dict = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    int nums[128];
    int i;

    for (i = 0; i < 128; i++) {
      nums[i] = i;
      CFStringRef stringRef = [ClixMacSimulator createStringForKey:i];
      CFDictionarySetValue(dict, stringRef, CFNumberCreate(NULL, kCFNumberSInt32Type, &nums[i]));
      CFRelease(stringRef);
    }
  }
  return dict;
}

+ (CGKeyCode) getKeyCodeFromChar:(char)ch {
  CGKeyCode code;
  CFNumberRef codeRef;
  UniChar character = ch;
  CFStringRef stringRef = CFStringCreateWithCharacters(kCFAllocatorDefault, &character, 1);
  codeRef = (CFNumberRef) CFDictionaryGetValue([ClixMacSimulator charAndCodeDict], stringRef);
  CFNumberGetValue(codeRef, kCFNumberSInt32Type, &code);

  CFRelease(stringRef);
  CFRelease(codeRef);
  return code;
}

- (id)initWithDirectory:(NSString*)dir {
  self = [super init];
  if (self) {
    self.workdir = dir;
  }
  return self;
}

- (void) clickAtX:(int)x andY:(int)y {
  CGPoint point = CGPointMake(x, y);
  CGEventRef event = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, point, kCGMouseButtonLeft);
  CGEventPost(kCGHIDEventTap, event);

  CGEventSetType(event, kCGEventLeftMouseUp);
  CGEventPost(kCGHIDEventTap, event);

  CFRelease(event);
}

- (void) moveToX:(int)x andY:(int)y {
  CGPoint point = CGPointMake(x, y);
  CGEventRef moveEvent = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, point, kCGMouseButtonLeft);
  CGEventPost(kCGHIDEventTap, moveEvent);
  CFRelease(moveEvent);
}

- (void) clickWhenVisible:(NSString*)wanted andThen:(void (^)())doSomething {
  int retry = 0;
  int x, y;
  do {
    if (retry >= 10) break;
    NSString* screenshot = [self capture];
    const char* screenshot_path = [screenshot UTF8String];
    const char* image_path = [wanted UTF8String];
    sleep(5);
    clix::cv::match(screenshot_path, image_path, &x, &y);
    if (x != -1) {
      x /= 2;
      y /= 2;
      [self clickAtX:(x + 15) andY:(y + 15)];
      sleep(3);
      
      if (doSomething != nil)
      {
        doSomething();
      }
      break;
    }
    [self scrollTo:200];
    retry++;
    sleep(1);
  } while (1);
}

- (void) clickWhenVisible:(NSString*)wanted andX:(int)x andY:(int)y {
  [self clickWhenVisible:wanted andX:x andY:y byScroll:200];
}

- (void) clickWhenVisible:(NSString*)wanted andX:(int)x andY:(int)y byScroll:(int)delta {
  int retry = 0;
  int fx, fy;
  do {
    if (retry >= 10) break;
    NSString* screenshot = [self capture];
    const char* screenshot_path = [screenshot UTF8String];
    const char* image_path = [wanted UTF8String];
    sleep(5);
    clix::cv::match(screenshot_path, image_path, &fx, &fy);
    if (fx != -1) {
      [self clickAtX:x andY:y];
      sleep(3);
      break;
    }
    [self scrollTo:delta];
    retry++;
    sleep(1);
  } while (1);
}

- (void) scrollTo:(int)delta untilSeen:(NSString*)wanted {
  int retry = 0;
  int fx, fy;
  do {
    if (retry >= 20) break;
    NSString* screenshot = [self capture];
    const char* screenshot_path = [screenshot UTF8String];
    const char* image_path = [wanted UTF8String];
    sleep(5);
    clix::cv::match(screenshot_path, image_path, &fx, &fy);
    if (fx != -1) {
      sleep(3);
      break;
    }
    [self scrollTo:delta];
    retry++;
    sleep(1);
  } while (1);
}

/*
** Scrolls vertical scrollbar with delta offset, if delta is negative,
** scroll to top and if delta is positive, scroll to bottom.
*/
- (void) scrollTo:(int)delta {
  CGEventSourceRef eventSource = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
  CGPoint mouseLocation = CGEventGetLocation(CGEventCreate(NULL));

  // Create a new scroll event
  CGEventRef scrollEvent = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 1, delta);
  if (scrollEvent) {
    CGEventSetLocation(scrollEvent, mouseLocation);
    CGEventPost(kCGHIDEventTap, scrollEvent);
    CFRelease(scrollEvent);
  }
  CFRelease(eventSource);
}

- (void) pasteFromText:(NSString*)text {
  NSPasteboard *pasteBoard = [NSPasteboard generalPasteboard];
  [pasteBoard declareTypes:[NSArray arrayWithObjects:NSPasteboardTypeString, nil] owner:nil];
  [pasteBoard setString:text forType:NSPasteboardTypeString];
  
  

  /*!
  ** SELECT ALL
  */
#if defined(__clang__) && (__clang_major__ >= 14)
  CGKeyCode keyCode = [ClixMacSimulator getKeyCodeFromChar:'a'];
  // key down
  CGEventRef keyDownEvent = CGEventCreateKeyboardEvent(NULL, keyCode, true);
  CGEventSetFlags(keyDownEvent, NX_COMMANDMASK);
  CGEventPost(kCGSessionEventTap, keyDownEvent);
  CFRelease(keyDownEvent);
  // key up
  CGEventRef keyUpEvent = CGEventCreateKeyboardEvent(NULL, keyCode, false);
  CGEventPost(kCGSessionEventTap, keyUpEvent);
  CFRelease(keyUpEvent);
#else
  CGEventRef event = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)0x00, true); // a
  CGEventFlags flags = kCGEventFlagMaskCommand;
  CGEventSetFlags(event, flags);
  CGEventPost(kCGHIDEventTap, event);
  CFRelease(event);
#endif  
  sleep(1);
  
  /*!
  ** PASTE
  */
#if defined(__clang__) && (__clang_major__ >= 14)  
  keyCode = [ClixMacSimulator getKeyCodeFromChar:'v'];
  // key down
  keyDownEvent = CGEventCreateKeyboardEvent(NULL, keyCode, true);
  CGEventSetFlags(keyDownEvent, NX_COMMANDMASK);
  CGEventPost(kCGSessionEventTap, keyDownEvent);
  CFRelease(keyDownEvent);
  // key up
  keyUpEvent = CGEventCreateKeyboardEvent(NULL, keyCode, false);
  CGEventPost(kCGSessionEventTap, keyUpEvent);
  CFRelease(keyUpEvent);
#else
  event = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)0x09, true); // v
  flags = kCGEventFlagMaskCommand;
  CGEventSetFlags(event, flags);
  CGEventPost(kCGHIDEventTap, event);
  CFRelease(event);
#endif  
}

/*!
** CAPTURE
*/
- (NSString*) capture {
  NSString* savePath = [NSString stringWithFormat:@"%@/latest.png", self.workdir];
#if defined(__clang__) && (__clang_major__ >= 14)  
  CGImageRef screenshot = CGWindowListCreateImage(CGRectInfinite, kCGWindowListOptionOnScreenOnly, kCGNullWindowID, kCGWindowImageDefault);
  NSBitmapImageRep* bitmap = [[NSBitmapImageRep alloc] initWithCGImage:screenshot];
  if (!bitmap) return @"";
  @try {
    NSData* data = [bitmap representationUsingType:NSBitmapImageFileTypePNG properties:{}];
    [data writeToFile:savePath atomically: NO];
  }
  @catch (NSException* ex) {
    NSLog(@"Caught an exception: %@", ex);
  }
#else
  // Get the main display ID
  CGDirectDisplayID displayID = CGMainDisplayID();

  // Capture the screen
  CGImageRef screenShot = CGDisplayCreateImage(displayID);
  CFStringRef cfstr = (__bridge CFStringRef)savePath;
  CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                               cfstr,
                                               kCFURLPOSIXPathStyle,
                                               false);
  CGImageDestinationRef destination = CGImageDestinationCreateWithURL(url, kUTTypePNG, 1, NULL);
  CGImageDestinationAddImage(destination, screenShot, nil);
  CGImageDestinationFinalize(destination);

  // Release resources
  CFRelease(destination);
  CFRelease(url);
  CGImageRelease(screenShot);
#endif  
  return savePath;
}

- (int) clickAtX:(int)x andY:(int)y ifSeen:(NSString*)wanted {
  int retry = 0;
  int fx, fy;
  do {
    if (retry >= 10) break;
    NSString* screenshot = [self capture];
    const char* screenshot_path = [screenshot UTF8String];
    const char* image_path = [wanted UTF8String];
    sleep(5);
    clix::cv::match(screenshot_path, image_path, &fx, &fy);
    if (fx != -1) {
      [self clickAtX:x andY:y];
      sleep(3);
      break;
    }
    [self scrollTo:200];
    retry++;
    sleep(1);
  } while (1);
  return 0;
}

- (int) clickAtX:(int)x andY:(int)y untilSeen:(NSString*)wanted byScroll:(int)delta {
  int retry = 0;
  int fx, fy;
  do {
    if (retry >= 20) break;
    NSString* screenshot = [self capture];
    const char* screenshot_path = [screenshot UTF8String];
    const char* image_path = [wanted UTF8String];
    sleep(5);
    clix::cv::match(screenshot_path, image_path, &fx, &fy);
    if (fx != -1) {
      [self clickAtX:x andY:y];
      sleep(3);
      break;
    }
    [self scrollTo:delta];
    retry++;
    sleep(1);
  } while (1);
  
  return 0;
}

- (int) clickOnX:(int)x andY:(int)y ifSeen:(NSString*)wanted {
  int retry = 0;
  int fx, fy;
  do {
    if (retry >= 10) break;
    NSString* screenshot = [self capture];
    const char* screenshot_path = [screenshot UTF8String];
    const char* image_path = [wanted UTF8String];
    sleep(5);
    clix::cv::match(screenshot_path, image_path, &fx, &fy);
    if (fx != -1) {
      [self clickAtX:(fx / 2 + x) andY:(fy / 2 + y)];
      sleep(3);
      break;
    }
    [self scrollTo:200];
    retry++;
    sleep(1);
  } while (1);
  return 0;
}

- (int) clickOnX:(int)x andY:(int)y untilSeen:(NSString*)wanted {
  int retry = 0;
  int fx, fy;
  do {
    if (retry >= 20) break;
    NSString* screenshot = [self capture];
    const char* screenshot_path = [screenshot UTF8String];
    const char* image_path = [wanted UTF8String];
    sleep(5);
    clix::cv::match(screenshot_path, image_path, &fx, &fy);
    if (fx != -1) {
      [self clickAtX:(fx / 2 + x) andY:(fy / 2 + y)];
      sleep(3);
      break;
    }
    [self scrollTo:100];
    retry++;
    sleep(1);
  } while (1);
  
  return 0;
}

@end
