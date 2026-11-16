#include <nitro/types.h>

void SaveData_SetDifficulty(s32 a);

#pragma thumb on
void OptionsMenu_Unk_ov001_022c8924(u8 *p, s32 v) {
    if (*(s32 *)(p + 8) != v) {
        *(s32 *)(p + 8) = v;
        switch (v) {
        case 0:
            SaveData_SetDifficulty(1);
            break;
        case 1:
            SaveData_SetDifficulty(0);
            break;
        }
    }
}
#pragma thumb reset
