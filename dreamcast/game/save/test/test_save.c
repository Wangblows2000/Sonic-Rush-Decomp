/* Host test of the save module: serialisation, CRC, slot rotation, recovery,
 * time records, VS records. A file in /tmp stands in for the VMU.
 * Usage: test_save <menudata.bin or -> [tmpdir] */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../save.h"

static int fails, checks;
#define CHECK(c)                                                         \
    do {                                                                 \
        checks++;                                                        \
        if (!(c)) {                                                      \
            fails++;                                                     \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #c);          \
        }                                                                \
    } while (0)

static char s_path[256];
static int s_present = 1, s_fail_write, s_writes;
static int h_present(void) { return s_present; }
static int h_read(uint8_t *img, int size) {
    FILE *f = fopen(s_path, "rb");
    int n;
    if (!f)
        return 1;
    n = (int)fread(img, 1, (size_t)size, f);
    fclose(f);
    return n == size ? 0 : 1;
}
static int h_write(const uint8_t *img, int size) {
    FILE *f;
    if (s_fail_write)
        return -1;
    f = fopen(s_path, "wb");
    if (!f)
        return -1;
    fwrite(img, 1, (size_t)size, f);
    fclose(f);
    s_writes++;
    return 0;
}

/* signature from menudata.bin (section "save_sig"), else a test value */
static void load_sig(const char *path) {
    uint8_t sig[12] = "test_sig___";
    FILE *f = path && strcmp(path, "-") ? fopen(path, "rb") : NULL;
    if (f) {
        static uint8_t buf[1 << 20];
        size_t n = fread(buf, 1, sizeof(buf), f);
        unsigned i, cnt = buf[6] | buf[7] << 8;
        fclose(f);
        for (i = 0; i < cnt && 8 + 20 * i + 20 <= n; i++) {
            const uint8_t *d = buf + 8 + 20 * i;
            if (!strncmp((const char *)d, "save_sig", 12)) {
                unsigned off = d[12] | d[13] << 8 | d[14] << 16 | (unsigned)d[15] << 24;
                memcpy(sig, buf + off, 12);
                printf("signature from menudata.bin: \"%s\"\n", (const char *)sig);
            }
        }
    }
    save_set_signature(sig);
}

/* standard CRC-32 (zlib) bit by bit, as an independent reference */
static uint32_t crc_ref(const uint8_t *p, int n) {
    uint32_t c = 0xffffffffu;
    int i, k;
    for (i = 0; i < n; i++) {
        c ^= p[i];
        for (k = 0; k < 8; k++)
            c = (c >> 1) ^ (0xedb88320u & (0u - (c & 1)));
    }
    return ~c;
}

int main(int argc, char **argv) {
    uint32_t c;
    int r, i;
    snprintf(s_path, sizeof(s_path), "%s/srdc_save_test.bin", argc > 2 ? argv[2] : "/tmp");
    remove(s_path);
    g_save_backend.present = h_present;
    g_save_backend.read = h_read;
    g_save_backend.write = h_write;
    load_sig(argc > 1 ? argv[1] : NULL);

    /* 1. CRC = CRC-32 of bytes 4..0x314 */
    SaveData_SetDefaults(&gSaveData);
    c = crc_ref((const uint8_t *)&gSaveData + 4, 0x310);
    CHECK(gSaveData.unk0 == c);
    CHECK(SaveData_VerifyChecksum(&gSaveData));
    CHECK(gSaveData.unkE == 2 && gSaveData.unk66 == 2);
    CHECK(gSaveData.unkD2[0][0][0] == 0xffff && gSaveData.unkD2[6][2][4] == 0xffff);
    CHECK(gSaveData.unk1A8[1][1][4] == 0xffff && gSaveData.unk1D0[0] == 0);

    /* 2. fresh device: Startup formats (code 4), writes one VMU file */
    s_writes = 0;
    r = save_load();
    CHECK(r == SAVE_NEW);
    CHECK(s_writes == 1);
    CHECK(gSaveState.b2 == 0 && (gSaveState.flags & 0x80) == 0);
    CHECK(memcmp(g_save_image + 12, &gSaveData, 0) == 0);
    CHECK(SaveData_VerifyChecksum(g_save_image + 12 + 9 * 0x320));

    /* 3. nothing changed -> commit writes nothing */
    s_writes = 0;
    CHECK(save_write() == 0 && s_writes == 0);

    /* 4. progress -> commit writes slot pair 1 with counter 1 */
    SaveData_SetSonicProgress(5);
    gSaveData.unkE = 7;
    SaveData_UnlockSoundTest();
    r = save_write();
    CHECK(r == 0 && s_writes == 1);
    CHECK(gSaveState.b2 == 1 && gSaveData.unk4 == 1);
    {
        SaveData a, b;
        memcpy(&a, g_save_image + 12 + 2 * 0x320, sizeof(a));
        memcpy(&b, g_save_image + 12 + 3 * 0x320, sizeof(b));
        CHECK(SaveData_VerifyChecksum(&a) && memcmp(&a, &b, sizeof(a)) == 0);
        CHECK(a.unkC == 5 && a.unkE == 7 && (a.unk8 & 0x200));
    }

    /* 5. reboot: SelectLatestSlot finds pair 1, data comes back */
    memset(&gSaveData, 0, sizeof(gSaveData));
    gSaveState.b2 = 0;
    r = save_load();
    CHECK(r == SAVE_OK);
    CHECK(gSaveState.b2 == 1 && gSaveData.unkC == 5 && gSaveData.unkE == 7 && gSaveData.unk4 == 1);

    /* 6. several commits rotate the pairs 2,3,4,0 (counter % 5 == pair) */
    for (i = 0; i < 4; i++) {
        gSaveData.unk10[0][0] = 1000 * (i + 1);
        CHECK(save_write() == 0);
        CHECK(gSaveState.b2 == (2 + i) % 5 && gSaveData.unk4 % 5 == gSaveState.b2);
    }
    memset(&gSaveData, 0, sizeof(gSaveData));
    r = save_load();
    CHECK(r == SAVE_OK && gSaveState.b2 == 0 && gSaveData.unk10[0][0] == 4000 && gSaveData.unk4 == 5);

    /* 7. first copy of the newest pair damaged -> second copy used */
    {
        FILE *f = fopen(s_path, "r+b");
        fseek(f, 12 + 0 * 0x320 + 0x20, SEEK_SET);
        fputc(0x5a, f);
        fclose(f);
        memset(&gSaveData, 0, sizeof(gSaveData));
        r = save_load();
        CHECK(r == SAVE_OK && gSaveData.unk10[0][0] == 4000);
    }
    /* 8. both copies damaged -> Startup code 3 (corrupt), defaults loaded */
    {
        FILE *f = fopen(s_path, "r+b");
        fseek(f, 12 + 1 * 0x320 + 0x20, SEEK_SET);
        fputc(0x5a, f);
        fclose(f);
        r = save_load();
        CHECK(r == SAVE_ERR_CORRUPT && save_last_error() == 3);
        CHECK(gSaveData.unkE == 2 && gSaveData.unk10[0][0] == 0);
    }

    /* 9. Validate clamps */
    SaveData_SetDefaults(&gSaveData);
    gSaveData.unkE = 120;
    gSaveData.unkC = 40;
    gSaveData.unk64 = 40;
    gSaveData.unkF = 0x85;
    gSaveData.unk10[3][1] = 5000000;
    gSaveData.unkD2[2][1][3] = 40000;
    gSaveData.unk1D4[4].cnt[2] = 200;
    CHECK(SaveData_Validate() == 0);
    CHECK(gSaveData.unkE == 99 && gSaveData.unkC == 26 && gSaveData.unk64 == 28 && gSaveData.unkF == 5);
    CHECK(gSaveData.unk10[3][1] == 999999 && gSaveData.unkD2[2][1][3] == 36000 && gSaveData.unk1D4[4].cnt[2] == 99);
    CHECK(SaveData_Validate() == 1);

    /* 10. time records: sorted insert, Blaze flag bits move with the entries */
    SaveData_SetDefaults(&gSaveData);
    SaveData_InsertTimeRecord(1, 0, 3000, 0, 1, 0);
    SaveData_InsertTimeRecord(1, 0, 2000, 1, 1, 0);
    SaveData_InsertTimeRecord(1, 0, 2500, 0, 1, 0);
    SaveData_InsertTimeRecord(1, 0, 99999, 0, 1, 0);   /* clamped to 35999 */
    CHECK(gSaveData.unkD2[1][0][0] == 2000 && gSaveData.unkD2[1][0][1] == 2500 && gSaveData.unkD2[1][0][2] == 3000);
    CHECK(gSaveData.unkD2[1][0][3] == 35999 && gSaveData.unkD2[1][0][4] == 0xffff);
    CHECK(gSaveData.unkBC[1][0] == 0x01);
    CHECK(SaveData_GetTimeRank(1, 0, 2100, 1, 0) == 1 && SaveData_GetTimeRank(1, 0, 1, 1, 0) == 0);
    SaveData_InsertTimeRecord(7, 1, 1234, 1, 7, 1);    /* zone 7/8 tables */
    CHECK(gSaveData.unk1A8[0][1][0] == 1234 && gSaveData.unk1A4[0][1] == 1);

    /* 11. stage result keeps the best score, clamps at 999999 */
    s_fail_write = 1;   /* commit fails -> save error 2, saving disabled */
    gSaveState.flags = 1;
    SaveData_StoreStageResult(0, 2, 1, 9, 0x83, 50000);
    SaveData_StoreStageResult(0, 2, 1, 8, 0x03, 40000);
    CHECK(gSaveData.unk10[2][1] == 50000 && gSaveData.unkE == 8 && gSaveData.unkF == 3);
    SaveData_StoreStageResult(1, 2, 0, 4, 0, 2000000);
    CHECK(gSaveData.unk68[2][0] == 999999 && gSaveData.unk66 == 4);
    CHECK(save_last_error() == 2 && (gSaveState.flags & 0x80));
    s_fail_write = 0;

    /* 12. VS records: move to front, counters */
    {
        uint16_t n1[3] = { 'A', 'B', 'C' }, n2[2] = { 'X', 'Y' };
        uint8_t m1[6] = { 1, 2, 3, 4, 5, 6 }, m2[6] = { 9, 9, 9, 9, 9, 9 };
        SaveData_SetDefaults(&gSaveData);
        SaveData_RecordVsResult(0, n1, 3, m1);
        SaveData_RecordVsResult(1, n2, 2, m2);
        SaveData_RecordVsResult(0, n1, 3, m1);
        CHECK(gSaveData.unk1D4[0].len == 3 && gSaveData.unk1D4[0].name[2] == 'C' && gSaveData.unk1D4[0].cnt[0] == 2);
        CHECK(gSaveData.unk1D4[1].len == 2 && gSaveData.unk1D4[1].cnt[1] == 1);
        CHECK(gSaveData.unk1D0[0] == 2 && gSaveData.unk1D0[1] == 1);
    }

    /* 13. lives helpers */
    SaveData_SetLivesBoth(1, 150);
    CHECK(gSaveData.unkE == 2 && gSaveData.unk66 == 99);
    SaveData_SetLives(1, 1, 1);
    CHECK(gSaveData.unk66 == 2);
    SaveData_SetDifficulty(0);
    SaveData_SetTimeLimit(0);
    CHECK((SaveData_GetProgressFlags() & 3) == 3);

    /* 14. no VMU: play with defaults, saving disabled */
    s_present = 0;
    r = save_load();
    CHECK(r == SAVE_DISABLED && (gSaveState.flags & 0x80) && gSaveData.unkE == 2);
    CHECK(save_write() == 6);

    printf("save host test: %d/%d checks passed\n", checks - fails, checks);
    remove(s_path);
    return fails ? 1 : 0;
}
