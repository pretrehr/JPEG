#ifndef _QTABLES_H_
#define _QTABLES_H_

#include <stdint.h>

/* Tables de "The GIMP" (cf sujet). */
static uint8_t compressed_Y_table[] = {
    0x05, 0x03, 0x03, 0x05, 0x07, 0x0c, 0x0f, 0x12,
    0x04, 0x04, 0x04, 0x06, 0x08, 0x11, 0x12, 0x11,
    0x04, 0x04, 0x05, 0x07, 0x0c, 0x11, 0x15, 0x11,
    0x04, 0x05, 0x07, 0x09, 0x0f, 0x1a, 0x18, 0x13,
    0x05, 0x07, 0x0b, 0x11, 0x14, 0x21, 0x1f, 0x17,
    0x07, 0x0b, 0x11, 0x13, 0x18, 0x1f, 0x22, 0x1c,
    0x0f, 0x13, 0x17, 0x1a, 0x1f, 0x24, 0x24, 0x1e,
    0x16, 0x1c, 0x1d, 0x1d, 0x22, 0x1e, 0x1f, 0x1e
};

static uint8_t compressed_CbCr_table[] = {
    0x05, 0x05, 0x07, 0x0e, 0x1e, 0x1e, 0x1e, 0x1e,
    0x05, 0x06, 0x08, 0x14, 0x1e, 0x1e, 0x1e, 0x1e,
    0x07, 0x08, 0x11, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e,
    0x0e, 0x14, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e,
    0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e,
    0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e,
    0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e,
    0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e
};

#endif /* _QTABLES_H_ */
