#include <nitro/types.h>

extern u16 data_0207ad9c[];
extern u16 data_022c44f8[];
extern u16 data_022c4500[];
void func_02043010(u16 *info, void *a, u16 b, u16 c, void *data, u16 len);
void func_02042e94(u16 *info, u16 b, u16 c, void *data, u16 len);

#pragma thumb on
void func_0205153c(void) {
    data_0207ad9c[0] = 1;
    switch (data_0207ad9c[1]) {
    case 0:
    case 1:
        func_02043010(data_022c44f8, NULL, 0x200, 0x10, NULL, 0);
        break;
    case 2:
    case 3:
        func_02042e94(data_022c4500, 0x200, 0x10, NULL, 0);
        break;
    }
}
#pragma thumb reset
