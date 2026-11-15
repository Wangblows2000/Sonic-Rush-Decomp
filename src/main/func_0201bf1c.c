/* Clears a pointed-to word and the pointer to it. Matched (CW 1.2). */
void func_0201bf1c(int **pp) {
    int *p = *pp;
    if (p != 0) {
        *p = 0;
        *pp = 0;
    }
}
