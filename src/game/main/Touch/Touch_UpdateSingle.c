#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
    u16 touch;
    u16 validity;
} UnkTP_02037a8c;

typedef struct {
    u16 x;
    u16 y;
    u16 lastX;
    u16 lastY;
    u16 startX;
    u16 startY;
    u16 endX;
    u16 endY;
    u32 flags;
} UnkTPState_02037a8c;

extern UnkTPState_02037a8c gTouchState;

void TP_RequestSamplingAsync(void);
BOOL TP_WaitRawResult(UnkTP_02037a8c *raw);
void TP_GetCalibratedPoint(UnkTP_02037a8c *out, const UnkTP_02037a8c *raw);

void Touch_UpdateSingle(void) {
    UnkTPState_02037a8c *st = &gTouchState;
    UnkTP_02037a8c raw;
    UnkTP_02037a8c cal;
    st->lastX = st->x;
    st->lastY = st->y;
    st->flags = (st->flags & ~0x20) | (((st->flags & 0x10) >> 4) << 5);
    do {
        TP_RequestSamplingAsync();
    } while (TP_WaitRawResult(&raw));
    if (raw.touch == 1) {
        if (st->flags & 0x10) {
            st->flags &= ~0x40;
            TP_GetCalibratedPoint(&cal, &raw);
            if (raw.validity != 0) {
                st->flags |= 0x100;
                return;
            }
            st->x = cal.x;
            st->y = cal.y;
            st->flags &= ~0x100;
            return;
        }
        st->flags &= ~0x80;
        if (raw.validity != 0) {
            return;
        }
        st->flags |= 0x10;
        st->flags |= 0x40;
        TP_GetCalibratedPoint(&cal, &raw);
        st->startX = cal.x;
        st->x = st->startX;
        st->startY = cal.y;
        st->y = st->startY;
        st->endX = 0xffff;
        st->endY = 0xffff;
    } else {
        st->flags &= ~0x100;
        if (st->flags & 0x10) {
            st->flags &= ~0x10;
            st->flags &= ~0x40;
            st->flags |= 0x80;
            st->endX = st->x;
            st->endY = st->y;
            st->x = 0xffff;
            st->y = 0xffff;
        } else {
            st->flags &= ~0x80;
            st->x = 0xffff;
            st->y = 0xffff;
        }
    }
}
