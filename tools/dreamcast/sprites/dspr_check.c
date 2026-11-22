/* dspr_check.c - minimal reader for the "DSPR" sprite frame table written by bac2dc.py.
 *
 * Shows the exact on-disk layout a Dreamcast (SH-4, little-endian) runtime consumes, and
 * validates a file: bounds of every image rect against its atlas page, frame image indices,
 * animation frame ranges and loop targets.  Host build:
 *     gcc -O2 -Wall -o /tmp/sprites/dspr_check tools/dreamcast/sprites/dspr_check.c
 * Usage: dspr_check frames.bin [-v]      exit status 0 = valid
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { char magic[4]; uint16_t version, pages, images, anims, frames, reserved; } DsprHeader; /* 16 bytes */
typedef struct { uint16_t w, h; } DsprPage;                                                             /* 4 bytes  */
typedef struct { uint8_t page, pad; uint16_t x, y, w, h; int16_t origin_x, origin_y; uint16_t pad2; } DsprImage; /* 16 bytes */
typedef struct { uint16_t first_frame, frame_count; int16_t loop_frame, next_anim; uint8_t format, pad; uint16_t pad2; } DsprAnim; /* 12 bytes */
typedef struct { uint16_t image, duration; } DsprFrame;                                                 /* 4 bytes  */

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s frames.bin [-v]\n", argv[0]); return 2; }
    int verbose = argc > 2 && !strcmp(argv[2], "-v");
    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror(argv[1]); return 2; }
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *buf = malloc(n);
    if (fread(buf, 1, n, f) != (size_t)n) { perror("read"); return 2; }
    fclose(f);

    const DsprHeader *h = (const DsprHeader *)buf;
    if (n < (long)sizeof *h || memcmp(h->magic, "DSPR", 4) || h->version != 1) { fprintf(stderr, "bad header\n"); return 1; }
    long need = sizeof *h + h->pages * 4L + h->images * 16L + h->anims * 12L + h->frames * 4L;
    if (need != n) { fprintf(stderr, "size mismatch: header implies %ld, file %ld\n", need, n); return 1; }
    const DsprPage *pg = (const DsprPage *)(buf + sizeof *h);
    const DsprImage *im = (const DsprImage *)(pg + h->pages);
    const DsprAnim *an = (const DsprAnim *)(im + h->images);
    const DsprFrame *fr = (const DsprFrame *)(an + h->anims);
    int errors = 0;
    for (int i = 0; i < h->images; i++) {
        if (im[i].page >= h->pages || im[i].x + im[i].w > pg[im[i].page].w || im[i].y + im[i].h > pg[im[i].page].h) {
            printf("image %d out of page\n", i);
            errors++;
        }
    }
    unsigned total = 0;
    for (int a = 0; a < h->anims; a++) {
        if (an[a].first_frame != total) { printf("anim %d: first_frame %u != %u\n", a, an[a].first_frame, total); errors++; }
        if (an[a].loop_frame >= (int)an[a].frame_count) { printf("anim %d: loop_frame out of range\n", a); errors++; }
        if (an[a].next_anim >= (int)h->anims) { printf("anim %d: next_anim out of range\n", a); errors++; }
        unsigned dur = 0;
        for (int k = 0; k < an[a].frame_count; k++) {
            const DsprFrame *q = &fr[an[a].first_frame + k];
            if (q->image != 0xFFFF && q->image >= h->images) { printf("anim %d frame %d: bad image\n", a, k); errors++; }
            dur += q->duration;
        }
        if (verbose)
            printf("anim %2d: %3u frames, %5u ticks @60Hz, loop %d, next %d, fmt %u\n", a, an[a].frame_count, dur,
                   an[a].loop_frame, an[a].next_anim, an[a].format);
        total += an[a].frame_count;
    }
    if (total != h->frames) { printf("frame total mismatch\n"); errors++; }
    printf("%s: %u pages, %u images, %u anims, %u frames: %s\n", argv[1], h->pages, h->images, h->anims, h->frames,
           errors ? "INVALID" : "ok");
    free(buf);
    return errors ? 1 : 0;
}
