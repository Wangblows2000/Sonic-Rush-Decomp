#include "input.h"
#include <kos.h>
#include <dc/maple.h>
#include <dc/maple/controller.h>

int g_input_deadzone = 32;
int g_input_dir_threshold = 64;

static void poll_dev(maple_device_t *dev, InputState *in)
{
    uint32_t prev = in->held, h = 0;
    in->ax = in->ay = 0;
    in->lt = in->rt = 0;
    in->dpad = 0;
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
            in->lt = st->ltrig; in->rt = st->rtrig;
            if(st->buttons & CONT_DPAD_LEFT)  in->dpad |= IN_LEFT;
            if(st->buttons & CONT_DPAD_RIGHT) in->dpad |= IN_RIGHT;
            if(st->buttons & CONT_DPAD_UP)    in->dpad |= IN_UP;
            if(st->buttons & CONT_DPAD_DOWN)  in->dpad |= IN_DOWN;
        }
    }
    in->held = h;
    in->pressed = h & ~prev;
    in->released = prev & ~h;
}

void input_poll(InputState *in)
{
    poll_dev(maple_enum_type(0, MAPLE_FUNC_CONTROLLER), in);   /* the first controller, any port */
}

/* Battle Play: the controller in port `port` (0 = A, 1 = B) */
static maple_device_t *port_dev(int port)
{
    maple_device_t *dev = maple_enum_dev(port, 0);
    return dev && (dev->info.functions & MAPLE_FUNC_CONTROLLER) ? dev : NULL;
}
void input_poll_port(int port, InputState *in) { poll_dev(port_dev(port), in); }
int input_port_present(int port) { return port_dev(port) != NULL; }

