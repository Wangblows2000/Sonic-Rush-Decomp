/*
 * SaveData_* translated from src/game/SaveData/ (same order of operations,
 * constants and return codes). DS services replaced:
 *   CARD_LockBackup / CARD_UnlockBackup / OS_GetLockID  -> nothing (single thread)
 *   CARDi_ReadBackup / CARDi_ProgramBackup               -> g_save_image (RAM copy of
 *                                                           the backup), flushed to the
 *                                                           VMU by save_flush()
 *   CARD_IdentifyBackup                                  -> g_save_backend.present()
 *   CARD_GetResultCode                                   -> s_card_result
 *   Heap_Alloc/Heap_Free (0x340 scratch buffers)          -> stack buffers
 *   MATH_CalcCRC32 (table from MATHi_CRC32InitTableRev)   -> same reflected CRC-32
 *   SaveMsg_* message boxes                               -> s_last_error (see SAVE notes)
 * Portable C (host + KOS).
 */
#include <string.h>
#include "save.h"

/* struct layout of gSaveData (offsets used by the DS code) */
typedef char save_size_check[sizeof(SaveData) == SAVE_DATA_SIZE ? 1 : -1];
typedef char save_off_check1[__builtin_offsetof(SaveData, unk64) == 0x64 ? 1 : -1];
typedef char save_off_check2[__builtin_offsetof(SaveData, unkD2) == 0xD2 ? 1 : -1];
typedef char save_off_check3[__builtin_offsetof(SaveData, unk1A8) == 0x1A8 ? 1 : -1];
typedef char save_off_check4[__builtin_offsetof(SaveData, unk1D4) == 0x1D4 ? 1 : -1];

SaveData gSaveData;
SaveState gSaveState;
int gSaveDataBusy;
uint8_t g_save_image[SAVE_IMAGE_SIZE];
uint8_t g_save_zone_order[2][9] = { { 0, 1, 2, 3, 4, 5, 6, 7, 8 }, { 0, 1, 2, 3, 4, 5, 6, 7, 8 } };

static uint32_t gSaveCRCTable[256];
static uint8_t gSaveSignature[12];
static int s_have_sig;
static int s_card_result;   /* CARD_GetResultCode: 0 = success */
static int s_image_dirty;
static int s_last_error;
static int s_in_startup;   /* Startup writes the image once, after the signature */

static int nobackend_present(void) { return 0; }
static int nobackend_read(uint8_t *img, int size) { (void)img; (void)size; return -1; }
static int nobackend_write(const uint8_t *img, int size) { (void)img; (void)size; return -1; }
SaveBackend g_save_backend = { nobackend_present, nobackend_read, nobackend_write };

void save_set_signature(const uint8_t sig[12]) {
    memcpy(gSaveSignature, sig, 12);
    s_have_sig = 1;
}
int save_last_error(void) { return s_last_error; }
#ifndef _arch_dreamcast
/* host: the icon only matters to the VMU backend */
void save_set_icon(const uint16_t pal[16], const uint8_t data[512]) {
    (void)pal;
    (void)data;
}
void save_vmu_install(void) {}
#endif

/* image -> device. Called where the DS has finished a CARD write sequence. */
static int save_flush(void) {
    if (!s_image_dirty)
        return 1;
    if (g_save_backend.write(g_save_image, SAVE_IMAGE_SIZE) != 0) {
        s_card_result = 1;
        return 0;
    }
    s_image_dirty = 0;
    s_card_result = 0;
    return 1;
}

/* CARDi_ReadBackup / CARDi_ProgramBackup on the image (TRUE on success) */
static int card_read(uint32_t ofs, void *buf, uint32_t size) {
    if (ofs + size > SAVE_IMAGE_SIZE) {
        s_card_result = 1;
        return 0;
    }
    memcpy(buf, g_save_image + ofs, size);
    s_card_result = 0;
    return 1;
}
static int card_program(uint32_t ofs, const void *buf, uint32_t size) {
    if (ofs + size > SAVE_IMAGE_SIZE) {
        s_card_result = 1;
        return 0;
    }
    memcpy(g_save_image + ofs, buf, size);
    s_image_dirty = 1;
    s_card_result = 0;
    return 1;
}

/* ------------------------------------------------------------------ CRC */
void SaveData_InitCRCTable(void) {
    /* MATHi_CRC32InitTableRev(&gSaveCRCTable, 0xedb88320) */
    uint32_t i, j, r;
    for (i = 0; i < 256; i++) {
        r = i;
        for (j = 0; j < 8; j++)
            r = (r & 1) ? (r >> 1) ^ 0xedb88320u : r >> 1;
        gSaveCRCTable[i] = r;
    }
}

uint32_t SaveData_CalcChecksum(const void *p) {
    /* MATH_CalcCRC32(&gSaveCRCTable, p + 4, 0x310): init ~0, final ~ */
    const uint8_t *b = (const uint8_t *)p + 4;
    uint32_t crc = 0xffffffffu, i;
    if (gSaveCRCTable[1] == 0)
        SaveData_InitCRCTable();
    for (i = 0; i < 0x310; i++)
        crc = gSaveCRCTable[(crc ^ b[i]) & 0xff] ^ (crc >> 8);
    return ~crc;
}

int SaveData_VerifyChecksum(const void *p) {
    uint32_t v;
    memcpy(&v, p, 4);
    return v == SaveData_CalcChecksum(p);
}

int SaveData_Init(void) {
    /* OS_GetLockID() == -3 -> FALSE: no lock ids on the DC */
    gSaveState.lock_id = 0;
    SaveData_InitCRCTable();
    return 1;
}

/* ------------------------------------------------------------ defaults */
void SaveData_SetDefaults(SaveData *s) {
    uint16_t i;
    uint16_t *t;
    memset(s, 0, sizeof(*s));
    s->unkE = 2;       /* sub[0].b2 */
    s->unk66 = 2;      /* sub[1].b2 */
    t = &s->unkD2[0][0][0];
    for (i = 0; i < 0x69; i++)
        t[i] = 0xffff;
    t = &s->unk1A8[0][0][0];
    for (i = 0; i < 0x14; i++)
        t[i] = 0xffff;
    s->unk0 = SaveData_CalcChecksum(s);
}

int SaveData_Validate(void) {
    int ok = 1;
    uint32_t i;
    SaveData *s = &gSaveData;
    if (!SaveData_VerifyChecksum(s)) {
        s->unk0 = SaveData_CalcChecksum(s);
        ok = 0;
    }
    if (s->unkE > 99) {
        s->unkE = 99;
        ok = 0;
    }
    if (s->unk66 > 99) {
        s->unk66 = 99;
        ok = 0;
    }
    if (s->unkC > 26) {
        s->unkC = 26;
        ok = 0;
    }
    if (s->unk64 > 28) {
        s->unk64 = 28;
        ok = 0;
    }
    if (s->unkF & 0x80) {
        s->unkF &= ~0x80;
        ok = 0;
    }
    for (i = 0; i < 21; i++) {
        uint32_t *p32 = &s->unk10[0][0];
        if (p32[i] > 999999) {
            p32[i] = 999999;
            ok = 0;
        }
    }
    for (i = 0; i < 21; i++) {
        uint32_t *p32 = &s->unk68[0][0];
        if (p32[i] > 999999) {
            p32[i] = 999999;
            ok = 0;
        }
    }
    for (i = 0; i < 105; i++) {
        uint16_t *p16 = &s->unkD2[0][0][0];
        if (p16[i] != 0xffff && p16[i] > 36000) {
            p16[i] = 36000;
            ok = 0;
        }
    }
    for (i = 0; i < 20; i++) {
        uint16_t *p16 = &s->unk1A8[0][0][0];
        if (p16[i] != 0xffff && p16[i] > 36000) {
            p16[i] = 36000;
            ok = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (s->unk1D0[i] > 99) {
            s->unk1D0[i] = 99;
            ok = 0;
        }
    }
    for (i = 0; i < 10; i++) {
        SaveVsEntry *e = &s->unk1D4[i];
        int k;
        if (e->len > 10) {
            e->len = 10;
            ok = 0;
        }
        for (k = 0; k < 3; k++) {
            if (e->cnt[k] > 99) {
                e->cnt[k] = 99;
                ok = 0;
            }
        }
    }
    s->unk0 = SaveData_CalcChecksum(s);
    return ok;
}

int SaveData_Compare(const void *a, const void *b) {
    if (!SaveData_VerifyChecksum(a))
        return 0;
    if (!SaveData_VerifyChecksum(b))
        return 0;
    if (SaveData_CalcChecksum(a) != SaveData_CalcChecksum(b))
        return 0;
    return memcmp(a, b, SAVE_DATA_SIZE) == 0;
}

/* ----------------------------------------------------------- slots */
int SaveData_ReadSlot(int slot, void *buf) {
    return card_read((uint32_t)slot * SAVE_SLOT_STRIDE + SAVE_SLOT_BASE, buf, SAVE_DATA_SIZE) != 0;
}

int SaveData_WriteSlot(int slot, const void *buf) {
    return card_program((uint32_t)slot * SAVE_SLOT_STRIDE + SAVE_SLOT_BASE, buf, SAVE_DATA_SIZE) != 0;
}

int SaveData_ReadSlotPair(void *buf) {
    int f = 0;
    uint32_t s = (uint32_t)gSaveState.b2 << 1;
    if (SaveData_ReadSlot((int)s, buf)) {
        if (SaveData_VerifyChecksum(buf))
            return 0;
    } else {
        f = 1;
    }
    if (SaveData_ReadSlot((int)s + 1, buf)) {
        if (SaveData_VerifyChecksum(buf))
            return 0;
    } else if (f) {
        return 1;
    }
    return 3;
}

int SaveData_AdvanceSlot(void) {
    gSaveState.b2 = (uint8_t)((gSaveState.b2 + 1) % 5);
    gSaveData.unk4++;
    if ((gSaveData.unk4 - gSaveState.b2) % 5 != 0)
        return 0;
    gSaveData.unk0 = SaveData_CalcChecksum(&gSaveData);
    return 1;
}

int SaveData_Save(void) {
    uint8_t start = gSaveState.b2;
    if (!SaveData_AdvanceSlot())
        return 2;
    while (start != gSaveState.b2) {
        if (SaveData_WriteSlot(gSaveState.b2 << 1, &gSaveData) &&
            SaveData_WriteSlot((gSaveState.b2 << 1) + 1, &gSaveData) && save_flush())
            return 0;
        if (!SaveData_AdvanceSlot())
            return 2;
    }
    return 2;
}

int SaveData_IsDirty(void) {
    uint8_t p[0x340];
    memset(p, 0, SAVE_DATA_SIZE);
    gSaveData.unk0 = SaveData_CalcChecksum(&gSaveData);
    if (SaveData_ReadSlotPair(p))
        return 1;
    if (SaveData_Compare(&gSaveData, p))
        return 0;
    return 1;
}

int SaveData_Load(void) {
    int r;
    gSaveDataBusy = 1;
    if (gSaveState.flags & 0x80) {
        gSaveDataBusy = 0;
        return 6;
    }
    gSaveState.flags |= 1;
    r = SaveData_ReadSlotPair(&gSaveData);
    if (SaveData_Validate() == 0) {
        gSaveDataBusy = 0;
        return 3;
    }
    gSaveDataBusy = 0;
    return r;
}

int SaveData_Commit(void) {
    gSaveDataBusy = 1;
    SaveData_Validate();
    if (gSaveState.flags & 0x80) {
        gSaveDataBusy = 0;
        return 6;
    }
    if ((gSaveState.flags & 1) == 0) {
        SaveData_Load();
        gSaveDataBusy = 0;
        return 0;
    }
    if (SaveData_IsDirty() == 0) {
        gSaveDataBusy = 0;
        return 0;
    }
    if (SaveData_Save()) {
        gSaveDataBusy = 0;
        return 2;
    }
    gSaveDataBusy = 0;
    return 0;
}

int SaveData_FormatSlot(int idx) {
    uint8_t buf[0x340];
    memset(buf, 0, SAVE_DATA_SIZE);
    SaveData_SetDefaults((SaveData *)(void *)buf);
    return card_program((uint32_t)idx * SAVE_SLOT_STRIDE + SAVE_SLOT_BASE, buf, SAVE_DATA_SIZE) ? 1 : 0;
}

int SaveData_FormatAll(void) {
    int err = 0;
    uint8_t i;
    for (i = 0; i < 5; i++) {
        err |= SaveData_FormatSlot(i * 2) == 0;
        err |= SaveData_FormatSlot(i * 2 + 1) == 0;
    }
    gSaveState.b2 = 0;
    return err == 0;
}

int SaveData_Format(void) {
    gSaveDataBusy = 1;
    if (gSaveState.flags & 0x80) {
        gSaveDataBusy = 0;
        return 6;
    }
    SaveData_SetDefaults(&gSaveData);
    if (SaveData_FormatAll() == 0 || (!s_in_startup && !save_flush())) {
        gSaveDataBusy = 0;
        return 2;
    }
    gSaveDataBusy = 0;
    return 0;
}

int SaveData_SelectLatestSlot(void) {
    int8_t best;
    uint8_t i;
    uint32_t cnt[5] = { 0, 0, 0, 0, 0 };
    uint8_t st[5] = { 0, 0, 0, 0, 0 };
    uint8_t raw[0x340];
    SaveData *buf = (SaveData *)(void *)raw;
    int bad, zero;
    memset(raw, 0, SAVE_DATA_SIZE);
    for (i = 0; i < 5; i++) {
        bad = 0;
        if (SaveData_ReadSlot(i * 2, buf)) {
            if (SaveData_VerifyChecksum(buf)) {
                cnt[i] = buf->unk4;
                continue;
            }
            bad = 1;
            cnt[i] = buf->unk4;
        }
        if (SaveData_ReadSlot(i * 2 + 1, buf)) {
            if (SaveData_VerifyChecksum(buf)) {
                cnt[i] = buf->unk4;
            } else {
                st[i] = 2;
                cnt[i] = buf->unk4;
            }
        } else if (bad) {
            st[i] = 2;
        } else {
            st[i] = 1;
        }
    }
    best = -1;
    for (i = 0; i < 5; i++) {
        if (st[i] == 1)
            continue;
        if (best < 0)
            best = (int8_t)i;
        else if (cnt[i] > cnt[best])
            best = (int8_t)i;
    }
    if (best < 0)
        return 0;
    zero = 1;
    for (i = 0; i < 5; i++) {
        if (cnt[i] != 0) {
            zero = 0;
            break;
        }
    }
    if (zero)
        best = 0;
    gSaveState.b2 = (uint8_t)best;
    if (st[best] == 2)
        return 0;
    return (cnt[best] - (uint32_t)best) % 5 == 0;
}

/* ------------------------------------------------------- device */
int SaveData_IdentifyBackup(void) {
    /* CARD_IdentifyBackup(0xd01): the device is there. Read its image (a missing
     * file reads as an erased backup). */
    int r;
    if (!g_save_backend.present())
        return 0;
    memset(g_save_image, 0, sizeof(g_save_image));
    r = g_save_backend.read(g_save_image, SAVE_IMAGE_SIZE);
    if (r < 0)
        return 0;
    if (r > 0)
        memset(g_save_image, 0xff, sizeof(g_save_image));   /* erased flash */
    s_image_dirty = 0;
    return 1;
}

int SaveData_CheckSignature(void) {
    uint8_t buf[12] = { 0 };
    if (!card_read(0, buf, 12))
        return 0;
    return s_have_sig && memcmp(buf, gSaveSignature, 12) == 0;
}

int SaveData_WriteSignature(void) {
    return card_program(0, gSaveSignature, 12) != 0;
}

int SaveData_Startup(void) {
    int r;
    gSaveDataBusy = 1;
    if (SaveData_Init() == 0) {
        gSaveDataBusy = 0;
        return 5;
    }
    if (SaveData_IdentifyBackup() == 0)
        return 1;
    gSaveState.flags &= ~0x80;
    if (SaveData_CheckSignature() == 0) {
        if (s_card_result) {
            gSaveDataBusy = 0;
            return 1;
        }
        s_in_startup = 1;
        r = SaveData_Format();
        s_in_startup = 0;
        if (r) {
            gSaveDataBusy = 0;
            return 1;
        }
        SaveData_WriteSignature();
        save_flush();
        if (s_card_result) {
            gSaveDataBusy = 0;
            return 1;
        }
        gSaveDataBusy = 0;
        return 4;
    }
    if (SaveData_SelectLatestSlot() == 0) {
        gSaveDataBusy = 0;
        return 3;
    }
    gSaveDataBusy = 0;
    return 0;
}

int SaveData_IsEnabled(void) { return 1; }

/* SaveMsg_Request(1|2|3, ..): message box + halt (1, 2) or "format?" (3). The
 * DC keeps playing: errors 1/2 disable saving (flags 0x80), 3 is reported and
 * the defaults stay loaded (the original reformats after the user confirms). */
static void save_show_error(int mode) {
    s_last_error = mode;
    if (mode == 1 || mode == 2)
        gSaveState.flags |= 0x80;
}

int SaveData_StartupOrShowError(void (*cb)(void)) {
    (void)cb;
    switch (SaveData_Startup()) {
    case 0:
    case 4:
        return 1;
    case 3:
        save_show_error(3);
        return 0;
    case 6:
    case 7:
        return 1;
    default:
        save_show_error(1);
        return 0;
    }
}

int SaveData_LoadOrShowError(void (*cb)(void)) {
    int r;
    (void)cb;
    if (SaveData_IsEnabled() == 0)
        return 1;
    r = SaveData_Load();
    if (r == 0)
        return 1;
    save_show_error(r == 3 ? 3 : 1);
    return 0;
}

int SaveData_CommitOrShowError(void (*cb)(void)) {
    int r;
    if (SaveData_IsEnabled() == 0) {
        if (cb)
            cb();
        return 1;
    }
    r = SaveData_Commit();
    if (r != 0) {
        save_show_error(2);
        return 0;
    }
    if (cb)
        cb();
    return 1;
}

int SaveData_FormatOrShowError(void) {
    if (!SaveData_Format())
        return 1;
    save_show_error(2);
    return 0;
}

/* --------------------------------------------------------- accessors */
uint32_t SaveData_GetProgressFlags(void) { return gSaveData.unk8; }
void SaveData_SetProgressFlags(uint32_t v) { gSaveData.unk8 |= v; }
uint8_t SaveData_GetSonicEmeralds(void) { return gSaveData.unkF; }
void SaveData_SetSonicProgress(uint16_t v) { gSaveData.unkC = v; }
void SaveData_SetBlazeProgress(uint16_t v) { gSaveData.unk64 = v; }
void SaveData_SetDifficulty(int on) {
    if (on)
        gSaveData.unk8 &= ~1u;
    else
        gSaveData.unk8 |= 1;
}
void SaveData_SetTimeLimit(int on) {
    if (on)
        gSaveData.unk8 &= ~2u;
    else
        gSaveData.unk8 |= 2;
}
void SaveData_SetGameCleared(void) { gSaveData.unk8 |= 0x1000; }
void SaveData_UnlockPlayerSelectPage(void) { gSaveData.unk8 |= 0x100; }
void SaveData_UnlockSoundTest(void) { gSaveData.unk8 |= 0x200; }

void SaveData_SetLives(int which, uint32_t v, int clamp) {
    if (clamp && v <= 2)
        v = 2;
    switch (which) {
    case 0:
        gSaveData.unkE = (uint8_t)v;
        break;
    case 1:
        gSaveData.unk66 = (uint8_t)v;
        break;
    }
}

void SaveData_SetLivesBoth(int a, int b) {
    if (a < 2)
        a = 2;
    else if (a > 99)
        a = 99;
    if (b < 2)
        b = 2;
    else if (b > 99)
        b = 99;
    gSaveData.unkE = (uint8_t)a;
    gSaveData.unk66 = (uint8_t)b;
}

/* gGameState.zone/act (cur_zone/cur_act) pick the zone 7/8 tables */
static int special_stage(int cur_zone, int cur_act) { return cur_zone >= 7 && cur_zone < 9 && cur_act < 2; }

int SaveData_GetTimeRank(int zone, int act, uint32_t score, int cur_zone, int cur_act) {
    const uint16_t *p;
    int i;
    if (score > 35999)
        score = 35999;
    if (special_stage(cur_zone, cur_act))
        p = gSaveData.unk1A8[zone - 7][act];
    else
        p = gSaveData.unkD2[zone][act];
    for (i = 0; i < 5; i++) {
        if (score < *p)
            return i;
        p++;
    }
    return i;
}

void SaveData_InsertTimeRecord(int stage, int act, uint32_t time, uint32_t flag, int cur_zone, int cur_act) {
    int bit, n, rank;
    uint8_t *flags;
    uint16_t *p, *times;
    if (time > 35999)
        time = 35999;
    rank = SaveData_GetTimeRank(stage, act, time, cur_zone, cur_act);
    if (rank >= 5)
        return;
    n = 4 - rank;
    if (special_stage(cur_zone, cur_act)) {
        int k = stage - 7;
        flags = &gSaveData.unk1A4[k][act];
        times = gSaveData.unk1A8[k][act];
    } else {
        flags = &gSaveData.unkBC[stage][act];
        times = gSaveData.unkD2[stage][act];
    }
    p = &times[4];   /* data_022c475a / data_022c4830 */
    bit = 4;
    for (; n > 0; n--) {
        *flags &= ~(1 << bit);
        *flags |= (*flags & (1 << (bit - 1))) << 1;
        bit--;
        *p = p[-1];
        p--;
    }
    times[rank] = (uint16_t)time;
    *flags &= ~(1 << rank);
    *flags |= (uint8_t)((flag & 1) << rank);
}

/* gGameState.unk0 = character, data_022c4590.unk14 = lives, gGameState.unk28 = emeralds */
void SaveData_StoreLives(int character, uint8_t lives, uint16_t emeralds) {
    if (character != 1) {
        gSaveData.unkE = lives;
        gSaveData.unkF = (uint8_t)emeralds;
    } else {
        gSaveData.unk66 = lives;
    }
}

static uint8_t zone_index(int character, uint8_t v) {
    /* StoryProgress_GetZoneIndex */
    const uint8_t *tbl = g_save_zone_order[character == 1];
    uint8_t i;
    for (i = 0; i < 9; i++)
        if (v == tbl[i])
            return i;
    return 0xff;
}

void SaveData_StoreStageResult(int character, int zone, int act, uint8_t lives, uint16_t emeralds, uint32_t score) {
    uint32_t z = zone_index(character, (uint8_t)zone);
    uint8_t a = (uint8_t)act;   /* StoryProgress_GetActIndex: identity */
    if (character != 1) {
        gSaveData.unkE = lives;
        gSaveData.unkF = emeralds & 0x7f;
        if (zone < 7) {
            uint32_t *row = gSaveData.unk10[z];
            uint32_t v = row[a], t = score;
            if (t >= v) {
                if (t > 999999)
                    t = 999999;
                v = t;
            }
            row[a] = v;
        }
    } else {
        gSaveData.unk66 = lives;
        if (zone < 7) {
            uint32_t *row = gSaveData.unk68[z];
            uint32_t v = row[a], t = score;
            if (t >= v) {
                if (t > 999999)
                    t = 999999;
                v = t;
            }
            row[a] = v;
        }
    }
    SaveData_CommitOrShowError(NULL);
}

void SaveData_RecordVsResult(int kind, const uint16_t *name, uint8_t len, const uint8_t *mac) {
    int i, j;
    SaveVsEntry *p, e;
    if (len > 10)
        len = 10;
    p = gSaveData.unk1D4;
    for (i = 0; i < 10; i++, p++) {
        if (len == p->len) {
            for (j = 0; j < len; j++)
                if (name[j] != p->name[j])
                    break;
            if (j >= len) {
                for (j = 0; j < 6; j++)
                    if (mac[j] != p->mac[j])
                        break;
                if (j >= 6)
                    break;
            }
        }
    }
    if (i < 10) {
        e = gSaveData.unk1D4[i];
    } else {
        memset(&e, 0, sizeof(e));
        memcpy(e.name, name, (size_t)len * 2);
        e.len = len;
        memcpy(e.mac, mac, 6);
    }
    for (j = i - 1; j >= 0; j--)
        if (j < 9)
            gSaveData.unk1D4[j + 1] = gSaveData.unk1D4[j];
    gSaveData.unk1D4[0] = e;
    if (kind >= 0 && kind < 3) {
        if (gSaveData.unk1D0[kind] < 99)
            gSaveData.unk1D0[kind]++;
        if (gSaveData.unk1D4[0].cnt[kind] < 99)
            gSaveData.unk1D4[0].cnt[kind]++;
    }
}

/* ------------------------------------------------------------- port API */
/* Game_StartupSaveDataOnBoot (SaveData_StartupOrShowError) + the first
 * SaveData_LoadOrShowError of the title menu. */
int save_load(void) {
    int r;
    s_last_error = 0;
    gSaveState.flags = 0;
    SaveData_SetDefaults(&gSaveData);
    r = SaveData_Startup();
    switch (r) {
    case 0:
    case 4:
        if (SaveData_Load() != 0)
            save_show_error(3);
        break;
    case 3:
        save_show_error(3);
        SaveData_SetDefaults(&gSaveData);
        gSaveState.flags |= 1;
        break;
    default:
        /* no VMU / unusable: play with the defaults, no saving */
        gSaveDataBusy = 0;
        gSaveState.flags |= 0x80;
        SaveData_SetDefaults(&gSaveData);
        if (r == 1)
            r = 6;
        break;
    }
    return r;
}

int save_write(void) {
    int r = SaveData_Commit();
    if (r != 0 && r != 6)
        s_last_error = 2;
    return r;
}
