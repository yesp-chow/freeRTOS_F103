
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"


/*Function definition*/
static void BSP_Init(void);

static void Send_Task(void* pvParameters);
static void Callback_LEDTOGGLE(void* pvParameters);

static void AppTaskCreate(void);


/*Task Handle*/
static TaskHandle_t AppTaskCreate_Handle=NULL;
static TaskHandle_t Send_Task_Handle=NULL;       //PRO:3


/*Timer handle*/
static TimerHandle_t Timer1=NULL;

static void Callback_LEDTOGGLE(void* pvParameters)
{
  LED1_TOGGLE;
  printf("TickValue=%d\n",(TickType_t)xTaskGetTickCount());
}


int main(void)
{  
  BaseType_t xReturn =pdPASS;
  
  BSP_Init();
  
  printf("FreeRTOS creates tasks dynamically!\r\n");
  
  xReturn=xTaskCreate((TaskFunction_t)AppTaskCreate,\
                      (const char *)"APPTASKCREATE",\
                      (uint16_t)512,(void*)NULL,(UBaseType_t)1,\
                      (TaskHandle_t*)&AppTaskCreate_Handle);
  if(pdPASS==xReturn)
    vTaskStartScheduler();
  else
    return -1;
  while(1);
}


/*Hardware initialization*/
static void BSP_Init(void)
{
  /*Use this priority grouping uniformly, never group again
  * 4 bits for pre-emption priority
  */
  
  /*led initialization*/
  LED_GPIO_Config();
  
  /*Serial port initialization*/
  USART_Config();
  
  /*Key initialization*/
  Key_GPIO_Config();
}

/***********
 *@Function name:SEND_Task
 *@description:SEND_Task body
 *@Parameter:None
 *@return:None
*************/
static void Send_Task(void* pvParameters)
{

  while(1)
  {  
    vTaskDelay(100);         //Just to demonstrate task switching.
  }
}

static void AppTaskCreate(void)
{
  BaseType_t xReturn=pdPASS;
  taskENTER_CRITICAL();

  /* Create Timer1 Handle*/
  Timer1=xTimerCreate((const char*)"1sec-toggle",(TickType_t)pdMS_TO_TICKS(1000),\
                     (UBaseType_t)pdTRUE,(void*)1,(TimerCallbackFunction_t)Callback_LEDTOGGLE);
  if(NULL!=Timer1)
    printf("Timer1 created successfully.\r\n");
  xTimerStart(Timer1,0);

  /*SEND task*/
  xReturn=xTaskCreate((TaskFunction_t)Send_Task,\
                      (const char*)"SEND_TASK",\
                      (uint16_t)512,(void*)NULL,(UBaseType_t)3,\
                      (TaskHandle_t)&Send_Task_Handle);
  if(pdPASS==xReturn)
    printf("\r\nCreate Send_Task success!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle);
  taskEXIT_CRITICAL();
}









/*********************************************END OF FILE**********************/


