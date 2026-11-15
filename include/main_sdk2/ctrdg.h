#ifndef MAIN_SDK2_CTRDG_H
#define MAIN_SDK2_CTRDG_H
#include <nitro/types.h>

typedef struct {
    u32 locked;
    u32 irq;
} UnkCtrdgLock;

typedef struct {
    u32 rom1st;
    u32 rom2nd;
} UnkCtrdgRomCycle;

typedef struct {
    u16 moduleID;      /* 0 */
    u8 exLsiID[3];     /* 2 */
    u8 isAgbCartridge : 1; /* 5 */
    u8 detectPullOut : 1;
    u8 rsv : 6;
    u16 makerCode;     /* 6 */
    u32 gameCode;      /* 8 */
} UnkCtrdgModuleInfo;

typedef struct {
    u8 pad0[0xac];
    u32 gameCode;      /* 0xac */
    u16 makerCode;     /* 0xb0 */
    u8 fixed;          /* 0xb2 */
    u8 unitCode;       /* 0xb3 */
    u8 deviceType;     /* 0xb4 */
    u8 exLsiID[3];     /* 0xb5 */
    u8 reserved[4];    /* 0xb8 */
    u8 softVersion;    /* 0xbc */
    u8 complement;     /* 0xbd */
    u16 moduleID;      /* 0xbe */
} UnkCtrdgHeader;

typedef struct {
    u16 unk0;
    u16 lockId;
} UnkCtrdgWork;

#define CTRDG_MODULE_INFO ((UnkCtrdgModuleInfo *)0x027ffc30)
#define CTRDG_HEADER ((UnkCtrdgHeader *)0x08000000)

extern UnkCtrdgWork data_02080f54;

void func_0201aa68(u16 lockId, UnkCtrdgLock *lock);
void func_0201aa40(u16 lockId, UnkCtrdgLock *lock);
void CTRDGi_ChangeLatestAccessCycle(UnkCtrdgRomCycle *c);
void CTRDGi_RestoreAccessCycle(UnkCtrdgRomCycle *c);
#endif
