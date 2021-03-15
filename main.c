#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

void GpioConfig(void);
void UARTConfig(void);

uint16_t data=0;
char test[25]="Bilal Dogrucu";
char message[20];


void GpioConfig(){

GPIO_InitTypeDef GPIOInitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	  ///adc//
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIOInitStructure);

	  //////TX PA9
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	  //////RX PA10
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
     //led Out	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
}

void UARTConfig(){
	
USART_InitTypeDef   UARTinitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	UARTinitStructure.USART_BaudRate=9600;
	UARTinitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	UARTinitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	UARTinitStructure.USART_Parity=USART_Parity_No;
	UARTinitStructure.USART_StopBits=USART_StopBits_1;
	UARTinitStructure.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&UARTinitStructure);
	USART_Cmd(USART1,ENABLE);
	
}

void UARTTransmit(char *string){
	
	while(*string){
	while(!(USART1->SR & 0x00000040 ))
		USART_SendData(USART1,*string);
   	*string++;
	}
}
void adcConfig(){
	ADC_InitTypeDef AdcInitSt;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	AdcInitSt.ADC_ContinuousConvMode=ENABLE;
	AdcInitSt.ADC_DataAlign=ADC_DataAlign_Right;
	AdcInitSt.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	AdcInitSt.ADC_Mode=ADC_Mode_Independent;
	AdcInitSt.ADC_NbrOfChannel=1;
	AdcInitSt.ADC_ScanConvMode=0;
   
	
	ADC_Init(ADC1,&AdcInitSt);
	ADC_Cmd(ADC1,ENABLE);
}

uint16_t ReadAdc(){

ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);

ADC_SoftwareStartConvCmd(ADC1,ENABLE);

	
	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	
	return ADC_GetConversionValue;
	
}

void delay(uint32_t time){

	while(time--);
}

int main(){

	GpioConfig();
	UARTConfig();
	adcConfig();
while(1){

	
	data=ReadAdc();
	sprintf(message,"adcValue %d\r\n",data);
	UARTTransmit(message);
	delay(3600);
}

}