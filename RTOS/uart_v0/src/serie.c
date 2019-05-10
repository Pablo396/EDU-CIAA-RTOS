/* Copyright 2015, Pablo Ridolfi
 * All rights reserved.
 *
 * This file is part of lpc1769_template.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** @brief Blinky using FreeRTOS.
 *
 *
 * NOTE: It's interesting to check behavior differences between standard and
 * tickless mode. Set @ref configUSE_TICKLESS_IDLE to 1, increment a counter
 * in @ref vApplicationTickHook and print the counter value every second
 * inside a task. In standard mode the counter will have a value around 1000.
 * In tickless mode, it will be around 25.
 *
 */

/** \addtogroup rtos_blink FreeRTOS blink example
 ** @{ */

/*==================[inclusions]=============================================*/



#include "init.h"
//#include "f.h"
//#include "main.h"

#include "chip.h"
/*==================[macros and definitions]=================================*/

typedef struct {
	uint8_t port;
	uint8_t pin;
} io_puerto_t;


bool_t flag_start_stop = FALSE;
bool_t flag_reset = FALSE;
bool_t flag_up_down = TRUE;


uint8_t contador=0;

//static const io_puerto_t gpioBOTTONBits[] = {{0,4},{0,8},{0,9},{1,9}};

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
 *	@return none
 */
static void initHardware(void);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/




uint8_t Leer_Teclas(io_puerto_t button)
{
	uint8_t ret = NO_BUTTON_PRESSED;

	ret = Chip_GPIO_GetPinState(LPC_GPIO_PORT, button.port, button.pin);
	return ret;
}



static void initHardware(void)
{
    SystemCoreClockUpdate();

    Board_Init();
    //lcdInit(16,2,5,8);
    Board_LED_Set(0, FALSE);
}

static void blink_tsk(void * a)
{
	while (1) {
		Board_LED_Toggle(0);
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}

static void lcd_tsk(void * a)
{	uartConfig(UART_USB, 57600);
	int j=0;
	while (1) {

		Board_LED_Toggle(3);
		uartWriteString(UART_USB, "\n");

		uartWriteString(UART_USB, "\n");




		vTaskDelay(500 / portTICK_RATE_MS);
		//for(j=0;j<(100000000)*3/5;j++);


	}
}


static void teclas_tsk(void *a)
{
	io_puerto_t button_1, button_2, button_3, button_4;
	int tecla1,tecla1_anterior;
	int tecla2,tecla2_anterior;
	int tecla3,tecla3_anterior;
	int tecla4,tecla4_anterior;

	button_1.port=0;
	button_1.pin=4;
	button_2.port=0;
	button_2.pin=8;
	button_3.port=0;
	button_3.pin=9;
	button_4.port=1;
	button_4.pin=9;

	while (1) {

		tecla1 = Leer_Teclas(button_1);
		tecla2 = Leer_Teclas(button_2);
		tecla3 = Leer_Teclas(button_3);
		tecla4 = Leer_Teclas(button_4);

		if(tecla1!=tecla1_anterior && tecla1==1) {
			flag_reset = FALSE;
			flag_start_stop = !flag_start_stop;
			      }
		tecla1_anterior = tecla1;

		tecla2 = Leer_Teclas(button_2);

		if(tecla2!=tecla2_anterior && tecla2==1) {
			flag_reset = TRUE;
			flag_start_stop = FALSE;
					      }
		tecla2_anterior = tecla2;

		tecla3 = Leer_Teclas(button_3);

				if(tecla3!=tecla3_anterior && tecla3==1) {
					if(flag_start_stop==FALSE){
						flag_up_down = TRUE;
						flag_reset = TRUE;
					}
				}
		tecla3_anterior = tecla3;

		tecla4 = Leer_Teclas(button_4);

				if(tecla4!=tecla4_anterior && tecla4==1) {
					if(flag_start_stop==FALSE){
						flag_up_down = FALSE;
						flag_reset = TRUE;
					}
				}
		tecla3_anterior = tecla3;

		vTaskDelay(500 / portTICK_RATE_MS);
	}
}

static void reloj_tsk(void *a){

	bool_t toggle = FALSE;
	while(1){
		if(flag_start_stop==TRUE){
			contador++;
			toggle =(bool) Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 0);
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, 3, 0, !toggle);
			//Board_LED_Toggle(3);
		}
		else
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, 3, 0, 0);

		if(flag_reset){
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, 3, 0, 0);
			contador=0;
		}

		vTaskDelay(100 / portTICK_RATE_MS);

	}
}

/*==================[external functions definition]==========================*/

int main(void)
{
	initHardware();


	xTaskCreate(blink_tsk, (const char *)"blink", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1, 0);

	xTaskCreate(lcd_tsk, (const char *)"lcd", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1, 0);

	xTaskCreate(teclas_tsk, (const char *)"teclas", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+2, 0);

	xTaskCreate(reloj_tsk, (const char *)"reloj", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+3, 0);

	vTaskStartScheduler();

	while (1) {
	}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
