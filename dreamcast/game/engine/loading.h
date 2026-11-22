/* Loading indicator (spinning ring, bottom-right). loading_begin() before a
 * load, loading_end() after it; the engine's loaders call loading_tick(). */
#ifndef SRDC_LOADING_H
#define SRDC_LOADING_H
void loading_begin(void);
void loading_tick(void);
void loading_end(void);
int loading_active(void);
#include <kos/fs.h>
ssize_t loading_read(file_t f, void *buf, size_t n);
#endif
