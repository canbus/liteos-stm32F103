/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
static uint8_t Uar1tRxBuff[UART1_BUFF_SIZE+1];
USART_RECEIVETYPE Uart1Rx = {
                               .RxBuff = Uar1tRxBuff,
                            };
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  //add for DMA.Idle interrupt
  __HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_IDLE); 
  __HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_TC); 
  HAL_UART_Receive_DMA(&huart1, Uart1Rx.RxBuff, UART1_BUFF_SIZE); //开启DMA接收 
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);               //使能空闲中断 

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = Uart1TxPin_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Uart1TxPin_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = Uart1RxPin_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(Uart1RxPin_GPIO_Port, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, Uart1TxPin_Pin|Uart1RxPin_Pin);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void USART1_ReceiveIDLE(void)  
{  
    uint32_t temp;  
    if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET))  
    {
        __HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_IDLE); 
        temp = huart1.Instance->SR;
        temp = huart1.Instance->DR;			
        HAL_UART_DMAStop(&huart1);  
        temp = huart1.hdmarx->Instance->CNDTR;  
        Uart1Rx.RxLen =  UART1_BUFF_SIZE - temp;   
        Uart1Rx.RxFlag=1; 
        Uart1Rx.RxBuff[Uart1Rx.RxLen] = 0;
        HAL_UART_Receive_DMA(&huart1,Uart1Rx.RxBuff,UART1_BUFF_SIZE);  				
    } 				
}
void UART_SendByte(USART_TypeDef * Uart,uint8_t data)
{     
    Uart->DR = data;
	while((Uart->SR&UART_FLAG_TXE)==0);
	while((Uart->SR&UART_FLAG_TC)==0);	       
}
void UART_SendData(USART_TypeDef * Uart,uint8_t *buff,uint16_t size)
{
    while(size--)
	{	
		Uart->DR = *(buff++);
		while((Uart->SR&UART_FLAG_TXE)==0);
	}
    while((Uart->SR&UART_FLAG_TC)==0);       
}
///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到USART1 */
    UART_SendByte(USART1, (uint8_t) ch);
    return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
    /* 等待串口1输入数据 */
    while((USART1->SR&UART_FLAG_RXNE)==0);
    return (int)USART1->DR&0xff;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
