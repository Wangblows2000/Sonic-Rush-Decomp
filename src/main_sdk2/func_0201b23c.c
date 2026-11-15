#include <nitro/types.h>
#include <main_sdk2/b1dc.h>

BOOL func_02009158(u32 a);
void func_020089fc(void *p);
u32 func_020091ac(void);
void DC_InvalidateRange(void *p, u32 size);

BOOL func_0201b23c(void)
{
    if (data_02081044 == 0) {
        if (!func_02009158(data_02081048)) {
            return FALSE;
        }
        if (data_02081040 < 0) {
            data_02081040 = 1;
        }
        func_020089fc(&data_02081080[data_02081040]);
        data_02081048 = func_020091ac();
        if (data_02081040 == 0) {
            data_02081040 = 1;
        } else {
            data_02081040 = 0;
        }
        DC_InvalidateRange(&data_02081080[data_02081040], sizeof(UnkBuf_02081080));
        return TRUE;
    }
    func_020089fc(data_02081080);
    data_02081048 = func_020091ac();
    data_02081044 = 0;
    return FALSE;
}
