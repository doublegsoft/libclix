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
** Simulates mouse-double-click operation on specific coordination on screen.
*/
- (void)dblclickAtX:(int)x andY:(int)y;

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
- (int) clickAtX:(int)x andY:(int)y ifFound:(NSString*)wanted;

/**
 * 持续滚动页面并检测目标图像是否出现，一旦出现则在指定坐标执行点击操作。
 *
 * 执行流程：
 * 1. 循环最多尝试 20 次：
 *    - 截取当前屏幕截图。
 *    - 在截图中匹配目标图像 (wanted)。
 * 2. 如果检测到目标图像存在：
 *    - 在指定坐标 (x, y) 执行点击。
 *    - 结束循环。
 * 3. 如果未检测到目标图像：
 *    - 按指定滚动量 delta 执行滚动。
 *    - 继续下一轮检测。
 *
 * @param x 需要点击的屏幕横坐标。
 * @param y 需要点击的屏幕纵坐标。
 * @param wanted 需要检测是否出现的目标图像路径或标识。
 * @param delta 每次未检测到目标时执行的滚动距离（正负表示滚动方向）。
 *
 * @return 当前实现固定返回 0（可扩展为成功/失败状态）。
 */
- (int) clickAtX:(int)x andY:(int)y untilFound:(NSString*)wanted byScroll:(int)delta;

/*
** @since May 18, 2024
**
** @version 2.0
*/

/**
 * 尝试在屏幕上查找目标图像，并在目标位置的偏移坐标处执行点击操作。
 *
 * 执行流程：
 * 1. 截取当前屏幕截图。
 * 2. 在截图中匹配目标图像（wanted）。
 * 3. 如果找到目标图像：
 *    - 在检测到的位置基础上，加上指定偏移量 (x, y) 进行点击。
 * 4. 如果未找到目标图像：
 *    - 返回 -1。
 *
 * @param x 相对于检测目标中心点的横向偏移量。
 * @param y 相对于检测目标中心点的纵向偏移量。
 * @param wanted 需要匹配查找的目标图像路径或标识。
 *
 * @return 0 表示成功找到目标并完成点击。
 *         -1 表示未找到目标图像。
 */
- (int) clickAtOffsetX:(int)x andY:(int)y ifFound:(NSString*)wanted;

/**
 * 尝试在屏幕中查找指定图片（wanted），
 * 一旦找到则点击目标位置（带偏移量）。
 *
 * 工作流程：
 * 1. 截图当前屏幕
 * 2. 使用 OpenCV 模板匹配查找 wanted 图片
 * 3. 若找到则点击目标中心点（附加 offset）
 * 4. 若未找到则执行滚动并重试
 *
 * @param x 点击位置的 X 偏移量（用于修正匹配中心点）
 * @param y 点击位置的 Y 偏移量
 * @param wanted 目标模板图片路径
 *
 * @return 始终返回 0（目前未定义失败码）
 */
- (int) clickAtOffsetX:(int)x andY:(int)y untilFound:(NSString*)wanted;


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


