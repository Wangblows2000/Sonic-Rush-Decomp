#include <nitro/types.h>

typedef struct {
    u32 Intermediate_Hash[5]; /* 0x00 */
    u32 Length_Low;           /* 0x14 */
    u32 Length_High;          /* 0x18 */
    s32 Message_Block_Index;  /* 0x1c */
    u8 Message_Block[64];     /* 0x20 */
    s32 Computed;             /* 0x60 */
    s32 Corrupted;            /* 0x64 */
} UnkSha1Ctx_0200ce88;

void func_0200cb88(UnkSha1Ctx_0200ce88 *ctx);

s32 func_0200ce88(UnkSha1Ctx_0200ce88 *context, const u8 *message_array, u32 length)
{
    if (!length) {
        return 0;
    }
    if (!context || !message_array) {
        return 1;
    }
    if (context->Computed) {
        context->Corrupted = 3;
        return 3;
    }
    if (context->Corrupted) {
        return context->Corrupted;
    }
    while (length-- && !context->Corrupted) {
        context->Message_Block[context->Message_Block_Index++] = (*message_array & 0xff);
        context->Length_Low += 8;
        if (context->Length_Low == 0) {
            context->Length_High++;
            if (context->Length_High == 0) {
                context->Corrupted = 1;
            }
        }
        if (context->Message_Block_Index == 64) {
            func_0200cb88(context);
        }
        message_array++;
    }
    return 0;
}
