#include "i2c.h"

#define GPIOBEN (1U << 1)
#define I2C1EN  (1U << 21)
#define OTYPER_OT8 (1 << 8)
#define OTYPER_OT9 (1 << 9)
#define SWRST   (1 << 15)
#define FREQ    (1U << 4)
#define CR1_PE  (1 << 0)
#define SD_MODE_MAX_RISE_TIME (17 << 0)

#define SR2_BUSY  (1U << 1)
#define CR1_START (1 << 8)
#define CR1_ACK   (1 << 10)
#define SR1_SB    (1  << 0)
#define SR1_ADDR  (1  << 1)
#define SR1_TxE   (1  << 7)
#define CR1_STOP  (1U << 9)
#define SR1_RXNE  (1  << 6)
#define SR1_BTF   (1  << 2)
#define CR1_STOP  (1U << 9)



/***********Steps I2C Configurations**************
 * Enable the I2C and GPIO Clock
 * Configure the I2C Pins for the alternate function
 * a. Select Alternative function in the MODE Register
 * b. Select Open drain output
 * c. Select High Speed for the PINs
 * d. Select pull ups for both the pins
 * e. Configure the Alternate function in the AFR Register
 * Reset the I2C
 * Program the peripheral input clock in I2C_CR2 Register in order to generate correct timing
 * Configure the clock control register
 * Configure the rise time register
 * Program the I2C_CR1 register to enable the peripheral
 * */


void I2C_init(void){
	// Enable clock for the I2C
	RCC -> APB1ENR |= I2C1EN;

	//Enable the GPIOB Clock
	RCC -> AHB1ENR |= GPIOBEN;

	//Enables the Alternate function for the GPIO
	//MODER8
	GPIOB -> MODER &=~(1U << 16);
	GPIOB -> MODER |=  (1U << 17);

	//MODER9
	GPIOB -> MODER &=~(1U << 18);
	GPIOB -> MODER |=  (1U << 19);

	//Configuring the output type  -> Open Drain
	GPIOB -> OTYPER |= OTYPER_OT8;
	GPIOB -> OTYPER |= OTYPER_OT9;

	//Configuring the high-speed   -> High Speed
	//OSPEEDR8
	GPIOB-> OSPEEDR |= ((1U << 17) | (1U << 16));
	//OSPEEDR9
	GPIOB-> OSPEEDR |= ((1U << 19) | (1U << 18));

	//Selecting  the pull-up in the GPIO
	//It’s better to use external pull up registers while using I2C,
	//but just for Simplicity internal pull-up resistors is used

	//PUPDR8
	GPIOB-> PUPDR |=  (1U << 16);
	GPIOB-> PUPDR &=~(1U << 17);
	//PUPDR9
	GPIOB-> PUPDR |=  (1U << 18);
	GPIOB-> PUPDR &=~(1U << 19);

	//Configure the Alternate Function in the AFR Register
	//the pins were set in the alternate functions mode,
	//but that was not defined what those functions should be.
	GPIOB-> AFR[1] &=~(1U << 0);
	GPIOB-> AFR[1] &=~(1U << 1);
	GPIOB-> AFR[1] |= (1U << 2);
	GPIOB-> AFR[1] &=~(1U << 3);

	GPIOB-> AFR[1] &=~(1U << 4);
	GPIOB-> AFR[1] &=~(1U << 5);
	GPIOB-> AFR[1] |= (1U << 6);
	GPIOB-> AFR[1] &=~(1U << 7);


	//Reset the I2C -> Make sure I2C lines are released
	I2C1 -> CR1 |= SWRST;
	I2C1 -> CR1 &=~SWRST;


	//Program the peripheral input clock in I2C_CR2 Register in order to generate correct timing
	I2C1 -> CR2 |= (16<<0);   //PCLK1 frequency in MHz


	//T_high  = CCR * TPCLK1 ; T_high  = t_r(SCL) + t_w(SCLH)
	//CCR = (t_r(SCL) + t_w(SCLH))/TPCLK1 = (1000ns + 4000ns)/(62.5ns) = 80
	I2C1  -> CCR |= (80<<0);  //CCR Value

	// Configure the rise time register
	// TRISE = (T_r(SCL)/T(PCLK1))+1 = (1000 / 62.5) + 1 = 17
	I2C1 -> TRISE |= SD_MODE_MAX_RISE_TIME;

	//Enable the peripheral
	I2C1 -> CR1 |= CR1_PE;
}

void I2C_Start(void){

	//Acknowledge Enabled
	I2C1 -> CR1 |= CR1_ACK;

	//Generate the Start condition
	I2C1 -> CR1 |= CR1_START;

	//wait until the start condition is generated
	while(!((I2C1 -> SR1) & (SR1_SB))){}
}

void I2C_Address(uint8_t address){

	//Transmit address
	I2C1 -> DR = address;

	//wait until the address transmission is done
	while (!((I2C1 -> SR1) & (SR1_ADDR)));

	//Read SR1 and SR2 to clear the ADDR Bit
	uint16_t temp = ((I2C1 -> SR1) | (I2C1 -> SR2));
}


void I2C_Write(uint8_t data){

	//wait until the last address transmission is done
    while (!((I2C1 -> SR1) & (SR1_TxE)));

    //Transmit address + read
	I2C1 -> DR = data;

	//Wait until the data byte transfer is completed
    while (!((I2C1 -> SR1) & (SR1_BTF)));

}

void I2C_WriteMulti(uint8_t *data, uint8_t size){

	//wait until the last address transmission is done
    while (!((I2C1 -> SR1) & (SR1_TxE))){}

    while (size){
      while (!((I2C1 -> SR1) & (SR1_TxE)));
      //Transmit data
  	  I2C1 -> DR = (volatile uint32_t) *data++;
  	  size--;
    }

	//Wait until the data byte transfer is completed
    while (!((I2C1 -> SR1) & (SR1_BTF))){}

}


void I2C_Stop(void){
    //Generate the stop bit
	I2C1 -> CR1 |= CR1_STOP;
}


