#include "stm32f4xx.h"

#define GPIOAEN (1U<<0)
#define PIN5   (1U<<5)
#define LED_PIN PIN5



#define SWRST (1U<<15)
#define FREQ  (16<<0)
#define F_S   (1U << 15)
#define PE    (1U << 0)

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

int main(void){

	/**********Pin COnfiguration*************/
	// Enable clock for the I2C
	RCC -> APB1ENR |= (1U << 21);

	//Enable the GPIOB Clock
	RCC -> AHB1ENR |= (1U << 1);

    //Enables the Alternate function for the GPIO
	GPIOB -> MODER |= ((1U << 17) | (1U << 19)) ;

	//Configuring the output type  -> Open Drain
	GPIOB -> OTYPER |= ((1U << 8) | (1U << 9));

	//Configuring the high-speed   -> High Speed
	GPIOB-> OSPEEDR |= ((1U << 17) | (1U << 16));
	GPIOB-> OSPEEDR |= ((1U << 19) | (1U << 18));

	//Selecting  the pull-up in the GPIO
	GPIOB-> PUPDR |= ((1U << 18) | (1U << 16));

	//Configure the Alternate Function in the AFR Register
	GPIOB-> AFR[1] |= ((4 << 0) | (4 << 4));

	//Reset the I2C -> Make sure I2C lines are released
    I2C1 -> CR1 |= SWRST;
    I2C1 -> CR1 &= ~SWRST;

    //Program the peripheral input clock in I2C_CR2 Register
    //in order to generate correct timing
    I2C1 -> CR2 |= FREQ;   //PCLK1 frequency in MHz

    //Configuring the clock control register
    I2C1  -> CCR &= ~F_S;     //I2C Standard mode

    //T_high  = CCR * TPCLK1
    //T_high  = t_r(SCL) + t_w(SCLH)
    //CCR = (t_r(SCL) + t_w(SCLH))/TPCLK1 = (1000ns + 4000ns)/(62.5ns) = 80
    I2C1  -> CCR |= (80<<0);  //CCR Value

    // Configure the rise time register
    // TRISE = (T_r(SCL)/T(PCLK1))+1 = (1000 / 62.5) + 1 = 17
    I2C1 -> TRISE |= (17 << 0);

    //Enable the peripheral
    I2C1 -> CR2  |= PE;
	while(1)
	{

	}
}
