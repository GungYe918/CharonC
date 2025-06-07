#pragma once


// key map table
static const char scancode_table[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',     // 0x00 - 0x09
    '9', '0', '-', '=', '\b',                          // 0x0A - 0x0E
    '\t',                                              // 0x0F - Tab
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',   // 0x10 - 0x19
    '[', ']', '\n',                                    // 0x1A - 0x1C (Enter)
    0,                                                 // 0x1D - Ctrl
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',        // 0x1E - 0x26
    ';', '\'', '`',                                    // 0x27 - 0x29
    0,                                                 // 0x2A - Left Shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/' , // 0x2B - 0x35
    0,                                                 // 0x36 - Right Shift
    '*',                                               // 0x37 - Keypad *
    0,                                                 // 0x38 - Alt
    ' ',                                               // 0x39 - Space
    0,                                                 // 0x3A - Caps Lock
    0,                                                 // 0x3B - F1
    0, 0, 0, 0, 0, 0, 0, 0, 0,                         // 0x3C - F10
    0,                                                 // 0x45 - Num Lock
    0,                                                 // 0x46 - Scroll Lock
    '7', '8', '9', '-',                                // 0x47 - Keypad
    '4', '5', '6', '+',                                // 0x4B
    '1', '2', '3', '0', '.',                           // 0x4F
    0, 0,                                              // 0x54~, unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                       // reserved
    0, 0, 0, 0, 0, 0, 0, 0                             // reserved
};

static const char scancode_table_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',    // 0x00 - 0x09
    '(', ')', '_', '+', '\b',                          // 0x0A - 0x0E
    '\t',                                              // 0x0F - Tab
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',   // 0x10 - 0x19
    '{', '}', '\n',                                    // 0x1A - 0x1C
    0,                                                 // 0x1D - Ctrl
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',        // 0x1E - 0x26
    ':', '\"', '~',                                    // 0x27 - 0x29
    0,                                                 // 0x2A - Left Shift
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', // 0x2B - 0x35
    0,                                                 // 0x36 - Right Shift
    '*',                                               // 0x37 - Keypad *
    0,                                                 // 0x38 - Alt
    ' ',                                               // 0x39 - Space
    0,                                                 // 0x3A - Caps Lock
    0,                                                 // 0x3B - F1
    0, 0, 0, 0, 0, 0, 0, 0, 0,                         // 0x3C - F10
    0,                                                 // 0x45 - Num Lock
    0,                                                 // 0x46 - Scroll Lock
    '7', '8', '9', '-',                                // 0x47
    '4', '5', '6', '+',                                // 0x4B
    '1', '2', '3', '0', '.',                           // 0x4F
    0, 0,                                              // 0x54~, unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                       // reserved
    0, 0, 0, 0, 0, 0, 0, 0                             // reserved
};