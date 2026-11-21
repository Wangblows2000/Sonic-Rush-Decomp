#include <nitro/types.h>

void PM_SendUtilityCommandAsync(u32 a, u32 b, u32 c);
void PM_ForceToPowerOffAsync(u32 b, u32 c) { PM_SendUtilityCommandAsync(0xe, b, c); }
