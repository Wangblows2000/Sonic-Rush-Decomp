#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 favoriteColor : 4;
    u8 : 4;
    u8 month;           /* 0x03 */
    u8 day;             /* 0x04 */
    u8 pad5;
    u16 nickname[10];   /* 0x06 */
    u8 nicknameLength;  /* 0x1a */
    u8 pad1b;
    u16 comment[26];    /* 0x1c */
    u8 commentLength;   /* 0x50 */
    u8 pad51[0x13];
    u16 language : 3;   /* 0x64 */
    u16 : 13;
} UnkNvram_02007828;

typedef struct {
    u8 language;
    u8 favoriteColor;
    u8 month;
    u8 day;
    u16 nickName[10];   /* 0x04 */
    u16 nickNameLength; /* 0x18 */
    u16 comment[26];    /* 0x1a */
    u16 commentLength;  /* 0x4e */
} UnkOwnerInfo_02007828;

void MIi_CpuCopy16(const void *src, void *dest, u32 size);

void OS_GetOwnerInfo(UnkOwnerInfo_02007828 *info)
{
    UnkNvram_02007828 *src = (UnkNvram_02007828 *)0x027ffc80;

    info->language = (u8)src->language;
    info->favoriteColor = (u8)src->favoriteColor;
    info->month = src->month;
    info->day = src->day;
    info->nickNameLength = (u16)src->nicknameLength;
    info->commentLength = (u16)src->commentLength;
    MIi_CpuCopy16(src->nickname, info->nickName, 20);
    MIi_CpuCopy16(src->comment, info->comment, 52);
}
