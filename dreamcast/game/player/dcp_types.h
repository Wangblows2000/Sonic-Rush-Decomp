/*
 * Native player module (Dreamcast port) - basic types.
 *
 * No DS SDK headers: everything the player physics needs is defined here.
 * Units follow the original game exactly:
 *   - positions: s32, 24.8 fixed point (0x100 = 1 pixel)
 *   - velocities / speeds: s16, 1/256 pixel per frame
 *   - angles: u8 (0x100 = full turn, 0x40 = 90 deg, y axis points down)
 *   - trig tables: s16, 4.12 fixed point (0x1000 = 1.0), indexed by
 *     (angle16 >> 4), sin/cos interleaved (the original data_0206e470 layout).
 */
#ifndef DCP_TYPES_H
#define DCP_TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

typedef s32 dcp_bool;
#define DCP_TRUE 1
#define DCP_FALSE 0

#define DCP_ABS(v) ((v) < 0 ? -(v) : (v))

#endif
