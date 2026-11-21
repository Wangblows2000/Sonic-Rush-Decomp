/* Clears a pointed-to word and the pointer to it. Matched (CW 1.2). */
void NNS_SndHandleReleaseSeq(int **pp) {
    int *p = *pp;
    if (p != 0) {
        *p = 0;
        *pp = 0;
    }
}
