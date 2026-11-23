/* Native Dreamcast input: maple controller -> game buttons. */
#ifndef SRDC_INPUT_H
#define SRDC_INPUT_H
#include <stdint.h>

enum {
    IN_LEFT = 1 << 0, IN_RIGHT = 1 << 1, IN_UP = 1 << 2, IN_DOWN = 1 << 3,
    IN_JUMP = 1 << 4,   /* A */
    IN_BOOST = 1 << 5,  /* X */
    IN_TRICK = 1 << 6,  /* B */
    IN_ALT = 1 << 7,    /* Y */
    IN_START = 1 << 8,
    IN_R = 1 << 9,      /* right trigger: DS R (tricks, air dash / homing / hover, grind tricks) */
    IN_L = 1 << 10      /* left trigger: DS L */
};

typedef struct {
    uint32_t held, pressed, released;
    int ax, ay;          /* analog, -128..127 after dead zone */
    int lt, rt;          /* analogue triggers 0..255 (IN_L / IN_R are lt / rt >= 128) */
    uint32_t dpad;       /* IN_LEFT..IN_DOWN from the D-pad alone (held carries the stick too) */
} InputState;
#define IN_HAS_RAW 1     /* lt, rt, dpad present (special stages: trigger hysteresis, D-pad-only inputs) */

extern int g_input_deadzone;      /* default 32 */
extern int g_input_dir_threshold; /* analog magnitude that counts as a direction, default 64 */
void input_poll(InputState *in);              /* the first controller found (any port) */
void input_poll_port(int port, InputState *in); /* Battle Play: controller port 0 (A) / 1 (B); none = all released */
int input_port_present(int port);

#endif
