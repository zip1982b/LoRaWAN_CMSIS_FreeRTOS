#include "clock.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"



void vMCU_worked(void *arg);
void vLoRaWAN_Modem(void *arg);


static const UARTInitStructure_t UARTInitStr = 
{
  .bus_freq = 36000000,
  .baud = 9600,
  .data_bits = 8,
  .stop_bits = 1,
  .parity = 0,
};

int main()
{
  if(!ClockInit()){
    PortInit();
    UART_Init(2, &UARTInitStr);
    
    
    
    xTaskCreate(vMCU_worked, "MCU worked", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vLoRaWAN_Modem, "RHF78-052", 256, NULL, 2, NULL);
    vTaskStartScheduler();
  }
  else return 0;
  
  while(1){
    
  }
}


/**********TASKS***************/
void vMCU_worked(void *arg){
  
  while(1){
    PortSetHi();
    vTaskDelay(1000 / portTICK_RATE_MS);
    PortSetLow();
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}


void vLoRaWAN_Modem(void *arg){
  UART_ReadBuffClear(2);
  UART_WriteBuffClear(2);
  
  while(1){
    
  }
}


