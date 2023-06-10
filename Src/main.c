#include "stm32f4xx.h"
#include "i2c.h"


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
	    I2C_Write(0x03);
	    //Generate stop bit
	    I2C_Stop();

	}
}
