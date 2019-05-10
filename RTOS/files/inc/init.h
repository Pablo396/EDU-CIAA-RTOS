#ifndef _INIT_H_
#define _INIT_H_

#include "board.h"


#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "sapi.h"
#include "lcd.h"
#include "driver_i2c.h"
#include "driver_uart.h"




void init_all(void);
void Iniciar_Puertos(void);

#endif /* #ifndef _INIT_H_ */
