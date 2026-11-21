#include <nitro/types.h>

void Heap_InitSystem(void);
void Heap_InitMain(void);
void Heap_InitDTCM(void);
void Heap_InitITCM(void);

void Heap_InitAll(void) {
    Heap_InitSystem();
    Heap_InitMain();
    Heap_InitDTCM();
    Heap_InitITCM();
}
