#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; s32 unk8; } UnkNode_02034114;
UnkNode_02034114 *FileRequest_FindPending(void);
void FileRequest_StartRead(UnkNode_02034114 *n);
void FileRequest_Finish(UnkNode_02034114 *n);

void FileRequest_Update(void) {
    BOOL cont = TRUE;
    do {
        UnkNode_02034114 *n = FileRequest_FindPending();
        if (n == NULL) return;
        switch (n->unk8) {
        case 1:
            FileRequest_StartRead(n);
            cont = FALSE;
            break;
        case 3:
            FileRequest_Finish(n);
            if (n->unk8 != 5) cont = FALSE;
            break;
        }
    } while (cont);
}
