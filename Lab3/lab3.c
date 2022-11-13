#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "driver/gpio.h"



volatile uint32_t ulIdleCycle = 0UL;

// PREEMPTION = 0
volatile uint32_t ulIdleTask1 = 0UL;       
//

//PREEMPTION = 1
volatile uint32_t ulIdleTask2 = 0UL;
volatile uint32_t ulIdleTask3 = 0UL;
//

// void vApplicationTickHook( void )
// {
    
// }

void vTask_1(void *pvParameter);
void vTask_2(void *pvParameter);
void vTask_3(void *pvParameter);

void vApplicationIdleHook( void ) {
        ulIdleCycle++;
}

void vTask_1(void *pvParameter) {
    while (1) {
        if (configUSE_PREEMPTION == 0) {
            ulIdleTask1++;
            if (ulIdleTask1 == 3) {
                xTaskCreatePinnedToCore(&vTask_2,"Task2",2048,NULL,2,NULL,0);
            }
            else if (ulIdleTask1 == 6) {
                xTaskCreatePinnedToCore(&vTask_3,"Task3",2048,NULL,5,NULL,0);
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else {
            printf("%d \n", ulIdleTask2);
            printf("%d \n", ulIdleTask3);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

void vTask_2(void *pvParameter) {
    int count = 0;
    while (1) {
        if (configUSE_PREEMPTION == 0) {
            count++;
            printf("Task 2 count: %d\n",count);
            if(count == 20)  {
                printf("Task 2 end !!!\n");
                vTaskDelete(NULL);
            }
        }
        else if (configUSE_PREEMPTION == 1)
            ulIdleTask2++;
    }
}

void vTask_3(void *pvParameter) {
    int count = 0;
    while (1) {
        if (configUSE_PREEMPTION == 0) {
            count++;
            printf("Task 3 count: %d\n", count);
            if(count == 10) {
                printf("Task 3 end !!!\n");
                vTaskDelete(NULL);
            }
        }
        else if (configUSE_PREEMPTION == 1)   
            ulIdleTask3++;
    }
}


void app_main() {
    printf("Preemptive : %d \n",configUSE_PREEMPTION);
    printf("Time-slicing : %d \n\n",configUSE_TIME_SLICING);

    if (configUSE_PREEMPTION == 0) {
        xTaskCreate(&vTask_1,"Task1",2048,NULL,0,NULL);
    }
    else {
        xTaskCreatePinnedToCore(&vTask_1,"Task2",2048,NULL,2,NULL,1);
        xTaskCreatePinnedToCore(&vTask_2,"Task2",2048,NULL,0,NULL,0);
        xTaskCreatePinnedToCore(&vTask_3,"Task3",2048,NULL,0,NULL,0);
    }
}


