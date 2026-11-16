#include <nitro/types.h>


typedef struct {
    u8 justify;
    u8 sign;
    u8 precSet;
    u8 alt;
    u8 argOpt;
    u8 conv;
    s32 width;
    s32 prec;
} UnkPrintFmt_0202a3e8;

typedef void *(*UnkWriteProc_0202a3e8)(void *arg, const u8 *str, u32 len);

extern u16 data_02077f20[];
extern u8 data_02077f24[];

#define VA_ARG(ap, type) (*(type *)((ap += sizeof(type)) - sizeof(type)))

const u8 *strchr(const u8 *s, int c);
u32 strlen(const u8 *s);
const u8 *parse_format(const u8 *str, char **arg, UnkPrintFmt_0202a3e8 *out);
u8 *long2str(s32 num, u8 *buf, UnkPrintFmt_0202a3e8 fmt);
u8 *longlong2str(s64 num, u8 *buf, UnkPrintFmt_0202a3e8 fmt);
u8 *float2str(double num, u8 *buf, UnkPrintFmt_0202a3e8 fmt);
u8 *double2hex(double num, u8 *buf, UnkPrintFmt_0202a3e8 fmt);
int wcstombs(u8 *dst, const u16 *src, u32 n);
void *memchr(const void *p, int c, u32 n);

int __pformatter(UnkWriteProc_0202a3e8 WriteProc, void *WriteProcArg, const u8 *format_str, char *arg)
{
    int num_chars;
    int chars_written;
    int field_width;
    const u8 *format_ptr;
    const u8 *curr_format;
    UnkPrintFmt_0202a3e8 format;
    s32 long_num;
    s64 long_long_num;
    long double long_double_num;
    u8 buff[512];
    u8 *buff_ptr;
    u8 *string_end;
    u8 fill_char = ' ';

    format_ptr = format_str;
    chars_written = 0;

    while (*format_ptr) {
        if (!(curr_format = strchr(format_ptr, '%'))) {
            num_chars = strlen(format_ptr);
            chars_written += num_chars;
            if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, num_chars))
                return -1;
            break;
        }

        num_chars = curr_format - format_ptr;
        chars_written += num_chars;
        if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, num_chars))
            return -1;

        format_ptr = curr_format;
        format_ptr = parse_format(format_ptr, &arg, &format);

        switch (format.conv) {
        case 'd':
        case 'i':
            if (format.argOpt == 3)
                long_num = VA_ARG(arg, s32);
            else if (format.argOpt == 7)
                long_long_num = VA_ARG(arg, s64);
            else if (format.argOpt == 4)
                long_num = VA_ARG(arg, s64);
            else if (format.argOpt == 5)
                long_num = VA_ARG(arg, u32);
            else if (format.argOpt == 6)
                long_num = VA_ARG(arg, s32);
            else
                long_num = VA_ARG(arg, int);

            if (format.argOpt == 2)
                long_num = (s16)long_num;
            if (format.argOpt == 1)
                long_num = (s8)long_num;

            if (format.argOpt == 7) {
                if (!(buff_ptr = longlong2str(long_long_num, buff + 512, format)))
                    goto conversion_error;
            } else {
                if (!(buff_ptr = long2str(long_num, buff + 512, format)))
                    goto conversion_error;
            }
            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 'o':
        case 'u':
        case 'x':
        case 'X':
            if (format.argOpt == 3)
                long_num = VA_ARG(arg, u32);
            else if (format.argOpt == 7)
                long_long_num = VA_ARG(arg, u64);
            else if (format.argOpt == 4)
                long_num = VA_ARG(arg, u64);
            else if (format.argOpt == 5)
                long_num = VA_ARG(arg, u32);
            else if (format.argOpt == 6)
                long_num = VA_ARG(arg, s32);
            else
                long_num = VA_ARG(arg, unsigned int);

            if (format.argOpt == 2)
                long_num = (u16)long_num;
            if (format.argOpt == 1)
                long_num = (u8)long_num;

            if (format.argOpt == 7) {
                if (!(buff_ptr = longlong2str(long_long_num, buff + 512, format)))
                    goto conversion_error;
            } else {
                if (!(buff_ptr = long2str(long_num, buff + 512, format)))
                    goto conversion_error;
            }
            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if (format.argOpt == 8)
                long_double_num = VA_ARG(arg, long double);
            else
                long_double_num = VA_ARG(arg, double);

            if (!(buff_ptr = float2str(long_double_num, buff + 512, format)))
                goto conversion_error;
            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 'a':
        case 'A':
            if (format.argOpt == 8)
                long_double_num = VA_ARG(arg, long double);
            else
                long_double_num = VA_ARG(arg, double);

            if (!(buff_ptr = double2hex(long_double_num, buff + 512, format)))
                goto conversion_error;
            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 's':
            if (format.argOpt == 9) {
                u16 *wcs_ptr = VA_ARG(arg, u16 *);
                if (wcs_ptr == NULL)
                    wcs_ptr = data_02077f20;
                if ((num_chars = wcstombs(buff, wcs_ptr, sizeof(buff))) < 0)
                    goto conversion_error;
                buff_ptr = &buff[0];
            } else {
                buff_ptr = VA_ARG(arg, u8 *);
            }
            if (buff_ptr == NULL)
                buff_ptr = data_02077f24;
            if (format.alt) {
                num_chars = (u8)*buff_ptr++;
                if (format.precSet && num_chars > format.prec)
                    num_chars = format.prec;
            } else if (format.precSet) {
                num_chars = format.prec;
                if ((string_end = (u8 *)memchr(buff_ptr, 0, num_chars)) != 0)
                    num_chars = string_end - buff_ptr;
            } else {
                num_chars = strlen(buff_ptr);
            }
            break;

        case 'n':
            buff_ptr = VA_ARG(arg, u8 *);
            switch (format.argOpt) {
            case 0:
                *(int *)buff_ptr = chars_written;
                break;
            case 2:
                *(s16 *)buff_ptr = chars_written;
                break;
            case 3:
                *(s32 *)buff_ptr = chars_written;
                break;
            case 7:
                *(s64 *)buff_ptr = chars_written;
                break;
            }
            continue;

        case 'c':
            buff_ptr = buff;
            *buff_ptr = VA_ARG(arg, int);
            num_chars = 1;
            break;

        case '%':
            buff_ptr = buff;
            *buff_ptr = '%';
            num_chars = 1;
            break;

        case 0xff: case 'h':
        default:
        conversion_error:
            num_chars = strlen(curr_format);
            chars_written += num_chars;
            if (num_chars && !(*WriteProc)(WriteProcArg, curr_format, num_chars))
                return -1;
            return chars_written;
        }

        field_width = num_chars;

        if (format.justify != 0) {
            fill_char = (format.justify == 2) ? '0' : ' ';
            if (((*buff_ptr == '+') || (*buff_ptr == '-') || (*buff_ptr == ' ')) && (fill_char == '0')) {
                if ((*WriteProc)(WriteProcArg, buff_ptr, 1) == 0)
                    return -1;
                ++buff_ptr;
                num_chars--;
            }
            while (field_width < format.width) {
                if ((*WriteProc)(WriteProcArg, &fill_char, 1) == 0)
                    return -1;
                ++field_width;
            }
        }

        if (num_chars && !(*WriteProc)(WriteProcArg, buff_ptr, num_chars))
            return -1;

        if (format.justify == 0) {
            while (field_width < format.width) {
                u8 blank = ' ';
                if ((*WriteProc)(WriteProcArg, &blank, 1) == 0)
                    return -1;
                ++field_width;
            }
        }

        chars_written += field_width;
    }

    return chars_written;
}

