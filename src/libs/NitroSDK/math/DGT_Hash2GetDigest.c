#include <nitro/types.h>

typedef struct {
    u32 Intermediate_Hash[5]; /* 0x00 */
    u32 Length_Low;           /* 0x14 */
    u32 Length_High;          /* 0x18 */
    s32 Message_Block_Index;  /* 0x1c */
    u8 Message_Block[64];     /* 0x20 */
    s32 Computed;             /* 0x60 */
    s32 Corrupted;            /* 0x64 */
} UnkSha1Ctx;

void SHA1PadMessage(UnkSha1Ctx *ctx);

s32 DGT_Hash2GetDigest(UnkSha1Ctx *context, u8 *Message_Digest)
{
    s32 i;

    if (!context || !Message_Digest) {
        return 1;
    }
    if (context->Corrupted) {
        return context->Corrupted;
    }
    if (!context->Computed) {
        SHA1PadMessage(context);
        for (i = 0; i < 64; ++i) {
            context->Message_Block[i] = 0;
        }
        context->Length_Low = 0;
        context->Length_High = 0;
        context->Computed = 1;
    }
    for (i = 0; i < 20; ++i) {
        Message_Digest[i] = context->Intermediate_Hash[i >> 2] >> 8 * (3 - (i & 0x03));
    }
    return 0;
}
