#include <nitro/types.h>

typedef struct {
    u32 lock;                                /* 0x00 */
    void (*callback)(u32 result, void *arg); /* 0x04 */
    void *buffer;                            /* 0x08 */
    void *buffer2;                           /* 0x0c */
    void *callbackArg;                       /* 0x10 */
    u32 command;                             /* 0x14 */
    u32 index;                               /* 0x18 */
    void (*interrupt)(void);                 /* 0x1c */
} UnkRtcWork;

typedef struct {
    u32 year : 8;
    u32 month : 5;
    u32 : 3;
    u32 day : 6;
    u32 : 2;
    u32 week : 3;
    u32 : 5;
} UnkRtcRawDate;

typedef struct {
    u32 hour : 6;
    u32 afternoon : 1;
    u32 : 1;
    u32 minute : 7;
    u32 : 1;
    u32 second : 7;
    u32 : 9;
} UnkRtcRawTime;

typedef struct {
    u32 week : 3;
    u32 : 4;
    u32 weekEnable : 1;
    u32 hour : 6;
    u32 afternoon : 1;
    u32 hourEnable : 1;
    u32 minute : 7;
    u32 minuteEnable : 1;
    u32 : 8;
} UnkRtcRawAlarm;

typedef struct {
    u16 intr1 : 4;
    u16 : 2;
    u16 intr2 : 1;
    u16 : 9;
} UnkRtcRawStatus2;

typedef struct {
    u32 year;
    u32 month;
    u32 day;
    u32 week;
} UnkRtcDate;

typedef struct {
    u32 hour;
    u32 minute;
    u32 second;
} UnkRtcTime;

typedef struct {
    u32 week;
    u32 hour;
    u32 minute;
    u32 enable;
} UnkRtcAlarm;

#define RAW_DATE (*(UnkRtcRawDate *)0x027ffde8)
#define RAW_STATUS2 (*(UnkRtcRawStatus2 *)0x027ffdea)
#define RAW_TIME (*(UnkRtcRawTime *)0x027ffdec)
#define RAW_ALARM (*(UnkRtcRawAlarm *)0x027ffdec)

extern UnkRtcWork data_0207f0e4;

u32 func_0200ea8c(u32 bcd);
BOOL RTCi_WriteRawStatus2Async(void);

void func_0200eb10(u32 tag, u32 data, BOOL err)
{
    u32 result;
    u32 status;
    u32 command;
    void (*cb)(u32 result, void *arg);

    if (err) {
        if (data_0207f0e4.index != 0) {
            data_0207f0e4.index = 0;
        }
        if (data_0207f0e4.lock != 0) {
            data_0207f0e4.lock = 0;
        }
        cb = data_0207f0e4.callback;
        if (cb != NULL) {
            data_0207f0e4.callback = NULL;
            cb(6, data_0207f0e4.callbackArg);
        }
        return;
    }

    command = (u8)((data & 0x7f00) >> 8);
    status = data & 0xff;
    if (command == 0x30) {
        if (data_0207f0e4.interrupt != NULL) {
            data_0207f0e4.interrupt();
        }
        return;
    }

    if (status == 0) {
        result = 0;
        switch (data_0207f0e4.command) {
        case 0: {
            UnkRtcDate *date = data_0207f0e4.buffer;
            date->year = func_0200ea8c(RAW_DATE.year);
            date->month = func_0200ea8c(RAW_DATE.month);
            date->day = func_0200ea8c(RAW_DATE.day);
            date->week = RAW_DATE.week;
            break;
        }
        case 1: {
            UnkRtcTime *time = data_0207f0e4.buffer;
            time->hour = func_0200ea8c(RAW_TIME.hour);
            time->minute = func_0200ea8c(RAW_TIME.minute);
            time->second = func_0200ea8c(RAW_TIME.second);
            break;
        }
        case 2: {
            UnkRtcDate *date = data_0207f0e4.buffer;
            UnkRtcTime *time;
            date->year = func_0200ea8c(*(u32 *)0x027ffde8 & 0xff);
            date->month = func_0200ea8c(RAW_DATE.month);
            date->day = func_0200ea8c(RAW_DATE.day);
            date->week = RAW_DATE.week;
            time = data_0207f0e4.buffer2;
            time->hour = func_0200ea8c(RAW_TIME.hour);
            time->minute = func_0200ea8c(RAW_TIME.minute);
            time->second = func_0200ea8c(RAW_TIME.second);
            break;
        }
        case 3:
        case 4:
        case 5:
            break;
        case 6: {
            u32 *p = data_0207f0e4.buffer;
            switch (RAW_STATUS2.intr1) {
            case 4:
                *p = TRUE;
                break;
            default:
                *p = FALSE;
                break;
            }
            break;
        }
        case 7: {
            u32 *p = data_0207f0e4.buffer;
            if (RAW_STATUS2.intr2) {
                *p = TRUE;
            } else {
                *p = FALSE;
            }
            break;
        }
        case 8: {
            UnkRtcAlarm *alarm = data_0207f0e4.buffer;
            alarm->week = RAW_ALARM.week;
            alarm->hour = func_0200ea8c(RAW_ALARM.hour);
            alarm->minute = func_0200ea8c(RAW_ALARM.minute);
            alarm->enable = 0;
            if (RAW_ALARM.weekEnable) {
                alarm->enable += 1;
            }
            if (RAW_ALARM.hourEnable) {
                alarm->enable += 2;
            }
            if (RAW_ALARM.minuteEnable) {
                alarm->enable += 4;
            }
            break;
        }
        case 9:
            if (data_0207f0e4.index == 0) {
                if (*(u32 *)data_0207f0e4.buffer == TRUE) {
                    if (RAW_STATUS2.intr1 != 4) {
                        data_0207f0e4.index++;
                        RAW_STATUS2.intr1 = 4;
                        if (!RTCi_WriteRawStatus2Async()) {
                            data_0207f0e4.index = 0;
                            result = 3;
                        }
                    }
                } else {
                    if (RAW_STATUS2.intr1 != 0) {
                        data_0207f0e4.index++;
                        RAW_STATUS2.intr1 = 0;
                        if (!RTCi_WriteRawStatus2Async()) {
                            data_0207f0e4.index = 0;
                            result = 3;
                        }
                    }
                }
            } else {
                data_0207f0e4.index = 0;
            }
            break;
        case 10:
            if (data_0207f0e4.index == 0) {
                if (*(u32 *)data_0207f0e4.buffer == TRUE) {
                    if (!RAW_STATUS2.intr2) {
                        data_0207f0e4.index++;
                        RAW_STATUS2.intr2 = 1;
                        if (!RTCi_WriteRawStatus2Async()) {
                            data_0207f0e4.index = 0;
                            result = 3;
                        }
                    }
                } else {
                    if (RAW_STATUS2.intr2) {
                        data_0207f0e4.index++;
                        RAW_STATUS2.intr2 = 0;
                        if (!RTCi_WriteRawStatus2Async()) {
                            data_0207f0e4.index = 0;
                            result = 3;
                        }
                    }
                }
            } else {
                data_0207f0e4.index = 0;
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            break;
        default:
            data_0207f0e4.index = 0;
            result = 4;
            break;
        }
    } else {
        data_0207f0e4.index = 0;
        switch (status) {
        case 1:
            result = 4;
            break;
        case 2:
            result = 5;
            break;
        case 3:
            result = 1;
            break;
        case 4:
        default:
            result = 6;
            break;
        }
    }

    if (data_0207f0e4.index == 0) {
        if (data_0207f0e4.lock != 0) {
            data_0207f0e4.lock = 0;
        }
        cb = data_0207f0e4.callback;
        if (cb != NULL) {
            data_0207f0e4.callback = NULL;
            cb(result, data_0207f0e4.callbackArg);
        }
    }
}
