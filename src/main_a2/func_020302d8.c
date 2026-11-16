#include <nitro/types.h>

extern u32 data_02087cb4;
extern u32 data_02087cbc;
extern u32 data_02087cc8;
extern void (*data_02087cb0)(void);

BOOL FS_UnloadOverlay(u32 target, u32 id);
BOOL FS_LoadOverlay(u32 target, u32 id);

void func_020302d8(void) {
    if (data_02087cb4 != data_02087cbc) {
        if (data_02087cc8 != 0) {
            FS_UnloadOverlay(0, data_02087cb4);
            data_02087cc8 = 0;
        }
        if (data_02087cc8 == 0) {
            FS_LoadOverlay(0, data_02087cbc);
            data_02087cb4 = data_02087cbc;
            data_02087cc8 = 1;
        }
    }
    if (data_02087cb0 != NULL) {
        data_02087cb0();
        data_02087cb0 = NULL;
    }
}
