/*
 * GUIThread.c
 *
 *  Created on: 9 janv. 2021
 *      Author: Alan
 */
#include "main.h"

#include "gui.h"
#include "Buttons.h"

#include "DRV8876.h"

#include "configuration.h"

void GUIDelay() {
	// Called in all UI looping tasks,
	// This limits the re-draw rate to the LCD and also lets the DMA run
	// As the gui task can very easily fill this bus with transactions, which will
	// prevent the movement detection from running
	osDelay(50);
}

typedef enum ArrowState_t {
	ARROW_UP, /* TODO */
	ARROW_DOWN, /* TODO */
	ARROW_EMPTY, /* TODO */
}ArrowState;

ButtonState buttonsSave;

static void displayArrow(int16_t x){
	if(buttonsSave == BUTTON_UP_SHORT){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 2])); // Arrow UP full
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 1])); // Arrow DOWN empty
	}else if(buttonsSave == BUTTON_DOWN_SHORT){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 0])); // Arrow UP empty
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 3])); // Arrow DOWN full
	}else if(buttonsSave == BUTTON_NONE){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 0])); // Arrow UP empty
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 1])); // Arrow DOWN empty
	}else{
		// Nothing
	}
	osDelay(50);
}

static void displayArrowState(ArrowState arrowState, int16_t x){
	if(arrowState == ARROW_UP){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 2])); // Arrow UP full
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 1])); // Arrow DOWN empty
	}else if(arrowState == ARROW_DOWN){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 0])); // Arrow UP empty
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 3])); // Arrow DOWN full
	}else if(arrowState == ARROW_EMPTY){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 0])); // Arrow UP empty
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 1])); // Arrow DOWN empty
	}else{
		// Nothing
	}
	osDelay(50);
}

static void gui_automaticSolderPasteDispenserMode(void) {
	/*
	 * * Automatic Solder Paste Dispenser (gui_automaticSolderPasteDispenserMode)
	 * -> Main loop where we draw the footprint size, and animations
	 * --> User presses buttons and they goto the temperature adjust screen
	 * ---> Display the current setpoint temperature
	 * ---> Use buttons to change forward and back on temperature
	 * ---> Both buttons or timeout for exiting
	 * --> Long hold front button to enter boost mode
	 * ---> Just temporarily sets the system into the alternate temperature for
	 * PID control
	 * --> Long hold back button to exit
	 * --> Double button to exit
	 */
	counterFootprint = 0;
	bool isUP = false;

	uint16_t pushTime[12] = {1000, PUSH_TIME_0201, PUSH_TIME_0402, PUSH_TIME_0603, PUSH_TIME_0805, 0, 0, 0, 0, 0, 0, 0};
	uint16_t retractTime[12] = {10, RETRACT_TIME_0201, RETRACT_TIME_0402, RETRACT_TIME_0603, RETRACT_TIME_0805, 0, 0, 0, 0, 0, 0, 0};

	uint8_t footprintsCodeTabSize = sizeof(ImperialCode) / sizeof(ImperialCode[0]);

	//footPrints[footprintsCodeTabSize];

	for(uint8_t i = 0; i < footprintsCodeTabSize; i++){
#ifdef IMPERIAL
		footPrints[i].footprintName = ImperialCode[i];
#endif

#ifdef METRIC
		footPrints[i].footprintName = MetricCode[i];
#endif

		footPrints[i].pushTime = pushTime[i];
		footPrints[i].retractTime = retractTime[i];
	}

	counterFootprint = 3; //Just for test

	__HAL_TIM_SET_AUTORELOAD(&htim7, footPrints[counterFootprint].pushTime - 1); //Init timer period with the first value (0402)

	for (;;) {
		OLED_setFont(0);
		OLED_setCursor(0, 8);
		OLED_clearScreen();

		if(counterFootprint >= footprintsCodeTabSize){
			counterFootprint = 0;
		}

		OLED_print(footPrints[counterFootprint].footprintName);

		ButtonState buttons = getButtonState();
		buttonsSave = buttons;

		switch (buttons) {
			case BUTTON_NONE:
				//displayArrowState(ARROW_EMPTY, 88);
				break;
			case BUTTON_R_LONG:
				break;
			case BUTTON_L_LONG:
				break;
			case BUTTON_UP_LONG:
				break;
			case BUTTON_DOWN_LONG:
				break;
			case BUTTON_CENTER_LONG:
				// Exit this menu
				return;
				break;
			case BUTTON_R_SHORT:
				if(counterFootprint == footprintsCodeTabSize - 1){
					counterFootprint = 0;
				}else{
					counterFootprint++;
				}
				__HAL_TIM_SET_AUTORELOAD(&htim7, footPrints[counterFootprint].pushTime - 1);
				break;
			case BUTTON_L_SHORT:
				if(counterFootprint == 0){
					counterFootprint = footprintsCodeTabSize - 1;
				}else{
					counterFootprint--;
				}
				__HAL_TIM_SET_AUTORELOAD(&htim7, footPrints[counterFootprint].pushTime - 1);
				break;
			case BUTTON_UP_SHORT:
				isUP = true;
				displayArrowState(ARROW_UP, 88);
				drv8876_direction_control(false);
				break;
			case BUTTON_DOWN_SHORT:
				isUP = false;
				displayArrowState(ARROW_DOWN, 88);
				drv8876_direction_control(true);
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				if (motorTaskNotification) {
					vTaskNotifyGiveFromISR(motorTaskNotification,
							&xHigherPriorityTaskWoken);
					portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				}
				break;
			case BUTTON_CENTER_SHORT:

				break;
			default:
				break;
		}

		displayArrowState(ARROW_EMPTY, 88);

		if(isUP){
			OLED_setCursor(70, 8);
			OLED_print(UPString);
		}else{
			OLED_setCursor(70, 8);
			OLED_print(DOWNString);
		}

		OLED_refresh();

		// Slow down ui update rate
		GUIDelay();
	}
}

/* USER CODE BEGIN Header_StartGUITask */
/**
  * @brief  Function implementing the GUITask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartGUITask */
void StartGUITask(void *argument)
{
  /* USER CODE BEGIN StartGUITask */
  /* Infinite loop */
	OLED_initialize();  // start up the OLED screen

	bool startRootMenu = true;

	for (;;) {
		if(startRootMenu){
			enterRootMenu();  // enter the settings menu
		}else{
			gui_automaticSolderPasteDispenserMode();
		}

		startRootMenu = !startRootMenu;

		OLED_refresh();
		osDelay(200);
	}
  /* USER CODE END StartGUITask */
}

