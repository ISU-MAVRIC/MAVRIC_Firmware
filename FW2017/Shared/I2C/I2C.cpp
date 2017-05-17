/*
 * I2C.cpp
 *
 *  Created on: May 29, 2016
 *      Author: James
 */

#include "msp.h"
#include "system_msp432p401r.h"

void InitI2C(EUSCI_B_Type* target)
{
  // Put in reset mode to edit
  target->CTLW0 |= UCSWRST;
  
  
  // master I2C mode, retain SWRST
  target->CTLW0 = UCMST | UCMODE_3 | UCSWRST | UCSSEL__SMCLK;
  
  // default baud rate. ~1 MHz, conforms to I2C fast mode plus standard
  //target->BRW = 50;
  target->BRW = 30 * 4;
  
  //default to manual Stop condition
  target->TBCNT = 0;
  
  // no reception addresses
  target->I2COA0 = 0;
  target->I2COA1 = 0;
  target->I2COA2 = 0;
  target->I2COA3 = 0;
  
  // ADDMSK feature disabled
  target->ADDMASK = 0x3FF;
  // no interrupts, use polling to send multiple bytes.
  target->IE = 0;
  // enable the module.
  target->CTLW0 &= ~UCSWRST;
}

void ReadI2C(EUSCI_B_Type* module, unsigned char slaveAddress, unsigned char* rxBuffer, int count, bool sendStop = true)
{
  module->CTLW0 &= ~UCTR;
  module->I2CSA = slaveAddress;
  
  module->CTLW0 |= UCTXSTT;
  while ((module->CTLW0 & UCTXSTT) != 0);
  
  int i;
  for (i = 0; i < count; i++)
  {
    while ((module->IFG & UCRXIFG) == 0);
    rxBuffer[i] = module->RXBUF;
  }

  if (sendStop)
  {
    module->CTLW0 |= UCTXSTP;
    while ((module->CTLW0 & UCTXSTP) != 0);
  }
}

void WriteI2C(EUSCI_B_Type* module, unsigned char slaveAddress, unsigned char* txBuffer, int count, bool sendStop = true)
{
  module->CTLW0 |= UCTR;
  module->I2CSA = slaveAddress;
  
  module->CTLW0 |= UCTXSTT;
  while ((module->CTLW0 & UCTXSTT) != 0);
  int i;
  for (i = 0; i < count; i++)
  {
    module->TXBUF = txBuffer[i];
    while ((module->IFG & UCTXIFG) == 0);
  }
  if (sendStop)
  {
    module->CTLW0 |= UCTXSTP;
    while ((module->CTLW0 & UCTXSTP) != 0);
  }
}
