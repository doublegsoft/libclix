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
#pragma once

#include <string>
#include <functional>

#include "clix-desktop.h"

class ClixDesktopSimulator 
{

public:

ClixDesktopSimulator(const std::string& workdir);

~ClixDesktopSimulator(void);

/*!
** @brief Move mouse cursor to absolute screen coordinates.
**
** This function delegates to lower-level platform API to move
** the mouse pointer to the specified (x, y) position.
**
** @param x Horizontal screen coordinate.
** @param y Vertical screen coordinate.
**
** @note This function does not block.
*/
void moveTo(int x, int y);

/*!
** @brief Scroll screen vertically by given delta.
**
** Positive or negative delta determines scroll direction.
**
** @param delta Scroll amount.
**
** @note Delegates to platform-specific scroll implementation.
*/
void scroll(int delta);

/*!
** @brief Perform a double-click at given screen coordinates.
**
** Executes two consecutive click operations with a short delay
** between them to simulate a real double-click action.
**
** @param x Horizontal screen coordinate.
** @param y Vertical screen coordinate.
**
** @warning Uses usleep(100ms). Blocking call.
*/
void dblclickAt(int x, int y);

/*!
** @brief Perform a single left mouse click at given screen coordinates.
**
** Internally calls the platform-specific click implementation.
**
** @param x Horizontal screen coordinate.
** @param y Vertical screen coordinate.
**
** @note This function does not include any delay.
*/
void clickAt(int x, int y);

void clickWhenVisible(
  const std::string& wanted,
  std::function<void()> doSomething
);

void clickWhenVisible(
  const std::string& wanted,
  int x,
  int y
);

void clickWhenVisible(
  const std::string& wanted,
  int x,
  int y,
  int delta
);

/*!
** @brief Capture current screen and save as latest.png in working directory.
**
** This method:
**   1. Builds screenshot path (workdir + "/latest.png")
**   2. Calls platform capture API
**   3. Returns saved image path
**
** @return Full file path of captured screenshot.
**
** @note A new std::string is constructed each call.
** @note Actual performance cost depends on screen capture implementation.
*/
std::string capture();

/*!
** @brief Simulate paste operation using provided text content.
**
** This function:
**   1. Copies the given text into system clipboard
**   2. Simulates a paste action (e.g., Cmd+V or Ctrl+V depending on platform)
**   3. Pastes text into the currently focused UI element
**
** @param text Null-terminated C string to be pasted.
**
** @note The caller must ensure `text` is a valid null-terminated string.
** @note Behavior depends on platform implementation of clix_paste_from_text().
** @warning This function assumes a focused input field exists.
** @warning Clipboard content may be overwritten.
**
** @sideeffect Modifies system clipboard content.
*/
void pasteFromText(const std::string& text);

int clickAt(
  int x,
  int y,
  const std::string& wanted
);

/*!
** @brief Repeatedly click a fixed point until a target image is detected.
**
** Workflow:
**   1. Capture screenshot
**   2. Wait 5 seconds
**   3. Run template matching
**   4. Retry up to 10 times
**   5. If image found, click specified (x,y)
**
** @param x Fixed X coordinate to click.
** @param y Fixed Y coordinate to click.
** @param wanted Path of template image.
** @param delta Scroll delta (currently unused).
**
** @return Always returns 0 (no failure state implemented).
**
** @warning Blocking due to sleep(5) inside loop.
** @warning retry counter not incremented (bug risk).
*/
int clickAtPointUntilFound(
  int x,
  int y,
  const std::string& wanted,
  int delta
);

/*!
** @brief Attempt to find target image once and click with offset.
**
** Workflow:
**   1. Capture screenshot
**   2. Perform template matching
**   3. If found, click at (x + fx, y + fy)
**
** @param x X offset relative to matched position.
** @param y Y offset relative to matched position.
** @param wanted Template image path.
**
** @return 0 if found and clicked.
**         -1 if image not found.
**
** @warning Blocking due to sleep(5) and sleep(3).
*/
int clickAtOffsetIfFound(
  int x,
  int y,
  const std::string& wanted
);

/*!
** @brief Keep capturing and matching until target image appears,
**        then click with offset.
**
** Workflow:
**   1. Capture screenshot
**   2. Perform template matching
**   3. Retry up to 10 times
**   4. If found, click at offset position
**
** @param x X offset relative to match center.
** @param y Y offset relative to match center.
** @param wanted Template image path.
**
** @return Always returns 0 (no failure state defined).
*/
int clickAtOffsetUntilFound(
  int x,
  int y,
  const std::string& wanted
);

void scrollUntilSeen(
  int delta,
  const std::string& wanted
);

private:

std::string workdir;

std::string prompt;

clix_context_t* ctx;

};
