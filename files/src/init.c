
#include "init.h"





void init_all(void){


	/*inicializacion motores*/
	Iniciar_puertos();
	//portTICK_RATE_MS;

}

void Iniciar_Puertos(void)
{
	Chip_SCU_PinMux(1,0,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[4], SW1 */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, 0, 4);	// input

	Chip_SCU_PinMux(1,1,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[8], SW2 */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, 0, 8);	// input

	Chip_SCU_PinMux(1,2,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[9], SW3 */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, 0, 9);	// input

	Chip_SCU_PinMux(1,6,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO1[9], SW4 */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, 1, 9);	// input

	Chip_SCU_PinMux(6,1,MD_PUP|MD_EZI,FUNC0); 		/* GPIO3[0], GPIO 0 */
	Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, 3, 0, 1);	// output

//	Chip_SCU_PinMux(2,10,MD_PUP|MD_EZI,FUNC0); /* GPIO0[14], LED1 */
//	Chip_GPIO_SetDir(LPC_GPIO_PORT, 0, 14, 1);
//	Chip_GPIO_ClearValue(LPC_GPIO_PORT, 0,14);
}
