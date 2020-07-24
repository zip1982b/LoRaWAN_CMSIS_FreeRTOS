
#include "clock.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"
#include "modemRHF78-052.h"
#include "stm32f1xx.h"

__IO uint32_t tmpreg;

void vMCU_worked(void *arg);
void vLoRaWAN_Modem(void *arg);

//commands
const  char check_link[] = "AT\r\n";
const  char answer_check_link[] = "+AT: OK\r\n";


const  char set_DEVADDR[] = "AT+ID=DevAddr, \"014373d8\"\r\n";
const  char answer_DEVADDR[] = "+ID: DevAddr, 01:43:73:D8\r\n";

const  char set_devEUI[] = "AT+ID=DevEui, \"E49C8F9C70F2CD95\"\r\n";
const  char answer_devEUI[] = "+ID: DevEui, E4:9C:8F:9C:70:F2:CD:95\r\n";

const  char EU433[] = "AT+DR=EU433\r\n";
const  char answer_EU433[] = "+DR: EU433\r\n";


const char set_channel0[] = "AT+CH=0,433.175,DR0,DR5\r\n";
const char answer_set_channel0[] = "+CH: 0,433175000,DR0:DR5\r\n";


const  char set_channel1[] = "AT+CH=1,433.375,DR0,DR5\r\n";
const  char answer_set_channel1[] = "+CH: 1,433375000,DR0:DR5\r\n";


const  char set_channel2[] = "AT+CH=2,433.575,DR0,DR5\r\n";
const  char answer_set_channel2[] = "+CH: 2,433575000,DR0:DR5\r\n";


const  char set_channel3[] = "AT+CH=3,433.775,DR0,DR5\r\n";
const  char answer_set_channel3[] = "+CH: 3,433775000,DR0:DR5\r\n";


const  char set_channel4[] = "AT+CH=4,433.975,DR0,DR5\r\n";
const  char answer_set_channel4[] = "+CH: 4,433975000,DR0:DR5\r\n";


const  char set_channel5[] = "AT+CH=5,434.175,DR0,DR5\r\n";
const  char answer_set_channel5[] = "+CH: 5,434175000,DR0:DR5\r\n";


const  char set_channel6[] = "AT+CH=6,434.375,DR0,DR5\r\n";
const  char answer_set_channel6[] = "+CH: 6,434375000,DR0:DR5\r\n";


const  char set_channel7[] = "AT+CH=7,434.575,DR0,DR5\r\n";
const  char answer_set_channel7[] = "+CH: 7,434575000,DR0:DR5\r\n";


const  char RXWIN2[] = "AT+RXWIN2=434.665,DR0\r\n";
const  char answer_RXWIN2[] = "+RXWIN2: 434665000,DR0\r\n";


const  char default_Data_Rate[] = "AT+DR=DR0\r\n";
const  char answer_default_Data_Rate[] = "+DR: EU433 DR0 SF12 BW125K\r\n";


const  char default_Power[] = "AT+POWER=12\r\n";
const  char answer_default_Power[] = "+POWER: 12\r\n";

const  char Port[] = "AT+PORT=1\r\n";
const  char answer_Port[] = "+PORT: 1\r\n";

const  char ADR[] = "AT+ADR=ON\r\n";
const  char answer_ADR[] = "+ADR: ON\r\n";


const  char NwkSKey[] = "AT+KEY=NwkSKey, \"a3b6555a4525d72976ee2aa991fa4c5e\"\r\n";
const  char answer_NwkSKey[] = "+KEY: NWKSKEY A3B6555A4525D72976EE2AA991FA4C5E\r\n";


const  char AppSKey[] = "AT+KEY=APPSKEY, \"f8adb8ac7fea829dd5cefc00a72c55d7\"\r\n";
const  char answer_AppSKey[] = "+KEY: APPSKEY F8ADB8AC7FEA829DD5CEFC00A72C55D7\r\n";


const  char ClassA[] = "AT+CLASS=A\r\n";
const  char answer_ClassA[] = "+CLASS: A\r\n";


const  char message[] = "AT+CMSG=\"Hello Zhan!\"\r\n";



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
    RCC_DeInit();
  //if(!ClockInit()){
        
    SetSysClockTo72();
	/*
	RCC_DeInit();
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
	  //Delay after an RCC peripheral clock enabling
	tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
	  //NOJTAG: JTAG-DP Disabled and SW-DP Enabled 
	CLEAR_BIT(AFIO->MAPR,AFIO_MAPR_SWJ_CFG);
	SET_BIT(AFIO->MAPR, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);
	SetSysClockTo72();
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN |RCC_APB2ENR_IOPBEN);
	  //Delay after an RCC peripheral clock enabling
	tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN |RCC_APB2ENR_IOPBEN);
*/
    PortInit();
    UART_Init(2, &UARTInitStr);
    
    
    
    xTaskCreate(vMCU_worked, "MCU worked", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vLoRaWAN_Modem, "RHF78-052", 256, NULL, 2, NULL);
    vTaskStartScheduler();
  //}
  //else return 0;
  
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
        #if defined DEBUG
        vTaskDelay(5000 / portTICK_RATE_MS);
        #endif
        
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(check_link)){
		vTaskDelay(500);
		if(receive_answer(answer_check_link)){
            #if defined DEBUG
			printf("link is ok\n");
            #endif
            #if defined RELEASE
			asm("nop");
            #endif
		}
		else {
			#if defined DEBUG
			printf("link is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(set_DEVADDR)){
		vTaskDelay(500);
		if(receive_answer(answer_DEVADDR)){
			#if defined DEBUG
			printf("device address is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("device address is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}

	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(set_devEUI)){
		vTaskDelay(500);
		if(receive_answer(answer_devEUI)){
			#if defined DEBUG
			printf("dev EUI is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("dev EUI is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);  
	if(send_AT_command(EU433)){
		vTaskDelay(500);
		if(receive_answer(answer_EU433)){
			#if defined DEBUG
			printf("EU433 is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("EU433 is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
   
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);  
	if(send_AT_command(set_channel0)){
		vTaskDelay(500);
		if(receive_answer(answer_set_channel0)){
			#if defined DEBUG
			printf("set_channel0 is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
			
		}
        else {
			#if defined DEBUG
			printf("set_channel0 is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);  
	if(send_AT_command(set_channel1)){
		vTaskDelay(500);
		if(receive_answer(answer_set_channel1)){
			#if defined DEBUG
			printf("set_channel1 is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("set_channel1 is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);  
	if(send_AT_command(set_channel2)){
		vTaskDelay(500);
		if(receive_answer(answer_set_channel2)){
			#if defined DEBUG
			printf("set_channel2 is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else{
			#if defined DEBUG
			printf("set_channel2 is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);  
	if(send_AT_command(set_channel3)){
		vTaskDelay(500);
		if(receive_answer(answer_set_channel3)){
			#if defined DEBUG
			printf("set_channel3 is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("set_channel3 is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);  
	if(send_AT_command(set_channel4)){
		vTaskDelay(500);
		if(receive_answer(answer_set_channel4)){
			#if defined DEBUG
			printf("set_channel4 is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("set_channel4 is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);  
	if(send_AT_command(set_channel5)){
		vTaskDelay(500);
		if(receive_answer(answer_set_channel5)){
			#if defined DEBUG
			printf("set_channel5 is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("set_channel5 is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}

	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(set_channel6)){
		vTaskDelay(500);
		if(receive_answer(answer_set_channel6)){
			#if defined DEBUG
			printf("set_channel6 is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else{
			#if defined DEBUG
			printf("set_channel6 is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(set_channel7)){
		vTaskDelay(500);
		if(receive_answer(answer_set_channel7)){
			#if defined DEBUG
			printf("set_channel7 is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("set_channel7 is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);	
	if(send_AT_command(RXWIN2)){
		vTaskDelay(500);
		if(receive_answer(answer_RXWIN2)){
			#if defined DEBUG
			printf("RXWIN2 is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("RXWIN2 is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
  
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(default_Data_Rate)){
		vTaskDelay(500);
		if(receive_answer(answer_default_Data_Rate)){
			#if defined DEBUG
			printf("default_Data_Rate is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("default_Data_Rate is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}

	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(default_Power)){
		vTaskDelay(500);
		if(receive_answer(answer_default_Power)){
			#if defined DEBUG
			printf("default_Power is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("default_Power is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(ADR)){
		vTaskDelay(500);
		if(receive_answer(answer_ADR)){
			#if defined DEBUG
			printf("ADR is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("ADR is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(NwkSKey)){
		vTaskDelay(500);
		if(receive_answer(answer_NwkSKey)){
			#if defined DEBUG
			printf("NwkSKey is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("NwkSKey is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}	
 
 
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(AppSKey)){
		vTaskDelay(500);
		if(receive_answer(answer_AppSKey)){
			#if defined DEBUG
			printf("AppSKey is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
			
		}
        else {
			
		}
	}
  
  
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(Port)){
		vTaskDelay(500);
		if(receive_answer(answer_Port)){
			#if defined DEBUG
			printf("Port is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
        else {
			#if defined DEBUG
			printf("Port is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
  
	UART_ReadBuffClear(2);
	UART_WriteBuffClear(2);
	if(send_AT_command(ClassA)){
		vTaskDelay(500);
		if(receive_answer(answer_ClassA)){
			#if defined DEBUG
			printf("ClassA is ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
			
		}
        else {
			#if defined DEBUG
			printf("ClassA is not ok\n");
			#endif
			#if defined RELEASE
			asm("nop");
			#endif
		}
	}
	
	
  while(1){
    vTaskDelay(10000);
    UART_ReadBuffClear(2);
    UART_WriteBuffClear(2);
    send_AT_command(message);
  }
}


	
	