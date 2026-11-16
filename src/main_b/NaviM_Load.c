#include <nitro/types.h>

extern u8 data_0207cf68;
extern void *gNaviMSprite;
void *File_Load(void *a, s32 b);
void NaviM_Load(void) { gNaviMSprite = File_Load(&data_0207cf68, 0); }
