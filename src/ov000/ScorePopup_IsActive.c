#include <nitro/types.h>

extern void *data_ov000_0236a980;

BOOL ScorePopup_IsActive(void) {
    return data_ov000_0236a980 != NULL;
}
