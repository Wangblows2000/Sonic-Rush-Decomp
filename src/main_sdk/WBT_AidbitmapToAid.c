#include <nitro/types.h>

s32 WBT_AidbitmapToAid(s32 v) { s32 i; for (i = 0; i < 16; i++) { if ((v >> i) & 1) return i; } return -1; }
