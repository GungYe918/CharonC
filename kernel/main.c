#include <stdint.h>
#include "Libraries/FileFormat/elf/elf.h"
void KernelMain(
    uint64_t frame_buffer_base,
    uint64_t frame_buffer_size
) {
    uint8_t* frame_buffer = (uint8_t*)frame_buffer_base;
    for (uint64_t i = 0; i < frame_buffer_size; i++) {
        frame_buffer[i] = (uint8_t)(i % 256);
    }

    for (;;) {
        __asm__("hlt");
    }
}
