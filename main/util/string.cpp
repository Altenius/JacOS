#include "string.h"
#include "memory.h"

size_t strlen(const char *string) {
    size_t len = 0;
    for (; *string != 0; string++, len++);
    return len;
}

void strcat(char *dst, const char *str2) {
    size_t len = strlen(dst);
    size_t slen = strlen(str2);
    memcpy(dst + len, str2, slen + 1); // copies the null terminating 0
}

char istr[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const char *uitos(uint32_t num) {
    char *ptr = istr + 10;
    if (num == 0) {
        return "0";
    }
    while (num != 0) {
        ptr--;
        *ptr = (num % 10) + 48;
        num /= 10;
    }
    return ptr;
}

char qstr[21] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const char *uqtos(uint64_t num) {
    char *ptr = qstr + 10;
    if (num == 0) {
        return "0";
    }
    while (num != 0) {
        ptr--;
        *ptr = (num % 10) + 48;
        num /= 10;
    }
    return ptr;
}

char ihstr[] = "00000000";

const char *uitohs(uint32_t num) {
    for (char *ptr = ihstr + 7; ptr >= ihstr; ptr--) {
        char c = num & 0xF;
        if (c < 10) {
            c += 48;
        } else {
            c += 55; // 65 - 10
        }
        *ptr = c;
        num >>= 4;
    }

    return ihstr;
}

char qhstr[] = "0000000000000000";

const char *uqtohs(uint64_t num) {
    for (char *ptr = qhstr + 15; ptr >= qhstr; ptr--) {
        char c = num & 0xF;
        if (c < 10) {
            c += 48;
        } else {
            c += 55; // 65 - 10
        }
        *ptr = c;
        num >>= 4;
    }

    return qhstr;
}
