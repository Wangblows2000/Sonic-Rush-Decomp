#include <nitro/types.h>

extern u8 data_02078b14;
extern void *gNaviBSprite;
void *File_Load(void *a, s32 b);
void NaviB_Load(void) { gNaviBSprite = File_Load(&data_02078b14, 0); }
