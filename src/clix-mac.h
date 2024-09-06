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
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

@interface ClixMacSimulator : NSObject {
  
}

@property (nonatomic, strong) NSString* workdir;
@property (nonatomic, strong) NSString* prompt;

+ (CFMutableDictionaryRef) charAndCodeDict;
+ (CGKeyCode)getKeyCodeFromChar:(char)ch;

- (id)initWithDirectory:(NSString*)workdir;

- (void)moveToX:(int)x andY:(int)y;

- (void)scrollTo:(int)delta;

/*
** Simulates mouse-click operation on specific coordination on screen.
*/
- (void)clickAtX:(int)x andY:(int)y;

/*
** Simulates mouse-click operation on a wanted graphics (or scene) displaying
** on screen.
*/
- (void) clickWhenVisible:(NSString*)wanted andThen:(void (^)())doSomething;

/*
**
*/
- (void) clickWhenVisible:(NSString*)wanted andX:(int)x andY:(int)y;

- (void) clickWhenVisible:(NSString*)wanted andX:(int)x andY:(int)y byScroll:(int)delta;

/*
** Captures a screenshot and save it as an image under a specific directory.
*/
- (NSString*)capture;

/*
** Simulates keypress operation to make the given character to be shown in
** any control which is focused.
*/
// - (void)press:(char)ch;

/*
** Simulates Ctrl+V operation to make the given text into clipboard and
** paste the text from clipboard to any control which is focused.
*/
- (void)pasteFromText:(NSString*)text;

/*
** @since Feb 26, 2024
**
** @version 2.0
*/

/*
** Click a position on screen with x and y, until found a wanted image on
** screen. This method allows to scroll to bottom to find the wanted image.
*/
- (int) clickAtX:(int)x andY:(int)y ifSeen:(NSString*)wanted;

/*
** Click a position on screen with x and y, until found a wanted image on
** screen. This method allows to scroll to bottom to find the wanted image.
*/
- (int) clickAtX:(int)x andY:(int)y untilSeen:(NSString*)wanted byScroll:(int)delta;

/*
** @since May 18, 2024
**
** @version 2.0
*/

/*
** Click a position on image with relative x and relative y, until found
** a wanted image on screen. This method allows to scroll to bottom to
** find the wanted image.
*/
- (int) clickOnX:(int)x andY:(int)y ifSeen:(NSString*)wanted;

/*
** Click a position on image with relative x and relative y, until found
** a wanted image on screen. This method allows to scroll to bottom to
** find the wanted image.
*/
- (int) clickOnX:(int)x andY:(int)y untilSeen:(NSString*)wanted;


/*
** Click a position on screen with x and y, until found a wanted image on
** screen. This method allows to scroll to bottom to find the wanted image.
** If not seen the wanted in time, it would return
** CLIX_ERROR_NOT_SEEN_IMAGE_AFTER_TIMEOUT error.
*/
// - (int) clickAtX:(int)x andY:(int)y untilSeen:(NSString*)wanted onTimeout:(NSUInteger)timeout;

/*
** Click an image, until found a wanted image on screen. This method allows 
** to scroll to bottom to find the wanted image.
*/
// - (int) clickAt:(NSString*)image untilSeen:(NSString*)wanted;

/*
** Click an image, until found a wanted image on screen. This method allows
** to scroll to bottom to find the wanted image. If not seen the wanted in
** time, it would return CLIX_ERROR_NOT_SEEN_IMAGE_AFTER_TIMEOUT error.
*/
// - (int) clickAt:(NSString*)image untilSeen:(NSString*)wanted onTimeout:(NSUInteger)timeout;

/*
** Scroll delta offset, until found a wanted image on
** screen. This method allows to scroll to bottom to find the wanted image.
*/
- (void) scrollTo:(int)delta untilSeen:(NSString*)wanted;

@end


