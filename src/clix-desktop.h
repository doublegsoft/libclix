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
clix_enter(clix_context_t* ctx);

/*!
** @brief Copy text to system clipboard and simulate select-all and paste actions.
**
** This function:
**   1. Pipes the provided text into the system clipboard via xclip
**   2. Simulates a Select All action (Ctrl+A) into the active context
**   3. Simulates a Paste action (Ctrl+V) into the active context
**
** @param ctx  Pointer to the active clix context.
** @param text Null-terminated C string to be copied and pasted.
**
** @note If `text` is NULL, clipboard copying is skipped, but key events are still sent.
** @note Relies on `xclip` being installed and present in system PATH.
** @warning Assumes a focused input field exists in the active target.
** @warning Overwrites existing system clipboard content if `text` is not NULL.
**
** @sideeffect Modifies system clipboard content and sends simulated keystrokes.
*/
void
clix_paste_from_text(clix_context_t* ctx, const char* text);

/*!
** @brief Stream file contents directly into the system clipboard.
**
** This function:
**   1. Opens the file at the specified `path`
**   2. Streams the file contents in chunks directly to xclip
**   3. Updates the system clipboard with the file content
**
** @param ctx  Pointer to the active clix context.
** @param path Path to the source file to be loaded.
**
** @note Caller must ensure `path` points to a readable file.
** @note Streams in 4096-byte chunks to support arbitrarily large files without large memory allocation.
** @warning Overwrites current system clipboard content.
**
** @sideeffect Modifies system clipboard content.
*/
void
clix_load_from_file(clix_context_t* ctx, const char* path);

void
clix_screen_capture(clix_context_t* ctx, const char* path);

#ifdef __cplusplus
}
#endif

#endif // __CLIX_DESKTOP_H__