#include <nitro/types.h>

typedef struct {
    u8 data[0x68];
} UnkSha1Ctx_0200c8b0;

void func_0200d030(UnkSha1Ctx_0200c8b0 *ctx);
void func_0200ce88(UnkSha1Ctx_0200c8b0 *ctx, const void *data, u32 len);
void func_0200cf74(UnkSha1Ctx_0200c8b0 *ctx, void *digest);

void func_0200c8b0(void *digest, const void *bin, u32 binLen, const u8 *key, s32 keyLen)
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
        func_0200d030(&ctx);
        func_0200ce88(&ctx, key, keyLen);
        func_0200cf74(&ctx, h);
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
    func_0200d030(&ctx);
    func_0200ce88(&ctx, ipad, 64);
    func_0200ce88(&ctx, bin, binLen);
    func_0200cf74(&ctx, h);
    func_0200d030(&ctx);
    func_0200ce88(&ctx, opad, 64);
    func_0200ce88(&ctx, h, 20);
    func_0200cf74(&ctx, digest);
}
