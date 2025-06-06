#include "pci.h"
#include <io/io.h>

uint32_t pci_read_config(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    // offset은 반드시 4의 배수
    if (offset & 0x3) {
        printk("[PCI] Error: Offset 0x%x is not 4-byte aligned\n", offset);
        return 0xFFFFFFFF;
    }

    if (slot >= 32 || function >= 8) {
        printk("[PCI] Error: Invalid slot (%d) or function (%d)\n", slot, function);
        return 0xFFFFFFFF;
    }

    uint32_t address = (1U << 31) | ((uint32_t)bus << 16) | ((uint32_t)slot << 11) |
                       ((uint32_t)function << 8) | (offset & 0xFC);
    
    outl(PCI_CONFIG_ADDRESS, address);
    uint32_t value = inl(PCI_CONFIG_DATA);

    printk("[PCI] Read 0x%x from CONFIG[bus=%d, slot=%d, func=%d, offset=0x%x]\n",
           value, bus, slot, function, offset);

    return value;
}



int pci_write_config(uint8_t bus, uint8_t function, uint8_t slot, uint8_t offset, uint32_t data) {
    // offset은 반드시 4의 배수
    if (offset & 0x3) {
        printk("[PCI] Error: Offset 0x%x is not 4-byte aligned\n", offset);
        return -1;
    }

    if (slot >= 32 || function >= 8) {
        printk("[PCI] Error: Invalid slot (%d) or function (%d)\n", slot, function);
        return -1;
    }



    uint32_t address = (1 << 31) | (bus << 16) | (slot << 11) |
                       (function << 8) | (offset & 0xFC);

    outl(PCI_CONFIG_ADDRESS, address);
    outl(PCI_CONFIG_DATA, data);

    printk(
        "[PCI] Write 0x%lx to CONFIG[bus=%d, slot=%d, func=%d, offset=0x%x]\n",
           data, bus, slot, function, offset
    );
}

int pci_init() {
    printk("[PCI] Initializing PCI Subsystem...\n");

    // pci 접근 테스트
    uint16_t vendor = pci_read_config(0, 0, 0, 0x00);
    if (vendor == 0xFFFF) {
        printk("[PCI] No device found at 00:00. no PCI bus?\n");
        return -1;
    }

    // Master PIC: 0x21 포트, IRQ1만 허용 (bit 1 = 0)
    outb(0x21, 0xFD);  // 11111101
    outb(0xA1, 0xFF);  // 슬레이브는 모두 마스크

    printk("[PCI] Bus 0, Device 0, Func 0 — Vendor ID = %x\n", vendor);
    return 0;
}