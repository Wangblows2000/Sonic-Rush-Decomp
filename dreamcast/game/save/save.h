/* Native save data (Dreamcast port): translation of src/game/SaveData/SaveData_*.
 *
 * The DS game keeps gSaveData (0x314 bytes) in a 0x1F4C-byte backup image:
 *   0x000  12-byte signature (gSaveSignature, read from the ROM by the menus
 *          converter: build/dreamcast/menus/menudata.bin)
 *   0x00C  10 slots of 0x320 bytes (0x314 used): 5 pairs, rotated by the save
 *          counter (SaveData_AdvanceSlot), each pair written twice.
 * The port keeps that image unchanged (same layout, CRC, rotation) in RAM and
 * stores it as one VMU file (save_vmu.c, KOS vmufs + vmu_pkg with the DS banner
 * icon). CARDi_ReadBackup / CARDi_ProgramBackup become reads/writes of the RAM
 * image; a successful SaveData_Commit / SaveData_Format flushes the image.
 * All fields are little-endian on both machines, so a DS backup image is
 * byte-compatible.
 */
#ifndef SRDC_SAVE_H
#define SRDC_SAVE_H
#include <stdint.h>

/* VsRecordEntry (data_022c4854) */
typedef struct {
    uint16_t name[10];
    uint8_t mac[6];
    uint8_t pad1a[2];
    uint8_t len;        /* +0x1C */
    uint8_t cnt[3];     /* +0x1D wins / losses / draws */
} SaveVsEntry;

/* gSaveData (0x022c4680). Field names: offset names of the decompilation,
 * meaning in the comment. */
typedef struct SaveData {
    uint32_t unk0;             /* +0x000 CRC32 of +4..+0x314 (SaveData_CalcChecksum) */
    uint32_t unk4;             /* +0x004 save counter (SaveData_AdvanceSlot) */
    uint32_t unk8;             /* +0x008 flags: 1 easy (SetDifficulty(0)), 2 no time limit,
                                          0x100 player select page, 0x200 sound test, 0x1000 game cleared */
    /* data_022c468c: Sonic */
    uint16_t unkC;             /* +0x00C Sonic story progress (<= 26) */
    uint8_t unkE;              /* +0x00E Sonic lives */
    uint8_t unkF;              /* +0x00F Sonic emeralds (bit 7 reserved) */
    uint32_t unk10[7][3];      /* +0x010 Sonic best score per zone/act */
    /* Blaze */
    uint16_t unk64;            /* +0x064 Blaze story progress (<= 28) */
    uint8_t unk66;             /* +0x066 Blaze lives */
    uint8_t unk67;             /* +0x067 */
    uint32_t unk68[7][3];      /* +0x068 Blaze best score per zone/act */
    /* data_022c473c: time attack */
    uint8_t unkBC[7][3];       /* +0x0BC per stage: bit r = record r set by Blaze */
    uint8_t padD1;
    uint16_t unkD2[7][3][5];   /* +0x0D2 time records (frames, 0xffff = none) */
    uint8_t unk1A4[2][2];      /* +0x1A4 zones 7/8 flags */
    uint16_t unk1A8[2][2][5];  /* +0x1A8 zones 7/8 time records */
    /* data_022c4850: VS */
    uint8_t unk1D0[3];         /* +0x1D0 VS totals */
    uint8_t pad1D3;
    SaveVsEntry unk1D4[10];    /* +0x1D4 VS opponents, most recent first */
} SaveData;

#define SAVE_DATA_SIZE 0x314
#define SAVE_SLOT_STRIDE 0x320
#define SAVE_SLOT_BASE 0xC
#define SAVE_SLOTS 10
#define SAVE_IMAGE_SIZE (SAVE_SLOT_BASE + SAVE_SLOTS * SAVE_SLOT_STRIDE)

/* gSaveState (0x022c4994) */
typedef struct {
    uint16_t lock_id;  /* +0 OS lock id */
    uint8_t b2;        /* +2 current slot pair 0..4 */
    uint8_t flags;     /* +3 1 = loaded, 0x80 = no backup device (saving disabled) */
} SaveState;

extern SaveData gSaveData;
extern SaveState gSaveState;
extern int gSaveDataBusy;

/* result codes of Startup / Load / Commit / Format (SaveMsg modes follow them) */
enum { SAVE_OK = 0, SAVE_ERR_CARD = 1, SAVE_ERR_WRITE = 2, SAVE_ERR_CORRUPT = 3,
       SAVE_NEW = 4, SAVE_ERR_INIT = 5, SAVE_DISABLED = 6 };

/* ------------------------------------------------------- translated API */
int SaveData_Init(void);
void SaveData_InitCRCTable(void);
uint32_t SaveData_CalcChecksum(const void *p);
int SaveData_VerifyChecksum(const void *p);
void SaveData_SetDefaults(SaveData *s);
int SaveData_Validate(void);
int SaveData_Compare(const void *a, const void *b);
int SaveData_ReadSlot(int slot, void *buf);
int SaveData_WriteSlot(int slot, const void *buf);
int SaveData_ReadSlotPair(void *buf);
int SaveData_AdvanceSlot(void);
int SaveData_Save(void);
int SaveData_IsDirty(void);
int SaveData_Load(void);
int SaveData_Commit(void);
int SaveData_FormatSlot(int idx);
int SaveData_FormatAll(void);
int SaveData_Format(void);
int SaveData_SelectLatestSlot(void);
int SaveData_IdentifyBackup(void);
int SaveData_CheckSignature(void);
int SaveData_WriteSignature(void);
int SaveData_Startup(void);
int SaveData_IsEnabled(void);
/* the *OrShowError wrappers: the DS shows a message box (SaveMsg) on an error.
 * Here the code is kept in save_last_error() for the front end, see SAVE notes. */
int SaveData_StartupOrShowError(void (*cb)(void));
int SaveData_LoadOrShowError(void (*cb)(void));
int SaveData_CommitOrShowError(void (*cb)(void));
int SaveData_FormatOrShowError(void);

uint32_t SaveData_GetProgressFlags(void);
void SaveData_SetProgressFlags(uint32_t v);
uint8_t SaveData_GetSonicEmeralds(void);
void SaveData_SetSonicProgress(uint16_t v);
void SaveData_SetBlazeProgress(uint16_t v);
void SaveData_SetDifficulty(int on);
void SaveData_SetTimeLimit(int on);
void SaveData_SetGameCleared(void);
void SaveData_UnlockPlayerSelectPage(void);
void SaveData_UnlockSoundTest(void);
void SaveData_SetLives(int which, uint32_t v, int clamp);
void SaveData_SetLivesBoth(int a, int b);
/* gGameState / data_022c4590 values the DS functions read, passed explicitly */
int SaveData_GetTimeRank(int zone, int act, uint32_t time, int cur_zone, int cur_act);
void SaveData_InsertTimeRecord(int zone, int act, uint32_t time, uint32_t flag, int cur_zone, int cur_act);
void SaveData_StoreLives(int character, uint8_t lives, uint16_t emeralds);
void SaveData_StoreStageResult(int character, int zone, int act, uint8_t lives, uint16_t emeralds, uint32_t score);
void SaveData_RecordVsResult(int kind, const uint16_t *name, uint8_t len, const uint8_t *mac);

/* zone order tables used by SaveData_StoreStageResult (StoryProgress_GetZoneIndex):
 * data_02074bc4 (Sonic) / data_02074bd0 (Blaze), 9 entries each. Set by the menus
 * module from menudata.bin; default identity. */
extern uint8_t g_save_zone_order[2][9];

/* --------------------------------------------------------- port API */
/* boot: SaveData_Startup (reads the VMU image), then SaveData_Load. Returns the
 * Startup code (SAVE_OK, SAVE_NEW, SAVE_ERR_*, SAVE_DISABLED when no VMU). */
int save_load(void);
/* SaveData_Commit (writes the rotated slot pair when gSaveData changed). */
int save_write(void);
int save_last_error(void);
/* signature from menudata.bin (12 bytes). Without it the DS string can not be
 * checked: save_load treats the image as unformatted. */
void save_set_signature(const uint8_t sig[12]);
/* VMU icon: 16 ARGB4444 colours + 32x32 4 bpp (512 bytes), from menudata.bin */
void save_set_icon(const uint16_t pal[16], const uint8_t data[512]);

/* backend (save_vmu.c on KOS; test/ provides a host file backend) */
typedef struct {
    int (*present)(void);                        /* a storage device exists */
    int (*read)(uint8_t *img, int size);         /* 0 = ok, 1 = no file, <0 error */
    int (*write)(const uint8_t *img, int size);  /* 0 = ok */
} SaveBackend;
extern SaveBackend g_save_backend;
/* KOS: installs the VMU backend (save_vmu.c). Call before save_load(). */
void save_vmu_install(void);
extern uint8_t g_save_image[SAVE_IMAGE_SIZE];   /* the DS backup image */
#endif
