#include <nitro/types.h>

typedef enum StrmFormat_0201ed04 {
    STRM_FORMAT_PCM8,
    STRM_FORMAT_PCM16,
    STRM_FORMAT_ADPCM
} StrmFormat_0201ed04;

typedef enum NNSSndStrmFormat_0201ed04 {
    NNS_SND_STRM_FORMAT_PCM8,
    NNS_SND_STRM_FORMAT_PCM16
} NNSSndStrmFormat_0201ed04;

typedef struct AdpcmState_0201ed04 {
    s16 prevSample;
    u8 prevIndex;
    u8 padding;
} AdpcmState_0201ed04;

typedef void (*StrmCallback_0201ed04)(s32 status, int numChannels, void *buffer[], u32 len, NNSSndStrmFormat_0201ed04 format, void *arg);

typedef struct StrmData_0201ed04 {
    u8 header[0x18];
    u8 format;
    u8 loopFlag;
    u8 numChannels;
    u8 pad_;
    u16 sampleRate;
    u16 timer;
    u32 loopStart;
    u32 loopEnd;
    u32 dataOffset;
    u32 numBlocks;
    u32 blockSize;
    u32 blockSamples;
    u32 lastBlockSize;
    u32 lastBlockSamples;
} StrmData_0201ed04;

typedef struct StrmPlayer_0201ed04 {
    u8 stream[0x5c];
    u8 file[0x44];
    u32 fileOffset;
    StrmData_0201ed04 info;
    u8 fader[0x10];
    AdpcmState_0201ed04 adpcmState[6];
    BOOL activeFlag : 1;
    BOOL playFlag : 1;
    BOOL startFlag : 1;
    BOOL fadeOutFlag : 1;
    BOOL dirtyFlag : 1;
    BOOL finishFlag : 1;
    BOOL monoFlag : 1;
    volatile int finishCounter;
    volatile BOOL prepareFlag;
    u8 pad118[0x130 - 0x118];
    StrmCallback_0201ed04 strmCallback;
    void *strmCallbackArg;
    void *sndArcStrmCallback;
    u8 pad13C[0x158 - 0x13c];
    u32 curSample;
} StrmPlayer_0201ed04;

typedef struct LoadCommand_0201ed04 {
    u8 link[8];
    StrmPlayer_0201ed04 *player;
    s32 status;
    int numChannels;
    void *buffer[6];
    u32 bufLen;
} LoadCommand_0201ed04;

extern u8 data_02083f5c[];
extern u8 *data_02083f4c;
extern const s16 data_02072a4c[];
extern const s8 data_02072a3c[];

void MI_CpuFill8(void *dst, u8 val, u32 size);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void OS_LockMutex(void *m);
void OS_UnlockMutex(void *m);
void DC_FlushRange(const void *p, u32 size);
BOOL FS_SeekFile(void *file, s32 ofs, s32 whence);
s32 FS_ReadFile(void *file, void *dst, s32 len);
void OnDataEnd(StrmPlayer_0201ed04 *player);

static inline void MI_CpuClear8_0201ed04(void *dest, u32 size)
{
    MI_CpuFill8(dest, 0, size);
}

static inline s16 DecodeAdpcm(int code, AdpcmState_0201ed04 *state)
{
    int step;
    int sample;
    int index;
    int d;

    sample = state->prevSample;
    index = state->prevIndex;

    step = data_02072a4c[index];

    d = step >> 3;
    if (code & 4)
        d += step;
    if (code & 2)
        d += step >> 1;
    if (code & 1)
        d += step >> 2;

    if (code & 8) {
        sample -= d;
        if (sample < -32768)
            sample = -32768;
    } else {
        sample += d;
        if (sample > 32767)
            sample = 32767;
    }

    index += data_02072a3c[code];

    if (index < 0)
        index = 0;
    else if (index > 89 - 1)
        index = 89 - 1;

    state->prevSample = (s16)sample;
    state->prevIndex = (u8)index;

    return (s16)sample;
}

void MakeWaveData(LoadCommand_0201ed04 *command)
{
    StrmPlayer_0201ed04 *player = command->player;
    BOOL loopFlag;
    u32 destOffset;
    u32 restSize;
    u32 blockNo;
    u32 blockSize;
    u32 blockSamples;
    u32 blockOffsetSample;
    u32 blockOffset;
    u32 offset;
    u32 samples;
    u32 size;
    u32 readSize;
    int ch;

    if (player->finishFlag && player->finishCounter > 0) {
        player->finishCounter--;
    }

    destOffset = 0;

    restSize = command->bufLen;
    while (restSize > 0) {
        if (player->finishFlag) {
            for (ch = 0; ch < command->numChannels; ch++) {
                MI_CpuClear8_0201ed04((u8 *)(command->buffer[ch]) + destOffset, restSize);
            }
            break;
        }

        blockNo = player->curSample / player->info.blockSamples;

        if (blockNo < player->info.numBlocks - 1) {
            blockSize = player->info.blockSize;
            blockSamples = player->info.blockSamples;
        } else {
            blockSize = player->info.lastBlockSize;
            blockSamples = player->info.lastBlockSamples;
        }

        blockOffsetSample = player->curSample;
        blockOffsetSample -= blockNo * player->info.blockSamples;

        samples = restSize;
        if (player->info.format != STRM_FORMAT_PCM8) {
            samples >>= 1;
        }

        if (player->dirtyFlag) {
            if (blockOffsetSample == 0) {
                player->dirtyFlag = FALSE;
            } else {
                samples = blockOffsetSample;
                blockOffsetSample = 0;
            }
        }

        loopFlag = FALSE;
        if (blockOffsetSample + samples >= blockSamples) {
            samples = blockSamples - blockOffsetSample;

            if (blockNo >= player->info.numBlocks - 1) {
                if (player->info.loopFlag) {
                    loopFlag = TRUE;
                } else {
                    player->finishFlag = TRUE;
                }
            }
        }

        blockOffset = blockOffsetSample;
        size = samples;
        switch (player->info.format) {
        case STRM_FORMAT_PCM8:
            readSize = size;
            break;

        case STRM_FORMAT_PCM16:
            blockOffset <<= 1;
            size <<= 1;
            readSize = size;
            break;

        case STRM_FORMAT_ADPCM: {
            u32 endSample = blockOffsetSample + samples;
            blockOffset >>= 1;
            endSample++;
            endSample >>= 1;
            readSize = endSample - blockOffset;
            if (blockOffsetSample == 0) {
                readSize += sizeof(AdpcmState_0201ed04);
            } else {
                blockOffset += sizeof(AdpcmState_0201ed04);
            }
            size <<= 1;

            break;
        }
        }

        offset = blockOffset;
        offset += blockNo * player->info.blockSize * player->info.numChannels;
        offset += player->info.dataOffset;
        offset += player->fileOffset;

        for (ch = 0; ch < command->numChannels; ch++) {
            void *dest;
            void *read_dest;

            dest = read_dest = (u8 *)(command->buffer[ch]) + destOffset;

            if (ch < player->info.numChannels) {
                s32 resultSize;

                if (player->info.format == STRM_FORMAT_ADPCM) {
                    OS_LockMutex(data_02083f5c);
                    read_dest = data_02083f4c;
                }

                FS_SeekFile(player->file, (s32)(offset + ch * blockSize), 0);
                resultSize = FS_ReadFile(player->file, read_dest, (s32)readSize);

                if (resultSize != readSize) {
                    size = 0;
                    samples = 0;
                    loopFlag = FALSE;
                    player->finishFlag = TRUE;
                    if (player->info.format == STRM_FORMAT_ADPCM) {
                        OS_UnlockMutex(data_02083f5c);
                    }
                    break;
                }

                if (player->info.format == STRM_FORMAT_ADPCM) {
                    AdpcmState_0201ed04 *state = &player->adpcmState[ch];
                    u8 *srcp = data_02083f4c;
                    s16 *destp = dest;
                    u32 i;
                    u32 end;

                    if (blockOffsetSample == 0) {
                        *state = *((AdpcmState_0201ed04 *)srcp)++;
                    }

                    end = blockOffsetSample + samples;

                    i = blockOffsetSample;
                    if (i & 0x01) {
                        *destp++ = DecodeAdpcm((*srcp >> 4) & 0x0f, state);
                        i++;
                        srcp++;
                    }
                    while (i < (end & ~0x01)) {
                        *destp++ = DecodeAdpcm(*srcp & 0x0f, state);
                        i++;
                        *destp++ = DecodeAdpcm((*srcp >> 4) & 0x0f, state);
                        i++;
                        srcp++;
                    }
                    if (i < end) {
                        *destp++ = DecodeAdpcm(*srcp & 0x0f, state);
                        i++;
                    }
                    OS_UnlockMutex(data_02083f5c);
                }
            } else {
                if (player->monoFlag) {
                    MI_CpuClear8_0201ed04(dest, size);
                } else {
                    MI_CpuCopy8((u8 *)(command->buffer[0]) + destOffset, dest, size);
                }
            }
        }

        if (player->dirtyFlag) {
            player->dirtyFlag = FALSE;
            continue;
        }

        if (loopFlag) {
            player->curSample = player->info.loopStart;
        } else {
            player->curSample += samples;
        }

        destOffset += size;

        restSize -= size;

        if (player->finishFlag && player->sndArcStrmCallback) {
            OnDataEnd(player);
        }
    }

    if (player->strmCallback != NULL) {
        player->strmCallback(command->status, command->numChannels, command->buffer, command->bufLen,
                             player->info.format == STRM_FORMAT_PCM8 ? NNS_SND_STRM_FORMAT_PCM8 : NNS_SND_STRM_FORMAT_PCM16, player->strmCallbackArg);
    }

    for (ch = 0; ch < command->numChannels; ch++) {
        DC_FlushRange(command->buffer[ch], command->bufLen);
    }

    if (command->status == 0) {
        player->prepareFlag = TRUE;
    }
}
