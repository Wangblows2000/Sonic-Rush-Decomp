#ifndef MAIN_A2_G3DRS_H
#define MAIN_A2_G3DRS_H
#include <nitro/types.h>

typedef struct UnkRS UnkRS;
typedef void (*UnkRSCb)(UnkRS *rs);
struct UnkRS {
    u8 *c;
    void *obj;
    u32 flag;
    UnkRSCb cb[0x20];
    u8 timing[0x20];
    u8 curNode;
    u8 curMat;
    u8 curNodeDesc;
    u8 unkAF;
    void *pMatAnm;
    void *pJntAnm;
    BOOL *pVisible;
    u8 unkBC[8];
    u32 unkC4;
    u8 unkC8[0xC];
    void *unkD4;
    void *resMat;
    void *resShp;
    u32 unkE0;
    u32 unkE4;
    void *unkE8;
    void *unkEC;
    void *unkF0;
    u8 unkF4[0x38];
    u8 tmpJntAnm[0x58];
    BOOL visible;
};

typedef struct { u8 revision; u8 numEntry; u16 sizeDictBlk; u16 dummy; u16 ofsEntry; } UnkResDict;
typedef struct { u16 sizeUnit; u16 ofsName; u8 data[4]; } UnkResDictEntry;

#define UNK_DICT_DATA(dict, idx) \
    ((void *)(((UnkResDictEntry *)((u8 *)(dict) + (dict)->ofsEntry))->data + \
              (idx) * ((UnkResDictEntry *)((u8 *)(dict) + (dict)->ofsEntry))->sizeUnit))

static inline int UnkRS_GetTiming(UnkRS *rs, int cmd) {
    return rs->cb[cmd] ? rs->timing[cmd] : 0;
}

static inline BOOL UnkRS_CbA(UnkRS *rs, int cmd, int *t) {
    *t = UnkRS_GetTiming(rs, cmd);
    if (*t == 1) {
        rs->flag &= ~0x40;
        rs->cb[cmd](rs);
        *t = UnkRS_GetTiming(rs, cmd);
        return rs->flag & 0x40;
    }
    return FALSE;
}

static inline void UnkRS_CbB(UnkRS *rs, int cmd, int *t) {
    if (*t == 2) {
        rs->flag &= ~0x40;
        rs->cb[cmd](rs);
        *t = UnkRS_GetTiming(rs, cmd);
    }
}

static inline BOOL UnkRS_CbB2(UnkRS *rs, int cmd, int *t) {
    if (*t == 2) {
        rs->flag &= ~0x40;
        rs->cb[cmd](rs);
        *t = UnkRS_GetTiming(rs, cmd);
        return rs->flag & 0x40;
    }
    return FALSE;
}

static inline void UnkRS_CbC(UnkRS *rs, int cmd, int t) {
    if (t == 3) {
        rs->flag &= ~0x40;
        rs->cb[cmd](rs);
    }
}

#endif
