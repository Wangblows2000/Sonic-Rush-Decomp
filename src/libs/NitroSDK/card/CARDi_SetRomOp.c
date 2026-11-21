#include <nitro/types.h>

#define REG_CARD_MASTERCNT (*(vu8 *)0x040001a1)
#define REG_CARDCNT (*(vu32 *)0x040001a4)
#define REG_CARD_CMD0 (*(vu8 *)0x040001a8)
#define REG_CARD_CMD1 (*(vu8 *)0x040001a9)
#define REG_CARD_CMD2 (*(vu8 *)0x040001aa)
#define REG_CARD_CMD3 (*(vu8 *)0x040001ab)
#define REG_CARD_CMD4 (*(vu8 *)0x040001ac)
#define REG_CARD_CMD5 (*(vu8 *)0x040001ad)
#define REG_CARD_CMD6 (*(vu8 *)0x040001ae)
#define REG_CARD_CMD7 (*(vu8 *)0x040001af)

void CARDi_SetRomOp(u32 cmd1, u32 cmd2)
{
    while (REG_CARDCNT & 0x80000000) {
    }
    REG_CARD_MASTERCNT = 0xc0;
    REG_CARD_CMD0 = (u8)(cmd1 >> 24);
    REG_CARD_CMD1 = (u8)(cmd1 >> 16);
    REG_CARD_CMD2 = (u8)(cmd1 >> 8);
    REG_CARD_CMD3 = (u8)(cmd1 >> 0);
    REG_CARD_CMD4 = (u8)(cmd2 >> 24);
    REG_CARD_CMD5 = (u8)(cmd2 >> 16);
    REG_CARD_CMD6 = (u8)(cmd2 >> 8);
    REG_CARD_CMD7 = (u8)(cmd2 >> 0);
}
