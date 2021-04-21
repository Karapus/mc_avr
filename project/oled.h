#pragma once
#include<avr/io.h>

#define SCL_F		20000UL
#define Dev_Adr		0b00111100

void twi_Init() {
	TWBR = (((CPU_F)/(SCL_F) - 16)/2);
	TWSR = 0;
}

/*Передача адреса*/
void twi_SendAdress(void) {						
	TWDR=(Dev_Adr<<1)|0;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)))
		;
}

/*Старт на шине I2C*/
void twi_Start() {
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)))
		;
}

/*Стоп на шине I2C*/
void twi_Stop(void){
	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

/*Послать байт данных по шине I2C*/
void twi_SendByte(int Inf){
	TWDR = Inf;
	TWCR = (1<<TWINT)|(1<<TWEN);				
	while(!(TWCR & (1<<TWINT)))
		;
}

/*Послать контроольный байт и байт данных по шине I2C*/
void OLED_Command(int command) {
	twi_SendByte(0x80);
	twi_SendByte(command);
}

void OLED_Data(int data) {
	twi_SendByte(data);
}

/*инициализация дисплея*/
void OLED_Init(){
	twi_Init();
	twi_Start();
	twi_SendAdress();
	OLED_Command(0xA8);
	OLED_Command(0x3F);

	OLED_Command(0xD3);
	OLED_Command(0x00);

	OLED_Command(0x40);
	OLED_Command(0xA1);

	OLED_Command(0xC8);
	OLED_Command(0xDA);
	OLED_Command(0x12);

	OLED_Command(0x81);
	OLED_Command(0xFF);
	OLED_Command(0xA4);

	OLED_Command(0xA6);
	OLED_Command(0xD5);
	OLED_Command(0x80);
	OLED_Command(0x8D);
	OLED_Command(0x14);
	OLED_Command(0xAF);
	OLED_Command(0x20);
		OLED_Command(0x00);
	OLED_Command(0x21);
		OLED_Command(0);
		OLED_Command(127);
	OLED_Command(0x22);
		OLED_Command(0);
		OLED_Command(7);
	OLED_Data(0x40);//Указание что следующиеи байты данных
}

void OLED_Graph(unsigned data[64]) {
	for (int i = 0; i < 64; i += 8) {
		for (int j = 0; j < 128; j++) {
			int byte = 0x0;
			for (int k = 0; k < 8; ++k)
				if (data[i + k] > j)
					byte |= 1 << k;
			OLED_Data(byte);
		}
	}
}
