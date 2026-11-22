#include "input.h"
#include <kos.h>
#include <dc/maple.h>
#include <dc/maple/controller.h>

int g_input_deadzone = 32;
int g_input_dir_threshold = 64;

void input_poll(InputState *in)
{
    uint32_t prev = in->held, h = 0;
    maple_device_t *dev = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
    in->ax = in->ay = 0;
    if(dev) {
        cont_state_t *st = (cont_state_t *)maple_dev_status(dev);
        if(st) {
            int x = st->joyx, y = st->joyy;
            if(x > -g_input_deadzone && x < g_input_deadzone) x = 0;
            if(y > -g_input_deadzone && y < g_input_deadzone) y = 0;
            in->ax = x; in->ay = y;
            if(st->buttons & CONT_DPAD_LEFT  || x <= -g_input_dir_threshold) h |= IN_LEFT;
            if(st->buttons & CONT_DPAD_RIGHT || x >=  g_input_dir_threshold) h |= IN_RIGHT;
            if(st->buttons & CONT_DPAD_UP    || y <= -g_input_dir_threshold) h |= IN_UP;
            if(st->buttons & CONT_DPAD_DOWN  || y >=  g_input_dir_threshold) h |= IN_DOWN;
            if(st->buttons & CONT_A) h |= IN_JUMP;
            if(st->buttons & CONT_X) h |= IN_BOOST;
            if(st->buttons & CONT_B) h |= IN_TRICK;
            if(st->buttons & CONT_Y) h |= IN_ALT;
            if(st->buttons & CONT_START) h |= IN_START;
            if(st->rtrig >= 128) h |= IN_R;   /* analog triggers 0..255 */
            if(st->ltrig >= 128) h |= IN_L;
        }
    }
    in->held = h;
    in->pressed = h & ~prev;
    in->released = prev & ~h;
}
