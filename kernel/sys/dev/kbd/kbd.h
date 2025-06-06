#pragma once
#include <stdint.h>
#include <io/io.h>
#include <subr_printk.h>

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64
#define KBD_CMD_PORT 0x64
#define KBD_ENABLE_CMD 0xAE

int kbd_init();
int kbd_handler();