/*
 * GUIThread.c
 *
 *  Created on: 9 janv. 2021
 *      Author: Alan
 */
#include "main.h"

#include "gui.h"
#include "Buttons.h"

typedef struct FootPrint_t{
	const char* footprintName;
	uint16_t timeMs; // Motor activation time
	uint16_t length;
	uint16_t large;
}FootPrint;

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
	uint8_t metricCodeTabSize = sizeof(MetricCode) / sizeof(MetricCode[0]);

	FootPrint footPrints[metricCodeTabSize];

	for(uint8_t i = 0; i < metricCodeTabSize; i++){
		footPrints[i].footprintName = MetricCode[i];
		footPrints[i].timeMs = (50 * (i+1));
		footPrints[i].length = (5 * (i+1));
		footPrints[i].large = (2 * (i+1));
	}

	__HAL_TIM_SET_AUTORELOAD(&htim7, footPrints[0].timeMs - 1); //Init timer period with the first value (0402)

	for (;;) {
		OLED_setFont(0);
		OLED_setCursor(10, 8);
		OLED_clearScreen();

		if(counterFootprint >= metricCodeTabSize){
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
				if(counterFootprint == metricCodeTabSize - 1){
					counterFootprint = 0;
				}else{
					counterFootprint++;
				}
				break;
			case BUTTON_L_SHORT:
				if(counterFootprint == 0){
					counterFootprint = metricCodeTabSize - 1;
				}else{
					counterFootprint--;
				}
				break;
			case BUTTON_UP_SHORT:
				isUP = true;
				displayArrowState(ARROW_UP, 88);
				break;
			case BUTTON_DOWN_SHORT:
				isUP = false;
				displayArrowState(ARROW_DOWN, 88);
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
			drv8876_direction_control(true);
		}else{
			OLED_setCursor(70, 8);
			OLED_print(DOWNString);
			drv8876_direction_control(false);
		}

		__HAL_TIM_SET_AUTORELOAD(&htim7, footPrints[counterFootprint].timeMs - 1);

		OLED_refresh();

		// slow down ui update rate
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

	bool buttonLockout = false;
	bool alreadyStarted = false;

	for (;;) {
#if 1
		if(systemSettings.isFirstStart || alreadyStarted){
			enterRootMenu();  // enter the settings menu
			buttonLockout = true;
		}else{
			alreadyStarted = true;
			gui_automaticSolderPasteDispenserMode();
		}
#elif 0
		enterRootMenu();  // enter the settings menu
		buttonLockout = true;
#elif 0
		OLED_fullScreen();
		OLED_refresh();
		osDelay(1000);
		OLED_clearScreen();
		OLED_refresh();
		osDelay(1000);
#elif 0
		OLED_setFont(0);
		OLED_setCursor(0, 0);
		OLED_print("\x1D\x16\x15\x0C");
#else
		ButtonState buttons = getButtonState();
		if (buttons != BUTTON_NONE) {
			OLED_setDisplayState(ON);
			OLED_setFont(0);
		}

		if (buttons != BUTTON_NONE && buttonLockout)
			buttons = BUTTON_NONE;
		else
			buttonLockout = false;

		switch (buttons) {
			case BUTTON_NONE:
				// Do nothing
				break;
			case BUTTON_BOTH:
				// Not used yet
				// In multi-language this might be used to reset language on a long hold
				// or some such
				printf("BUTTON_BOTH\r\n");
				break;

			case BUTTON_L_LONG:
				printf("BUTTON_L_LONG\r\n");

				break;
			case BUTTON_R_LONG:
				printf("BUTTON_R_LONG\r\n");

				break;
			case BUTTON_L_SHORT:
				printf("BUTTON_L_SHORT\r\n");

				break;
			case BUTTON_R_SHORT:
				printf("BUTTON_R_SHORT\r\n");
				enterRootMenu();  // enter the settings menu
				buttonLockout = true;
				break;
			default:
				break;
		}
#endif

		OLED_refresh();
		GUIDelay();
	}
  /* USER CODE END StartGUITask */
}

