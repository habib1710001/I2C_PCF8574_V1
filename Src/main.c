#include "i2c.h"
#include "stdint.h"

void dummy_delay(uint32_t interation){
   for(volatile int k= 0; k <= interation; k++){

   }
}

int main(void)
{
	//Initializes the I2C Pins
 	I2C_init();
	while(1)
	{
		for (uint8_t i = 0; i < 8 ; i++){
		//Generate Start Bit
		I2C_Start();
		//Send the slave address
		I2C_Address(0x3f);
		//Write the data
		I2C_Write(1 << i);
		//Generate stop bit
		I2C_Stop();
		if (i==1 ) i+=2;
		dummy_delay(100000);
		}
	}
}
