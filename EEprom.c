#include <stdint.h>   
#include <pic32mx.h>  
#include "mipslab.h" 
#include <string.h>

#define EEPROM_EMULATION_START 0x9D007000
#define EEPROM_EMULATION_SIZE  0x1000

void writeEEPROM(uint32_t address, uint32_t data) {
    // Example function body
    // Erase Flash block
    // Write data to Flash

    NVMCON = 0x4004; // Enable and configure for erase operation 
//Wait(delay); // Delay for 6 μs for LVDstartup
NVMKEY = 0xAA996655; 
NVMKEY = 0x556699AA; 
NVMCONSET = 0x8000; // Initiate operation 
while(NVMCON==1); // Wait WR for current operation to complete
}

uint32_t readEEPROM(uint32_t address) {
    // Directly return the data from Flash
    return *((uint32_t*)address);
}

