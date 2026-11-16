#include <nitro/types.h>

void WH_End(void);
void func_02041238(void);
void func_02041264(void (**p)(void)) { WH_End(); *p = func_02041238; }
