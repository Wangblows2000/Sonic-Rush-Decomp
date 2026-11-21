#include <nitro/types.h>

extern u16 gDecAnimalCount;

void Decoration_Destructor(void *p);

void DecAnimal_Destructor(void *p) {
    gDecAnimalCount--;
    Decoration_Destructor(p);
}
