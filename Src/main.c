#include "i2c.h"
#include "stdint.h"

int main(void)
{
	//Initializes the I2C Pins
	I2C_init();
	while(1)
	{
		//Generate Start Bit
		I2C_Start();
		//Send the slave address
		I2C_Address(0x3F);
		//Write the data
		I2C_Write(0x02);
		//Generate stop bit
		I2C_Stop();
	}
}
