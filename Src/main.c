#include "stm32f4xx.h"
#include "i2c.h"

int main(void){

	//Generate Start Bit
    I2C1_Start();

    //Send the slave address
    I2C_Address(0x4E);

    //Write the data
    I2C_Write(0x01);

    //Generate stop bit
    I2C_Stop();
	while(1)
	{

	}
}
