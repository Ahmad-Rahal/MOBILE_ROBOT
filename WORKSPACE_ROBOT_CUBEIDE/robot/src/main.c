#include "main.h"


//#######################################################
#define EX1 1
#define EX2 2
#define EX3 3

#define SYNCHRO_EX EX1

//#######################################################

// Déclaration des objets synchronisants !! Ne pas oublier de les créer
xSemaphoreHandle xSemaphore = NULL;
xQueueHandle qhR = NULL, qhL = NULL;
TaskHandle_t xSensorTask, xMotorRightTask, xMotorLeftTask, xKeyControl;


extern uint8_t rec_buf2[NB_CAR_TO_RECEIVE+1];	 // defined in drv_uart.c
extern uint8_t rec_buf6[NB_CAR_TO_RECEIVE+1];

char sendData[100];
uint8_t robotState = 0;
int setVelocity = 0;
uint8_t mode = 1;
char LCDdirection ;
char testDirection;

struct AMessage
{
	char command;
	int data;
};

//========================================================

int tab_speed[200];
float tab_cde[200];
int desiredSpeedR = 600;
int desiredSpeedL = 600;
int frontDistSens[2];
int minFrontDistSens = 600;
int setPointLeft = 600;
int setPointRight = 600;
int dutyValueL;
int dutyValueR;

uint8_t controlDirection = 0;



uint16_t backDistSens[2];
uint16_t minBackDist = 13;

uint16_t i = 0;

static void motorRight(void *pvParameters)
{
    int speedRight;
    float upRight, uiRight = 0.0;
    float errRight;
    float Tright = 115 * 5;
    float TiRight = 0.1 * Tright;
    float KpRight = 0.03;
    int TeRight = 5;
    float KiRight = TeRight / TiRight;
    uint8_t add = 0;

    struct AMessage pxRxedMessage;
    pxRxedMessage.command='a';
    pxRxedMessage.data= 0;

	for (;;)
	{
		xQueueReceive(qhR,  &( pxRxedMessage ) , portMAX_DELAY );
		desiredSpeedR = pxRxedMessage.data;
	        speedRight = quadEncoder_GetSpeedR();
	        errRight = desiredSpeedR-(float)speedRight;
	        upRight = KpRight*errRight;
	        uiRight = uiRight+KpRight*KiRight*errRight;
	        motorRight_SetDuty(100+(int)(upRight + uiRight));
	        if((upRight + uiRight) > 100 || (upRight + uiRight) < -100){
	        	upRight = 0;
	        	uiRight = 0;
	        	        }
//	        snprintf(sendData, sizeof(sendData), "1,%d", 234);
//	        HAL_UART_Transmit(&Uart2Handle, (uint8_t*)sendData, strlen(sendData), HAL_MAX_DELAY);

	        xSemaphoreGive( xSemaphore );
	        vTaskDelay(pdMS_TO_TICKS(5));
		}


	}


static void motorLeft(void *pvParameters)
{
    int speedLeft;
    float upLeft, uiLeft = 0.0;
    float errLeft;
    float Tleft = 115 * 5;
    float TiLeft = 0.1 * Tleft;
    float KpLeft = 0.03;
    int TeLeft = 5;
    float KiLeft = TeLeft / TiLeft;
    uint8_t add = 0;

    struct AMessage pxRxedMessage;
    pxRxedMessage.command='a';
    pxRxedMessage.data= 0;

	    for (;;)
	    {
	    	xQueueReceive( qhL,  &( pxRxedMessage ) , portMAX_DELAY );
	    	desiredSpeedL = pxRxedMessage.data;
	        speedLeft = quadEncoder_GetSpeedL();
	        errLeft = desiredSpeedL-(float)speedLeft;
	        upLeft = KpLeft*errLeft;
	        uiLeft = uiLeft+KpLeft*KiLeft*errLeft;
	        motorLeft_SetDuty(100+(int)(upLeft + uiLeft));
	        if((upLeft + uiLeft) > 100 || (upLeft + uiLeft) < -100){
	        	upLeft = 0;
	        	uiLeft = 0;
	        }

//	        sendData[0] = 0;
//	        sendData[1] = 15;
//	        HAL_UART_Transmit(&Uart2Handle, sendData, 2, HAL_MAX_DELAY);
//	        add = (int)(upLeft + uiLeft);
//	        dutyValueL = 100 + add;
//	        motorLeft_SetDuty(100 + add);
//	        if(add > 100 || add < -100){
//	        	        	add = 0;
//	        	        }
	        xSemaphoreGive( xSemaphore );
	        vTaskDelay(pdMS_TO_TICKS(5));
}
}




static void decision( void *pvParameters )
{
	struct AMessage pxMessage;
		pxMessage.command='a';
		pxMessage.data= 0;

	for (;;)
	{
		if(robotState){
		switch(mode){
		case 1:
		captDistUS_Measure(0xE0);
		captDistUS_Measure(0xE2);

		// Wait for 70ms
		if(i==20){
			backDistSens[0] = captDistUS_Get(0xE0);
			backDistSens[1] = captDistUS_Get(0xE2);
			i = 0;
		}
		i++;

		captDistIR_Get(&frontDistSens);

//		if(backDistSens[0] <= minBackDist && backDistSens[1] <= minBackDist){
//			setPointRight = setVelocity;
//			setPointLeft = setVelocity;
//		}
//		else if(backDistSens[0] <= minBackDist){
//			setPointLeft = -setVelocity/2;
//		}
//		else if(backDistSens[1] <= minBackDist){
//			setPointRight = -setVelocity/2;
//		}else{
//			setPointLeft = -setVelocity;
//			setPointRight = -setVelocity;
//		}

		if(minFrontDistSens< frontDistSens[0] && minFrontDistSens < frontDistSens[1]) {
			LCDdirection = "BACKWARD";
		    setPointLeft = -setVelocity;
		    setPointRight = -setVelocity;
		} else if (minFrontDistSens < frontDistSens[0]) {
			LCDdirection = "RIGHT";
			setPointRight = -setVelocity;
			setPointLeft = setVelocity;

		} else if (minFrontDistSens < frontDistSens[1]) {
			LCDdirection = "LEFT";
			setPointLeft = -setVelocity;
			setPointRight = setVelocity;
		} else {
			LCDdirection = "FORWARD";
		    setPointLeft = setVelocity;
		    setPointRight = setVelocity;
		}
		break;

		case 2:
			switch (controlDirection){
					case 1:
						LCDdirection = "FORWARD";
						setPointLeft = setVelocity;
						setPointRight = setVelocity;
						break;
					case 2:
						LCDdirection = "LEFT";
						setPointLeft = -setVelocity;
						setPointRight = setVelocity;
						break;
					case 3:
						LCDdirection = "RIGHT";
						setPointLeft = setVelocity;
						setPointRight = -setVelocity;
						break;
					case 4:
						LCDdirection = "BACKWARD";
						setPointLeft = -setVelocity;
						setPointRight = -setVelocity;
						break;
					case 0:
						LCDdirection = "STOP";
						setPointLeft = 0;
						setPointRight = 0;
						break;
					}
				    controlDirection = 0;
	     break;
	     case 3:

	    	 posH=servoHigh_Get();
	    	 posL=servoLow_Get();
	         pixyCam2_Get(XY,WH);
	    	 term_printf_stlink("x = %d y = %d \n\r",XY[0],XY[1]);


	    	 						if(XY[0]>150)
	    	 						{
	    	 								servoLow_Set(posL-2);

	    	 						}
	    	 						else if(XY[0]<150)
	    	 						{
	    	 								servoLow_Set(posL+2);
	    	 						}

	    	 						if(XY[1]>150)
	    	 						{
	    	 								servoHigh_Set(posH+2);
	    	 														}
	    	 						else if(XY[1]<150)
	    	 						{
	    	 								servoHigh_Set(posH-2);
	    	 														}


	    	 break;

		}
		}else{
			setPointLeft = 0;
			setPointRight = 0;
		}


		pxMessage.data = setPointRight;
		xQueueSend( qhR, ( void * ) &pxMessage,  portMAX_DELAY );

		xSemaphoreTake( xSemaphore, portMAX_DELAY );

		pxMessage.data = setPointLeft;
		xQueueSend( qhL, ( void * ) &pxMessage,  portMAX_DELAY );

		xSemaphoreTake( xSemaphore, portMAX_DELAY );
		xSemaphoreTake( xSemaphore, portMAX_DELAY );


	}
}


static void LCD(void *pvParameters)
{
	struct AMessage pxRxedMessage;
	for (;;)
	{	testDirection = LCDdirection;
		groveLCD_term_printf(LCDdirection);
		xSemaphoreGive( xSemaphore );
	}
}
//========================================================


//=========================================================
//	>>>>>>>>>>>>	MAIN	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//=========================================================

int main(void)
{
	HAL_Init();
	SystemClock_Config();

	uart2_Init();			// CABLE
	uart6_Init();			// ZIGBEE
	i2c1_Init();
	spi1Init();
	pixyCam2_Init();			// Caméra Pixy
	captDistIR_Init();		// Capteurs Infrarouge
	quadEncoder_Init();		// Encodeurs Incrémentaux
	motorCommand_Init();	// Commande des Hacheurs
	servoCommand_Init();	// Commande des Servomoteurs

	// Test Ecran LCD
	int a, b;
	groveLCD_begin(16,2,0); // !! cette fonction prend du temps
	HAL_Delay(100);
	groveLCD_setColor(1);
	a=5; b=2;
	groveLCD_term_printf("%d+%d=%d",a,b,a+b);


	HAL_Delay(1000);

	//pixyCam2_Test();  // !! boucle infinie

	motorLeft_SetDuty(100);
	motorRight_SetDuty(100);



    xTaskCreate( decision, ( const portCHAR * ) "task B", 512 /* stack size */, NULL, tskIDLE_PRIORITY+3, &xSensorTask);
	xTaskCreate( motorRight, ( const portCHAR * ) "task A", 512 /* stack size */, NULL, tskIDLE_PRIORITY+3, NULL );
	xTaskCreate( motorLeft, ( const portCHAR * ) "task B", 512 /* stack size */, NULL, tskIDLE_PRIORITY+2, NULL );
	xTaskCreate( LCD, ( const portCHAR * ) "task A", 512 /* stack size */, NULL, tskIDLE_PRIORITY+1, NULL );



	vSemaphoreCreateBinary(xSemaphore);
	xSemaphoreTake( xSemaphore, portMAX_DELAY );

	qhR = xQueueCreate( 1, sizeof(struct AMessage ) );
	qhL = xQueueCreate( 1, sizeof(struct AMessage ) );


	vTaskStartScheduler();

	return 0;

}


//=================================================================
//	UART RECEIVE CALLBACK5
//=================================================================
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle -> Instance == USART2)
	{
        switch(rec_buf2[0]){
        case 0:
           robotState = rec_buf2[1];
        	break;
        case 1:
           mode = rec_buf2[1];
        	break;
        case 2:
           setVelocity = (int)(rec_buf2[1]*3100/100);  //percentage
        	break;
        case 3:
           controlDirection = rec_buf2[1];
        	break;
        case 4:

        	break;
        }
		HAL_UART_Receive_IT(&Uart2Handle, (uint8_t *)rec_buf2, NB_CAR_TO_RECEIVE);

	}

	if(UartHandle -> Instance == USART6)
	{
        switch(rec_buf6[0]){
        case 0:
           robotState = rec_buf6[1];
        	break;
        case 1:
         mode = rec_buf6[1];
        	break;
        case 2:
           setVelocity = (int)(rec_buf6[1]*3100/100);  //percentage
        	break;
        case 3:
           controlDirection = rec_buf6[1];
        	break;
        case 4:
        	break;
        }
		 HAL_UART_Receive_IT(&Uart6Handle, (uint8_t *)rec_buf6, NB_CAR_TO_RECEIVE);
	}

}



//=================================================================
// Called if stack overflow during execution
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed char *pcTaskName)
{
	//term_printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	/* If the parameters have been corrupted then inspect pxCurrentTCB to
	 * identify which task has overflowed its stack.
	 */
	for (;;) {
	}
}
//=================================================================
//This function is called by FreeRTOS idle task
extern void vApplicationIdleHook(void)
{
}
//=================================================================
// brief This function is called by FreeRTOS each tick
extern void vApplicationTickHook(void)
{
//	HAL_IncTick();
}
