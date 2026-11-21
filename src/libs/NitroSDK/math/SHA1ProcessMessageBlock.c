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

typedef struct {
    u32 k[4];
} UnkSha1K;

extern u32 data_0207f024;
extern const UnkSha1K data_0207798c;

#define SHA1CircularShift(bits, word) (((word) << (bits)) | ((word) >> (32 - (bits))))

void SHA1ProcessMessageBlock(UnkSha1Ctx *context)
{
    u32 save0;
    u32 save1;
    const UnkSha1K K = data_0207798c;
    int t;
    u32 temp;
    u32 W[80];
    u32 A, B, C, D, E;
    u8 *blk = context->Message_Block;

    if (data_0207f024 != 0) {
        save0 = *(u32 *)(blk + 0x18);
        save1 = *(u32 *)(blk + 0x38);
        *(u32 *)(blk + 0x18) = 0;
        *(u32 *)(blk + 0x38) = 0;
    }
    for (t = 0; t < 16; t++) {
        W[t] = context->Message_Block[t * 4] << 24;
        W[t] |= context->Message_Block[t * 4 + 1] << 16;
        W[t] |= context->Message_Block[t * 4 + 2] << 8;
        W[t] |= context->Message_Block[t * 4 + 3];
    }
    for (t = 16; t < 80; t++) {
        W[t] = SHA1CircularShift(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
    }
    A = context->Intermediate_Hash[0];
    B = context->Intermediate_Hash[1];
    C = context->Intermediate_Hash[2];
    D = context->Intermediate_Hash[3];
    E = context->Intermediate_Hash[4];
    for (t = 0; t < 20; t++) {
        temp = SHA1CircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K.k[0];
        E = D;
        D = C;
        C = SHA1CircularShift(30, B);
        B = A;
        A = temp;
    }
    for (t = 20; t < 40; t++) {
        temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K.k[1];
        E = D;
        D = C;
        C = SHA1CircularShift(30, B);
        B = A;
        A = temp;
    }
    for (t = 40; t < 60; t++) {
        temp = SHA1CircularShift(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K.k[2];
        E = D;
        D = C;
        C = SHA1CircularShift(30, B);
        B = A;
        A = temp;
    }
    for (t = 60; t < 80; t++) {
        temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K.k[3];
        E = D;
        D = C;
        C = SHA1CircularShift(30, B);
        B = A;
        A = temp;
    }
    context->Intermediate_Hash[0] += A;
    context->Intermediate_Hash[1] += B;
    context->Intermediate_Hash[2] += C;
    context->Intermediate_Hash[3] += D;
    context->Intermediate_Hash[4] += E;
    context->Message_Block_Index = 0;
    if (data_0207f024 != 0) {
        *(u32 *)(blk + 0x18) = save0;
        *(u32 *)(blk + 0x38) = save1;
    }
}
