"""Python reference of the DS stage map-collision queries.

Direct transcription of the decompiled functions in
src/game/Stage/ObjectSystem/ (MapCollision_GetTileAt, _GetTileHeight,
_GetTileWidth, _ProbeDistance, _ProbeDistanceLong).  It works on the *raw*
DS map-archive files (.df/.di/.at/.bk/_a.mp/_b.mp) so it can be used to check
the converted Dreamcast package (level_format.h reference code) against an
independent path.

Coordinates are integer pixels.  Return values follow the DS code exactly,
including the s8 truncation the callers apply (`(s8)Probe(...)`).
"""
import struct


def s8(v):
    v &= 0xFF
    return v - 0x100 if v & 0x80 else v


class DSMap:
    def __init__(self, files):
        """files: dict ext->bytes with keys at, bk, df, di, a, b (decompressed)."""
        self.df = files['df']
        self.di = files['di']
        self.at = files['at']
        bk = files['bk']
        self.bk = struct.unpack('<%dH' % (len(bk) // 2), bk)
        a = files['a']
        b = files['b']
        self.w, self.h = struct.unpack_from('<HH', a, 0)
        self.layers = [struct.unpack_from('<%dH' % (self.w * self.h), a, 4),
                       struct.unpack_from('<%dH' % (self.w * self.h), b, 4)]
        self.left = 0
        self.top = 0
        self.right = self.w << 6
        self.bottom = self.h << 6
        # rect used by ProbeDistanceLong when flags & 0x20 (data_022c4dcc)
        self.arena = (0, 0, 0, 0)

    # MapCollision_GetTileAt
    def tile_at(self, x, y, layer):
        tx = x >> 3
        ty = y >> 3
        bx = int(tx / 8)  # C division (operands are non-negative in practice)
        by = int(ty / 8)
        blk = self.layers[layer][self.w * by + bx]
        return self.bk[blk * 64 + (tx - bx * 8) + (ty - by * 8) * 8]

    def _hnib(self, cid, i):
        # (s32)((s8)b << 20) >> 24 == signed high nibble of the byte
        h = s8(self.df[cid * 8 + i]) >> 4
        if h & 8:
            h = s8(h | ~0xF)
        if h == -8:
            h = 8
        return h

    def _wnib(self, cid, i):
        h = s8(self.df[cid * 8 + i] & 0xF)
        if h & 8:
            h = s8(h | ~0xF)
        if h == -8:
            h = 8
        return h

    def _edge(self, flags, a, b, lo_a, hi_a, lo_b, hi_b):
        """Shared flag-0x40 / clamp prologue.  a = probed axis, b = other axis.
        Returns (early_result or None, a, b)."""
        if flags & 0x40:
            hi = hi_a - 1
            ta = a & ~7
            if ta > hi or a < lo_a - 7:
                return 8, a, b
            if ta + 8 > hi:
                h = s8(hi & 7)
                if h == 0:
                    h = 8
                return h, a, b
            if ta < lo_a:
                t = (0x10 - (lo_a & 7)) if (lo_a & 7) else 8
                t = s8(t | ~0xF)
                if t == -8:
                    t = 8
                return t, a, b
            if b > hi_b - 1 or b < lo_b:
                return 8, a, b
            return None, a, b
        return None, a, b

    def _clamp(self, x, y):
        x = self.left if x < self.left else (self.right - 1 if x > self.right - 1 else x)
        y = self.top if y < self.top else (self.bottom - 1 if y > self.bottom - 1 else y)
        return x, y

    # MapCollision_GetTileHeight(y, x, flags) -> (h, angle|None, attr|None)
    def tile_height(self, y, x, flags):
        if flags & 0x40:
            r, y, x = self._edge(flags, y, x, self.top, self.bottom, self.left, self.right)
            if r is not None:
                return r, None, None
        else:
            x, y = self._clamp(x, y)
        tile = self.tile_at(x, y, flags & 1)
        cid = tile & 0x3FF
        xi = x & 7
        if tile & 0x400:
            xi = 7 - xi
        h = self._hnib(cid, xi)
        if flags & 0x80 and self.at[cid] & 1:
            h = 0
        if tile & 0x800 and h != 8 and h != 0:
            h = s8(h - 8) if h > 0 else s8(h + 8)
        ang = att = None
        if h != 0:
            a = self.di[cid]
            if tile & 0x400:
                a = (-s8(a)) & 0xFF
            if tile & 0x800:
                a = (-0x80 - s8(a)) & 0xFF
            ang = a
            att = self.at[cid]
        return h, ang, att

    # MapCollision_GetTileWidth(x, y, flags)
    def tile_width(self, x, y, flags):
        if flags & 0x40:
            r, x, y = self._edge(flags, x, y, self.left, self.right, self.top, self.bottom)
            if r is not None:
                return r, None, None
        else:
            x, y = self._clamp(x, y)
        tile = self.tile_at(x, y, flags & 1)
        cid = tile & 0x3FF
        yi = y & 7
        if tile & 0x800:
            yi = 7 - yi
        h = self._wnib(cid, yi)
        if flags & 0x80 and self.at[cid] & 1:
            h = 0
        if tile & 0x400 and h != 8 and h != 0:
            h = s8(h - 8) if h > 0 else s8(h + 8)
        ang = att = None
        if h != 0:
            a = self.di[cid]
            if tile & 0x800:
                a = (-0x80 - s8(a)) & 0xFF
            if tile & 0x400:
                a = (-s8(a)) & 0xFF
            ang = a
            att = self.at[cid]
        return h, ang, att

    # MapCollision_ProbeDistanceLong(x, y, flags, dir, &angle, &attr)
    def probe_long(self, x, y, flags, d, angle_in=0, attr_in=0):
        """Returns (dist, angle_out, attr_out); angle/attr in = caller's
        current values (the function leaves them unchanged on a miss)."""
        cur = [angle_in, attr_in]

        def call(a, b):
            h, an, at = fn(a, b, flags)
            if an is not None:
                cur[0] = an
                cur[1] = at
            return h

        def calc(h, step, sub):
            if h > 0:
                return h - (sub + 1) if step > 0 else 8 - sub
            return -(sub + 1) if step > 0 else h + sub

        if flags & 0x20:
            l, t, r, b = self.arena
            dd = {2: b - y, 3: y - t, 1: x - l, 0: r - x}[d & 3]
            return max(-0x1F, min(0x1F, dd)), cur[0], cur[1]
        sa, sb = angle_in, attr_in
        step = -8 if d & 1 else 8
        off = 0
        if d & 2:
            sub = s8(y & 7)
            x, y = y, x
            fn = self.tile_height
        else:
            fn = self.tile_width
            sub = s8(x & 7)
        h = call(x, y)
        if h == 0:
            off += step
            h = call(x + off, y)
            if h == 0:
                h = call(x + off + step, y)
                if h == 0:
                    cur[0], cur[1] = sa, sb
                    return (8 - sub if step > 0 else sub + 1) + 16, cur[0], cur[1]
                if h == 8:
                    return (-(sub + 1) if step > 0 else sub - 8) + 16, cur[0], cur[1]
                return calc(h, step, sub) + 16, cur[0], cur[1]
            if h == 8:
                return (-(sub + 1) if step > 0 else sub - 8) + 8, cur[0], cur[1]
            return calc(h, step, sub) + 8, cur[0], cur[1]
        elif h == 8:
            sa, sb = cur
            off -= step
            h = call(x + off, y)
            if h == 8:
                sa, sb = cur
                h = call(x + off - step, y)
                if h == 0:
                    cur[0], cur[1] = sa, sb
                    if step > 0:
                        sub = 8 - (sub + 1)
                    return sub - 16, cur[0], cur[1]
                if h == 8:
                    return (-(sub + 1) if step > 0 else sub - 8) - 16, cur[0], cur[1]
                return calc(h, step, sub) - 16, cur[0], cur[1]
            if h == 0:
                cur[0], cur[1] = sa, sb
                if step > 0:
                    sub = 8 - (sub + 1)
                return sub - 8, cur[0], cur[1]
            return calc(h, step, sub) - 8, cur[0], cur[1]
        return calc(h, step, sub), cur[0], cur[1]

    def solid_pixel(self, x, y, layer, flags_extra=0):
        """True if pixel (x,y) is inside the vertical height mask of `layer`.
        Derived view used for overlays: h==8 full, h>0 solid rows h..7,
        h<0 solid rows 0..-h-1."""
        h, _, _ = self.tile_height(y, x, layer | flags_extra)
        r = y & 7
        if h == 8:
            return True
        if h > 0:
            return r >= h
        if h < 0:
            return r < -h
        return False
