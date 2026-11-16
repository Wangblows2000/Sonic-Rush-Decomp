#include <nitro/types.h>

u32 PMi_ReadRegister(u32 cmd, u16 *out);

u32 PM_GetBackLight(BOOL *a, BOOL *b)
{
    u16 status;
    u32 result = PMi_ReadRegister(0, &status);
    if (result != 0) {
        return result;
    }
    if (a != NULL) {
        *a = (status & 8) ? TRUE : FALSE;
    }
    if (b != NULL) {
        *b = (status & 4) ? TRUE : FALSE;
    }
    return result;
}
