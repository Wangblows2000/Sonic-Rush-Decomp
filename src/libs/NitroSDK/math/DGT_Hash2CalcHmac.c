#include <nitro/types.h>

typedef struct {
    u8 data[0x68];
} UnkSha1Ctx_0200c8b0;

void DGT_Hash2Reset(UnkSha1Ctx_0200c8b0 *ctx);
void DGT_Hash2SetSource(UnkSha1Ctx_0200c8b0 *ctx, const void *data, u32 len);
void DGT_Hash2GetDigest(UnkSha1Ctx_0200c8b0 *ctx, void *digest);

void DGT_Hash2CalcHmac(void *digest, const void *bin, u32 binLen, const u8 *key, s32 keyLen)
{
    u8 ipad[64];
    u8 opad[64];
    u8 hash[20];
    UnkSha1Ctx_0200c8b0 ctx;
    s32 i;
    u8 *h = hash;

    if (digest == NULL || bin == NULL || binLen == 0 || key == NULL || keyLen == 0) {
        return;
    }
    if (keyLen > 64) {
        DGT_Hash2Reset(&ctx);
        DGT_Hash2SetSource(&ctx, key, keyLen);
        DGT_Hash2GetDigest(&ctx, h);
        key = h;
        keyLen = 20;
    }
    for (i = 0; i < keyLen; i++) {
        ipad[i] = *key ^ 0x36;
        opad[i] = *key++ ^ 0x5c;
    }
    for (; i < 64; i++) {
        ipad[i] = 0x36;
        opad[i] = 0x5c;
    }
    DGT_Hash2Reset(&ctx);
    DGT_Hash2SetSource(&ctx, ipad, 64);
    DGT_Hash2SetSource(&ctx, bin, binLen);
    DGT_Hash2GetDigest(&ctx, h);
    DGT_Hash2Reset(&ctx);
    DGT_Hash2SetSource(&ctx, opad, 64);
    DGT_Hash2SetSource(&ctx, h, 20);
    DGT_Hash2GetDigest(&ctx, digest);
}
