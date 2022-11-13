#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "sdkconfig.h"

#define PUSH_BUTTON_PIN GPIO_NUM_13

static void configure_button(void)
{
    gpio_set_pull_mode(PUSH_BUTTON_PIN, GPIO_PULLDOWN_ONLY);
    gpio_set_level(PUSH_BUTTON_PIN, 0);
    gpio_set_direction(PUSH_BUTTON_PIN, GPIO_MODE_INPUT);
}

void printStudentID(void * parameters){
	for(;;){
		printf("1914403\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}

void pollingButton(void * parameters){
	int current_state;
	int last_state = 1;
	for(;;){
		configure_button();
		current_state = gpio_get_level(PUSH_BUTTON_PIN);
    		if (last_state == 1 && current_state == 0){
    			printf("esp32\n");
    		}
    		last_state = current_state;
    		vTaskDelay(10 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}

void app_main(void)
{
	xTaskCreate(&printStudentID, "print student ID task", 2048, NULL, 3, NULL);
	xTaskCreate(&pollingButton, "polling button task", 2048, NULL, 4, NULL);
}
