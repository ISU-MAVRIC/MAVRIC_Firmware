#ifndef _I2C_H_
#define _I2C_H_

// Sets up the I2C port in master mode, with 7-bit addresses and defaults the baud rate, etc.
void InitI2C(EUSCI_B_Type* target);
// Reads the given number of bytes from the selected device on the I2C bus.
void ReadI2C(EUSCI_B_Type* module, unsigned char slaveAddress, unsigned char* rxBuffer, int count, bool sendStop = true);
// Writes the specified bytes to the selected device on the I2C bus.
void WriteI2C(EUSCI_B_Type* module, unsigned char slaveAddress, unsigned char* txBuffer, int count, bool sendStop = true);

#endif
