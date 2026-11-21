#include <nitro/types.h>

typedef struct { u8 data[0x40]; } UnkStruct_022c6878_e;
typedef struct { u8 pad[0x5c]; UnkStruct_022c6878_e arr[25]; u8 pad2[4]; s32 cnt; } UnkStruct_022c6878;
void Background_Draw(void *p);

void Ending_DrawNextPicture(UnkStruct_022c6878 *p) {
    p->cnt++;
    Background_Draw(&p->arr[p->cnt]);
}
