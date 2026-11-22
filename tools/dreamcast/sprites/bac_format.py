#!/usr/bin/env python3
"""Parser and renderer for Sonic Rush ".bac" sprite-animation files.

Clean-room reading of the decompiled engine code (see docs/dreamcast/formats/SPRITES.md):
  AnimatorSprite_Init / _SetAnimation / _ProcessAnimation / _Draw, AnimatorSprite_AdvanceFrames,
  BAC_FrameGroupFunc_* (command handlers, table gBACFrameGroupFuncTable @ 0x02072e00),
  Model_DrawSprite3D (texture-format animations), VRAMTransfer_Flush / PaletteTransfer_Flush.

Importable:  from bac_format import Bac
    bac = Bac(data)             # raises BacError on malformed input
    bac.animations[i].frames    # list of Frame (timeline, durations, hitboxes, assembly)
    bac.render_frame(frame)     # -> (w, h, rgba bytes, origin_x, origin_y) or None
"""
import struct

from nds_archive import decompress, DecompressError

# OAM shape/size tables (ARM9 data_02072da0 = {w,h}, data_02072d60 = 8x8 tile count), idx = shape<<2 | size
OBJ_SIZE = [(8, 8), (16, 16), (32, 32), (64, 64),
            (16, 8), (32, 8), (32, 16), (64, 32),
            (8, 16), (8, 32), (16, 32), (32, 64)]
OBJ_TILES = [1, 4, 16, 64, 2, 4, 8, 32, 2, 4, 8, 32]
# Animation "format" (anim table u16) -> log2(bytes per 8x8 unit), ARM9 data_02072e64
FMT_UNIT_SHIFT = [5, 6, 7, 4, 5, 6, 7, 6, 6, 4]
# Animation format -> GX texture format used by Model_DrawSprite3D (data_02072d78)
FMT_GX_TEX = [3, 4, 7, 2, 3, 4, 7, 1, 6, 5]
FMT_NAMES = {0: "obj4bpp", 1: "obj8bpp", 2: "objbitmap", 3: "tex_pal4", 4: "tex_pal16",
             5: "tex_pal256", 6: "tex_direct", 7: "tex_a3i5", 8: "tex_a5i3", 9: "tex_4x4"}

CMD_END_FRAME = 0        # BAC_FrameGroupFunc_EndFrame
CMD_FRAME_ASSEMBLY = 1   # BAC_FrameGroupFunc_FrameAssembly
CMD_SPRITE_PARTS = 2     # BAC_FrameGroupFunc_SpriteParts
CMD_PALETTE = 3          # BAC_FrameGroupFunc_Palette
CMD_END_ANIMATION = 4    # BAC_FrameGroupFunc_EndAnimation (jump/loop)
CMD_CALLBACK_6, CMD_CALLBACK_7, CMD_CALLBACK_8 = 6, 7, 8
CMD_CHANGE_ANIMATION = 9  # BAC_FrameGroupFunc_ChangeAnimation
CMD_NAMES = {0: "EndFrame", 1: "FrameAssembly", 2: "SpriteParts", 3: "Palette", 4: "EndAnimation",
             5: "Skip5", 6: "Callback6", 7: "Callback7", 8: "Callback8", 9: "ChangeAnimation",
             10: "Skip10", 11: "Skip11"}


class BacError(ValueError):
    pass


def bgr555_to_rgba(c, transparent=False):
    r = (c & 0x1F) << 3
    g = ((c >> 5) & 0x1F) << 3
    b = ((c >> 10) & 0x1F) << 3
    return (r | r >> 5, g | g >> 5, b | b >> 5, 0 if transparent else 255)


class Piece:
    __slots__ = ("attr0", "attr1", "attr2", "attr3")

    def __init__(self, a0, a1, a2, a3):
        self.attr0, self.attr1, self.attr2, self.attr3 = a0, a1, a2, a3

    @property
    def size_index(self):
        return ((self.attr0 >> 14) << 2) | (self.attr1 >> 14)

    @property
    def size(self):
        return OBJ_SIZE[self.size_index]

    @property
    def y(self):      # 8-bit OAM Y as stored (the engine adds it unsigned, hardware wraps mod 256)
        return self.attr0 & 0xFF

    @property
    def x(self):      # 9-bit OAM X as stored (hardware wraps mod 512)
        return self.attr1 & 0x1FF

    @property
    def affine(self):
        return bool(self.attr0 & 0x100)

    @property
    def double_size(self):
        return self.affine and bool(self.attr0 & 0x200)

    @property
    def obj_mode(self):   # 0 normal, 1 semi-transparent, 2 window, 3 bitmap
        return (self.attr0 >> 10) & 3

    @property
    def bpp8(self):
        return bool(self.attr0 & 0x2000)

    @property
    def hflip(self):
        return not self.affine and bool(self.attr1 & 0x1000)

    @property
    def vflip(self):
        return not self.affine and bool(self.attr1 & 0x2000)

    @property
    def tile(self):
        return self.attr2 & 0x3FF

    @property
    def palette(self):
        return self.attr2 >> 12

    def as_dict(self):
        w, h = self.size
        return {"x": self.x, "y": self.y, "w": w, "h": h, "hflip": self.hflip, "vflip": self.vflip,
                "affine": self.affine, "double": self.double_size, "mode": self.obj_mode,
                "bpp8": self.bpp8, "tile": self.tile, "palette": self.palette,
                "attr": [self.attr0, self.attr1, self.attr2, self.attr3]}


class Assembly:
    """Frame assembly record (frames block): u16 count, u16 flags, s16 left, top, right, bottom,
    s16 cx, cy, then count * 8-byte OAM pieces (attr0, attr1, attr2, unused)."""

    def __init__(self, data, off):
        if off + 16 > len(data):
            raise BacError("frame assembly out of range @0x%x" % off)
        (self.count, self.flags, self.left, self.top, self.right, self.bottom,
         self.cx, self.cy) = struct.unpack_from("<HHhhhhhh", data, off)
        if off + 16 + 8 * self.count > len(data):
            raise BacError("frame assembly pieces out of range @0x%x" % off)
        self.pieces = [Piece(*struct.unpack_from("<HHHH", data, off + 16 + 8 * i)) for i in range(self.count)]

    @property
    def explicit_tiles(self):  # flags bit0: attr2 holds the tile number (else tiles are sequential)
        return bool(self.flags & 1)


class Frame:
    def __init__(self):
        self.assembly_ofs = None      # offset into frames block (relative to block start)
        self.tile_parts = []          # list of (ofs, size_in_units) relative to tiles block
        self.palettes = []            # list of (ofs, colors_hint, slot) relative to palette block
        self.duration = 0             # in 60 Hz frames (EndFrame u16 at +8; wait = value << 4, speed 0x10/frame)
        self.frame_no = 0             # EndFrame +6 (1-based), engine field unkA
        self.frame_count = 0          # EndFrame +4, engine field unkC (anim ends when unkA == unkC)
        self.events = []              # callbacks: dicts
        self.cmd_pos = None           # cmd-block position of the first command of this frame


class Animation:
    def __init__(self, index, pos, fmt, pad):
        self.index, self.pos, self.fmt, self.pad = index, pos, fmt, pad
        self.frames = []
        self.loop_pos = None          # EndAnimation jump target (cmd-block pos)
        self.loop_frame = None        # frame index of that target within this animation
        self.loop_anim = None         # animation index whose start == loop_pos (if outside)
        self.next_anim = None         # ChangeAnimation target
        self.vram = None              # info block: tile units needed for the 4 1D mapping modes


class Bac:
    def __init__(self, data):
        data = bytes(data)
        if data[:4] != b"BAC\n":
            raise BacError("bad magic %r" % data[:4])
        self.data = data
        off = struct.unpack_from("<6I", data, 4)
        self.ofs_anims, self.ofs_cmds, self.ofs_frames, self.ofs_pal, self.ofs_tiles = off[:5]
        # The optional info block (+0x18) exists when the header is 0x1C bytes long,
        # i.e. the first block (anims) starts at 0x1C or later. Used by MsgBox_InitSprite etc.
        self.ofs_info = off[5] if self.ofs_anims >= 0x1C else None
        for name in ("ofs_anims", "ofs_cmds", "ofs_frames", "ofs_pal", "ofs_tiles"):
            o = getattr(self, name)
            if o + 4 > len(data):
                raise BacError("%s 0x%x outside file" % (name, o))
        self.size = {n: struct.unpack_from("<I", data, getattr(self, n))[0]
                     for n in ("ofs_anims", "ofs_cmds", "ofs_frames", "ofs_pal", "ofs_tiles")}
        self.info = None
        self.notes = []
        self._parse_info()
        self._parse_anims()
        for a in self.animations:
            self._walk(a)

    # ----- blocks -----
    def _parse_info(self):
        if self.ofs_info is None:
            return
        d, o = self.data, self.ofs_info
        size = struct.unpack_from("<I", d, o)[0]
        if size < 0x1C:
            return
        hdr = struct.unpack_from("<12H", d, o + 4)
        self.info = {"anim_count": hdr[0], "max_vram_units": list(hdr[1:5]), "raw": list(hdr)}
        self._info_entries = []
        n = (size - 0x1C) // 0x14
        for i in range(n):
            self._info_entries.append(list(struct.unpack_from("<10H", d, o + 0x1C + 0x14 * i)))

    def _parse_anims(self):
        d = self.data
        n = (self.size["ofs_anims"] - 4) // 8
        self.animations = []
        for i in range(n):
            pos, fmt, pad = struct.unpack_from("<IHH", d, self.ofs_anims + 4 + 8 * i)
            a = Animation(i, pos, fmt, pad)
            if self.info is not None and i < len(self._info_entries):
                a.vram = self._info_entries[i][:4]
            self.animations.append(a)

    def _walk(self, anim):
        d = self.data
        base = self.ofs_cmds
        end = base + self.size["ofs_cmds"]
        pos = anim.pos
        cur = Frame()
        cur.cmd_pos = pos
        start_pos = {}
        state_assembly = None
        steps = 0
        while True:
            steps += 1
            if steps > 100000:
                raise BacError("anim %d: runaway command stream" % anim.index)
            p = base + pos
            if p + 4 > end:
                raise BacError("anim %d: command stream runs past block end" % anim.index)
            typ, ln = struct.unpack_from("<HH", d, p)
            if ln < 4 or p + ln > end:
                raise BacError("anim %d: bad command length %d @0x%x" % (anim.index, ln, pos))
            if cur.cmd_pos is None:
                cur.cmd_pos = pos
            if typ == CMD_END_FRAME:
                cur.frame_count, cur.frame_no, dur = struct.unpack_from("<HHH", d, p + 4)
                cur.duration = dur
                if cur.assembly_ofs is None:
                    cur.assembly_ofs = state_assembly
                start_pos[cur.cmd_pos] = len(anim.frames)
                anim.frames.append(cur)
                cur = Frame()
            elif typ == CMD_FRAME_ASSEMBLY:
                cur.assembly_ofs = state_assembly = struct.unpack_from("<I", d, p + 4)[0]
            elif typ == CMD_SPRITE_PARTS:
                for k in range(4, ln, 8):
                    o, s = struct.unpack_from("<IH", d, p + k)
                    cur.tile_parts.append((o, s))
            elif typ == CMD_PALETTE:
                o, cnt, slot = struct.unpack_from("<IHH", d, p + 4)
                cur.palettes.append((o, cnt, slot))
            elif typ == CMD_END_ANIMATION:
                anim.loop_pos = struct.unpack_from("<I", d, p + 4)[0]
                if anim.loop_pos in start_pos:
                    anim.loop_frame = start_pos[anim.loop_pos]
                else:
                    for b in self.animations:
                        if b.pos == anim.loop_pos:
                            anim.loop_anim = b.index
                            break
                break
            elif typ == CMD_CHANGE_ANIMATION:
                anim.next_anim = struct.unpack_from("<H", d, p + 4)[0]
                break
            elif typ in (CMD_CALLBACK_6, CMD_CALLBACK_7, CMD_CALLBACK_8):
                body = d[p + 4:p + ln]
                ev = {"cmd": typ, "raw": body.hex()}
                if typ == CMD_CALLBACK_7 and ln == 16:
                    # Callback 7 = collision box: u16 slot, u16 ?, s16 left, top, right, bottom
                    slot, unk, l, t, r, b = struct.unpack_from("<HHhhhh", d, p + 4)
                    ev = {"cmd": 7, "kind": "hitbox", "slot": slot, "flags": unk, "rect": [l, t, r, b]}
                cur.events.append(ev)
            elif typ in (5, 10, 11):
                pass  # no-op skip commands
            else:
                raise BacError("anim %d: unknown command %d @0x%x" % (anim.index, typ, pos))
            pos += ln
        if cur.tile_parts or cur.palettes or cur.events:
            self.notes.append("anim %d: commands after last EndFrame ignored" % anim.index)

    # ----- data access -----
    def assembly(self, frame):
        if frame.assembly_ofs is None:
            return None
        return Assembly(self.data, self.ofs_frames + frame.assembly_ofs)

    def tile_stream(self, ofs):
        p = self.ofs_tiles + ofs
        try:
            return decompress(self.data[p:])
        except DecompressError as e:
            raise BacError("tile data @0x%x: %s" % (ofs, e))

    def palette_colors(self, ofs):
        p = self.ofs_pal + ofs
        try:
            raw = decompress(self.data[p:])
        except DecompressError as e:
            raise BacError("palette data @0x%x: %s" % (ofs, e))
        return list(struct.unpack_from("<%dH" % (len(raw) // 2), raw))

    def compression_kinds(self):
        """Return {('tiles'|'palette', type_byte): count} over all referenced streams."""
        seen = {}
        for a in self.animations:
            for f in a.frames:
                for o, _ in f.tile_parts:
                    k = ("tiles", self.data[self.ofs_tiles + o])
                    seen[k] = seen.get(k, 0) + 1
                for o, _, _ in f.palettes:
                    k = ("palette", self.data[self.ofs_pal + o])
                    seen[k] = seen.get(k, 0) + 1
        return seen


class RenderState:
    """Emulates what the engine leaves in VRAM / palette RAM for one sprite while it plays
    an animation: SpriteParts commands overwrite the sprite's VRAM area from its start,
    Palette commands write 16-colour slots relative to the sprite's palette base."""

    def __init__(self, bac, anim, mapping_shift=1):
        """mapping_shift = OBJ 1D boundary of the screen the sprite is drawn on:
        0 = 1D 32K (32-byte units), 1 = 64K (stages, Stage_InitDisplay), 2 = 128K (menus,
        Vram_SetBankConfig mode 0), 3 = 256K. 3D (texture) sprites always use 0."""
        self.bac, self.anim = bac, anim
        self.shift = 0 if anim.fmt >= 2 else mapping_shift
        self.vram = bytearray()
        self.pal = {}   # colour index (relative) -> BGR555
        self.has_tiles = False
        self.has_palette = False

    def apply(self, frame):
        unit_shift = FMT_UNIT_SHIFT[self.anim.fmt] if self.anim.fmt < len(FMT_UNIT_SHIFT) else 5
        if frame.tile_parts:
            # VRAMTransfer_Flush copies/decompresses the WHOLE stream (size from its header) to
            # dst; the entry's unit count only advances dst for the next entry. Streams can be
            # longer than the count (e.g. explicit-tile frames index into a full tile set).
            # VRAM keeps whatever earlier frames wrote beyond the new data (as on hardware).
            vram = bytearray(self.vram)
            dst = 0
            for o, units in frame.tile_parts:
                raw = self.bac.tile_stream(o)
                end = dst + len(raw)
                if len(vram) < end:
                    vram += bytes(end - len(vram))
                vram[dst:end] = raw
                # BAC_FrameGroupFunc_SpriteParts: dst += ceil(size / 2^shift) << (shift + bpp)
                dst += ((units + (1 << self.shift) - 1) >> self.shift) << (self.shift + unit_shift)
            if len(vram) < dst:
                vram += bytes(dst - len(vram))
            self.vram = vram
            self.has_tiles = True
        for o, cnt, slot in frame.palettes:
            cols = self.bac.palette_colors(o)
            for i, c in enumerate(cols):
                self.pal[slot * 16 + i] = c
            self.has_palette = True


def _pixel_4bpp(vram, off):
    b = vram[off >> 1] if (off >> 1) < len(vram) else 0
    return (b >> 4) if off & 1 else (b & 0xF)


def render(bac, anim, state, frame, stats=None):
    """Compose one frame. Returns dict(w, h, rgba, ox, oy, bbox) or None for an empty frame.
    ox/oy = position of the sprite origin inside the image (pixel coords)."""
    asm = bac.assembly(frame)
    if asm is None or asm.count == 0:
        return None
    fmt = anim.fmt
    linear = fmt >= 3
    if fmt in (2, 6, 7, 8, 9):
        raise BacError("animation format %d (%s) not supported" % (fmt, FMT_NAMES.get(fmt)))
    # piece rectangles relative to the origin
    rects = []
    for pc in asm.pieces:
        w, h = pc.size
        if pc.obj_mode == 3:
            raise BacError("bitmap OBJ pieces not supported")
        dw, dh = (w * 2, h * 2) if pc.double_size else (w, h)
        x0 = pc.x - asm.cx
        y0 = pc.y - asm.cy
        # OAM coordinates wrap (9-bit X, 8-bit Y). Pick the wrapped value that lies inside
        # the frame bounding box when the stored one does not.
        if (x0 + dw > asm.right or x0 < asm.left) and asm.left <= x0 - 512 and x0 - 512 + dw <= asm.right:
            x0 -= 512
        if (y0 + dh > asm.bottom or y0 < asm.top) and asm.top <= y0 - 256 and y0 - 256 + dh <= asm.bottom:
            y0 -= 256
        rects.append((x0, y0, dw, dh))
    minx = min(min(r[0] for r in rects), asm.left)
    miny = min(min(r[1] for r in rects), asm.top)
    maxx = max(max(r[0] + r[2] for r in rects), asm.right)
    maxy = max(max(r[1] + r[3] for r in rects), asm.bottom)
    W, H = maxx - minx, maxy - miny
    img = bytearray(W * H * 4)
    # palette index plane: (palette base + index) per pixel, 0 = transparent. Index 0 of a
    # 4bpp piece is transparent; an 8bpp piece reads all 256 entries (index 0 transparent).
    idxplane = bytearray(W * H)
    vram = state.vram
    pal = state.pal

    def colour(idx):
        c = pal.get(idx)
        if c is None:
            g = (idx * 17) & 0xFF if idx < 16 else idx & 0xFF  # visible fallback grey ramp
            return (g, g, g, 255)
        return bgr555_to_rgba(c)

    # AnimatorSprite_Draw: character names count in units of 32 << shift bytes; sequential
    # pieces advance chr by ceil(tiles / 2^shift) << (8bpp ? 1 : 0). 3D draws advance texAddr
    # by tiles << unit_shift (Model_DrawSprite3D).
    shift = state.shift
    unit_bytes = 32 << shift
    seq_unit = 0
    order = []
    for i, pc in enumerate(asm.pieces):
        w, h = pc.size
        tiles = OBJ_TILES[pc.size_index]
        bpp8 = (fmt in (1, 5)) or (not linear and pc.bpp8)
        if linear:
            start = seq_unit * 32
            seq_unit += tiles << (FMT_UNIT_SHIFT[fmt] - 5)
        elif asm.explicit_tiles:
            start = pc.tile * unit_bytes
        else:
            start = seq_unit * unit_bytes
            seq_unit += ((tiles + (1 << shift) - 1) >> shift) << (1 if fmt == 1 else 0)
        order.append((i, pc, start, bpp8))
    # first OAM entry is in front: draw back to front
    for i, pc, start, bpp8 in reversed(order):
        w, h = pc.size
        x0, y0, dw, dh = rects[i]
        offx, offy = (w // 2, h // 2) if pc.double_size else (0, 0)
        palbase = 0 if (linear or bpp8) else pc.palette * 16
        if stats is not None:
            if pc.affine:
                stats["affine_pieces"] = stats.get("affine_pieces", 0) + 1
        for py in range(h):
            sy = h - 1 - py if pc.vflip else py
            for px in range(w):
                sx = w - 1 - px if pc.hflip else px
                if linear:
                    if fmt == 4:  # pal16 texture: row-major, 2 px per byte, low nibble first
                        idx = _pixel_4bpp(vram, start * 2 + sy * w + sx)
                    elif fmt == 3:
                        o = start * 4 + sy * w + sx
                        b = vram[o >> 2] if (o >> 2) < len(vram) else 0
                        idx = (b >> ((o & 3) * 2)) & 3
                    else:  # 5: pal256 linear
                        o = start + sy * w + sx
                        idx = vram[o] if o < len(vram) else 0
                else:
                    t = (sy >> 3) * (w >> 3) + (sx >> 3)
                    if bpp8:
                        o = start + t * 64 + (sy & 7) * 8 + (sx & 7)
                        idx = vram[o] if o < len(vram) else 0
                    else:
                        o = (start + t * 32) * 2 + ((sy & 7) * 8 + (sx & 7))
                        idx = _pixel_4bpp(vram, o)
                if idx == 0:
                    continue
                r, g, b, a = colour(palbase + idx)
                X = x0 - minx + offx + px
                Y = y0 - miny + offy + py
                q = (Y * W + X) * 4
                img[q:q + 4] = bytes((r, g, b, a))
                idxplane[Y * W + X] = (palbase + idx) & 0xFF
    return {"w": W, "h": H, "rgba": bytes(img), "ox": -minx, "oy": -miny,
            "bbox": [asm.left, asm.top, asm.right, asm.bottom], "idx": bytes(idxplane),
            "pal": [colour(i) for i in range(256)]}
