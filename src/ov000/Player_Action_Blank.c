#include <nitro/types.h>

typedef struct UnkStruct_02347c44 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02347c44;

void Player_InitState(UnkStruct_02347c44 *s);
void Player_ResetGimmickState(UnkStruct_02347c44 *s);
void Player_State_Blank(void);

void Player_Action_Blank(UnkStruct_02347c44 *s) {
    Player_InitState(s);
    Player_ResetGimmickState(s);
    s->func = Player_State_Blank;
}
