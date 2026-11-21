#include <nitro/types.h>
#include "main_a2/mic.h"

u32 MIC_GetLastSamplingAddress(void);
void DC_InvalidateRange(u32 addr, u32 size);

void Mic_Update(void) {
    UnkMic *m = &gMic;
    if (!(m->flags & 1)) {
        return;
    }
    m->prev = m->cur;
    m->cur = MIC_GetLastSamplingAddress();
    if (m->cur == 0) {
        m->flags |= 2;
        return;
    }
    m->flags &= ~2;
    if (m->prev == 0) {
        m->prev = m->buf;
    }
    if (m->prev < m->cur) {
        DC_InvalidateRange(m->prev, m->cur - m->prev);
        m->flags &= ~4;
        m->count = (m->flags & 8) ? (u16)((m->cur - m->prev) >> 1) : (u16)(m->cur - m->prev);
    } else if (m->prev > m->cur) {
        DC_InvalidateRange(m->prev, m->buf + m->size - m->prev);
        DC_InvalidateRange(m->buf, m->cur - m->buf);
        m->flags |= 4;
        m->count = (m->flags & 8) ? (u16)((m->cur + m->size - m->prev) >> 1) : (u16)(m->cur + m->size - m->prev);
    }
}
