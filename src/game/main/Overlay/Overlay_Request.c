#include <nitro/types.h>

extern void *gOverlayRequestedId;
extern void *gOverlayCallback;

void Overlay_Request(void *a, void *b) {
    gOverlayRequestedId = a;
    gOverlayCallback = b;
}
