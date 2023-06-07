#include "stm32f4xx.h"
#include "i2c.h"

#define I2C2EN  (1U << 21)
#define SWRST   (1U << 15)
#define FREQ    (1U << 4)
#define SD_MODE_MAX_RISE_TIME (17 << 0)
#define F_S     (1U << 15)
#define PE      (1U << 0)

#define SR2_BUSY  (1U << 1)
#define CR1_START (1U << 8)
#define SR1_SB    (1U << 0)
#define SR1_ADDR  (1U << 1)
#define SR1_TxE   (1U << 7)
#define CR1_ACK   (1U << 10)
#define CR1_STOP  (1U << 9)
#define SR1_RXNE  (1U << 6)

uint16_t tmp;

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


void I2C1_init(void){

		//Enable the GPIOB Clock
		RCC -> AHB1ENR |= (1U << 1);

	    //Enables the Alternate function for the GPIO
		GPIOB -> MODER |= (1U << 17);
		GPIOB -> MODER &= ~(1U << 16);

		GPIOB -> MODER |= (1U << 19);
		GPIOB -> MODER &= ~(1U << 18);

		//Configuring the output type  -> Open Drain
		GPIOB -> OTYPER |= (1U << 8);
		GPIOB -> OTYPER |= (1U << 9);

		//Configuring the high-speed   -> High Speed
		GPIOB-> OSPEEDR |= ((1U << 17) | (1U << 16));
		GPIOB-> OSPEEDR |= ((1U << 19) | (1U << 18));

		//Selecting  the pull-up in the GPIO
		GPIOB-> PUPDR |= (1U << 16);
		GPIOB-> PUPDR &= ~(1U << 17);

		GPIOB-> PUPDR |= (1U << 18);
		GPIOB-> PUPDR &= ~(1U << 19);

		//Configure the Alternate Function in the AFR Register
		GPIOB-> AFR[1] |= ((4 << 0) | (4 << 4));

		// Enable clock for the I2C
		RCC -> APB1ENR |= I2C2EN;

		//Reset the I2C -> Make sure I2C lines are released
	    I2C1 -> CR1 |= SWRST;
	    I2C1 -> CR1 &= ~SWRST;


	    //Program the peripheral input clock in I2C_CR2 Register in order to generate correct timing
	    I2C1 -> CR2 |= FREQ;   //PCLK1 frequency in MHz

	    //Configuring the clock control register
	    I2C1  -> CCR &= ~F_S;     //I2C Standard mode

	    //T_high  = CCR * TPCLK1 ; T_high  = t_r(SCL) + t_w(SCLH)
	    //CCR = (t_r(SCL) + t_w(SCLH))/TPCLK1 = (1000ns + 4000ns)/(62.5ns) = 80
	    I2C1  -> CCR |= ( 80<<0 );  //CCR Value

	    // Configure the rise time register
	    // TRISE = (T_r(SCL)/T(PCLK1))+1 = (1000 / 62.5) + 1 = 17
	    I2C1 -> TRISE |= SD_MODE_MAX_RISE_TIME;

	    //Enable the peripheral
	    I2C1 -> CR2 |= PE;
}

void I2C1_byteRead(char saddr, char maddr, char* data){
		//wait until the bus is not busy
		while((I2C1 -> SR2) & (SR2_BUSY)){}

		//Generate the Start condition
		I2C1 -> CR1 |= CR1_START;

		//wait until the start condition is generated
		while(!((I2C1 -> SR1) & (SR1_SB))){}

		//Transmit address + write
		I2C1 -> DR = saddr << 1 ;

		//wait until the address transmission is done
		while (!((I2C1 -> SR1) & (SR1_ADDR))){}

		tmp = I2C1 -> SR2;

		//Transmit memory address
		I2C1 -> DR = maddr ;

		//wait until the address transmission is done
		 while (!((I2C1 -> SR1) & (SR1_TxE))){}


		//Generate the restart condition
		 I2C1 -> CR1 |= CR1_START;

		//wait until the start condition is generated
		while(!((I2C1 -> SR1) & (SR1_SB))){}

		//Transmit address  + read
		I2C1 -> DR = saddr << 1 | 1;

		//wait until the address transmission is done
		while (!((I2C1 -> SR1) & (SR1_ADDR))){}

		I2C1 -> CR1 &= ~CR1_ACK;

		tmp = I2C1 -> SR2;

		I2C1 -> CR1 |= CR1_STOP;

		//wait until the data transmission is done

		while (!((I2C1 -> SR1) & (SR1_RXNE))){}

		*data++ = I2C1->DR;
}

