#include <nitro/types.h>

extern vu16 *data_02072c98[2][4];
extern vu32 *data_02072c90[2];

#define BG_TEXT(cnt)                                   \
    switch ((*(cnt) & 0xc000) >> 14) {                 \
    case 0:                                            \
        *out = (screen == 0) ? 0 : 0xc;                \
        return;                                        \
    case 1:                                            \
        *out = (screen == 0) ? 1 : 0xd;                \
        return;                                        \
    case 2:                                            \
        *out = (screen == 0) ? 2 : 0xe;                \
        return;                                        \
    case 3:                                            \
        *out = (screen == 0) ? 3 : 0xf;                \
        return;                                        \
    }                                                  \
    break;

#define BG_AFFINE(cnt)                                 \
    switch ((*(cnt) & 0xc000) >> 14) {                 \
    case 0:                                            \
        *out = (screen == 0) ? 4 : 0x10;               \
        return;                                        \
    case 1:                                            \
        *out = (screen == 0) ? 5 : 0x11;               \
        return;                                        \
    case 2:                                            \
        *out = 0x12;                                   \
        return;                                        \
    case 3:                                            \
        *out = (screen == 0) ? 7 : 0x13;               \
        return;                                        \
    }                                                  \
    break;

#define BG_EXT(cnt)                                    \
    switch ((*(cnt) & 0xc000) >> 14) {                 \
    case 0:                                            \
        *out = (screen == 0) ? 8 : 0x14;               \
        return;                                        \
    case 1:                                            \
        *out = (screen == 0) ? 9 : 0x15;               \
        return;                                        \
    case 2:                                            \
        *out = (screen == 0) ? 0xa : 0x16;             \
        return;                                        \
    case 3:                                            \
        *out = (screen == 0) ? 0xb : 0x17;             \
        return;                                        \
    }                                                  \
    break;

void Bg_GetScreenFormat(s32 screen, s32 bg, s32 *out, u16 *mode, u16 *prio) {
    vu16 *cnt;

    *mode = 0;
    if (screen == 0)
        *mode = (*(vu32 *)0x04000000 & 0x38000000) >> 27;
    cnt = data_02072c98[screen][bg];
    *prio = (*cnt & 0x1f00) >> 8;
    switch (*data_02072c90[screen] & 7) {
    case 0:
        switch (bg) {
        case 0:
        case 1:
        case 2:
        case 3:
            BG_TEXT(cnt)
        }
        break;
    case 1:
        switch (bg) {
        case 0:
        case 1:
        case 2:
            BG_TEXT(cnt)
        case 3:
            BG_AFFINE(cnt)
        }
        break;
    case 2:
        switch (bg) {
        case 0:
        case 1:
            BG_TEXT(cnt)
        case 2:
        case 3:
            BG_AFFINE(cnt)
        }
        break;
    case 3:
        switch (bg) {
        case 0:
        case 1:
        case 2:
            BG_TEXT(cnt)
        case 3:
            BG_EXT(cnt)
        }
        break;
    case 4:
        switch (bg) {
        case 0:
        case 1:
            BG_TEXT(cnt)
        case 2:
            BG_AFFINE(cnt)
        case 3:
            BG_EXT(cnt)
        }
        break;
    case 5:
        switch (bg) {
        case 0:
        case 1:
            BG_TEXT(cnt)
        case 2:
        case 3:
            BG_EXT(cnt)
        }
        break;
    }
}
