#pragma once
#include <stdint.h>
#include <subr_printk.h>

#define PCI_CONFIG_ADDRESS  0xCF8
#define PCI_CONFIG_DATA     0xCFC

// pci 초기화/탐색 함수
int pci_init();
int pci_scan_bus();
int pci_enum_devices();


/**
 * pci R+W
 * 
 * pci_read_config: pci에서 값을 읽어옴
 * pci_write_config: pci에 적절한 값을 작성함
 */

uint32_t pci_read_config(uint8_t bus, uint8_t function, uint8_t slot, uint8_t offset);
int pci_write_config(uint8_t bus, uint8_t function, uint8_t slot, uint8_t offset, uint32_t data);


// pci 장치 검색 함수
int pci_find_device(uint16_t vendor, uint16_t device);
int pci_get_device_by_class(uint8_t classcode);


