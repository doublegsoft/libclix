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
#ifndef __CLIX_DESKTOP_H__
#define __CLIX_DESKTOP_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct clix_context_s clix_context_t;

clix_context_t* 
clix_context_init(void);

void
clix_context_free(clix_context_t* ctx);

void
clix_move_to_point(clix_context_t* ctx, int x, int y);

void
clix_click_at_point(clix_context_t* ctx, int x, int y);

void
clix_click_at_offset(clix_context_t* ctx, const char* img, int x, int y, int repeat);

void
clix_dblclick_at_point(clix_context_t* ctx, int x, int y);

void
clix_scroll(clix_context_t* ctx, int delta);

void
clix_paste_from_text(clix_context_t* ctx, const char* text);

void
clix_screen_capture(clix_context_t* ctx, const char* path);

#ifdef __cplusplus
}
#endif

#endif // __CLIX_DESKTOP_H__