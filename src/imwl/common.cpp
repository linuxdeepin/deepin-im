#include "common.h"


uint32_t nextSerial() {
    static uint32_t current = 1;

    return current++;
}
