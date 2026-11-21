#include <nitro/types.h>

extern u32 data_0207d2e4;
extern u32 data_0207d2dc;

void OSi_ExceptionHandler(void);
u32 OS_GetConsoleType(void);

#define HW_EXCP_VECTOR_MAIN (*(u32 *)0x027ffd9c)
extern u8 __DTCM_LO[];
#define HW_EXCP_VECTOR_BUF  (*(u32 *)((u32)__DTCM_LO + 0x3fdc))

void OS_InitException(void)
{
    if (0x2600000 <= HW_EXCP_VECTOR_MAIN && HW_EXCP_VECTOR_MAIN < 0x2800000) {
        data_0207d2e4 = HW_EXCP_VECTOR_MAIN;
    } else {
        data_0207d2e4 = 0;
    }
    if (!data_0207d2e4 || !(OS_GetConsoleType() & 0x40000000)) {
        HW_EXCP_VECTOR_MAIN = (u32)OSi_ExceptionHandler;
        HW_EXCP_VECTOR_BUF = (u32)OSi_ExceptionHandler;
    }
    data_0207d2dc = 0;
}
