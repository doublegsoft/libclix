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

  void moveTo(int x, int y);

  void scroll(int delta);

  void dblclickAt(int x, int y);

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

  std::string capture();

  void pasteFromText(const std::string& text);

  int clickAt(
    int x,
    int y,
    const std::string& wanted
  );

  int clickAtUntilFound(
    int x,
    int y,
    const std::string& wanted,
    int delta
  );

  int clickAtOffsetIfFound(
    int x,
    int y,
    const std::string& wanted
  );

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
