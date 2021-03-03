#include<avr/io.h>
#include<avr/interrupt.h>

#define CPU_F				4000000UL
#define SCL_F				20000UL
#define Skip_Flag			0xff
#define Dev_Adr				0b00111100

/*Установка скорости I2C*/
void twi_Init(void){							
		TWBR=(((CPU_F)/(SCL_F)-16)/2);			
		TWSR=0;}

/*Передача адреса*/
void twi_SendAdress(void) {						
		TWDR=(Dev_Adr<<1)|0;					
		TWCR=(1<<TWINT)|(1<<TWEN);				
		while(!(TWCR & (1<<TWINT))){};
		}

/*Старт на шине I2C*/
void twi_Start(void){
		TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		while(!(TWCR & (1<<TWINT))){};
		}

/*Стоп на шине I2C*/
void twi_Stop(void){
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
		}

/*Послать байт данных по шине I2C*/
void twi_SendByte(int Inf){
		TWDR=Inf;
		TWCR=(1<<TWINT)|(1<<TWEN);				
		while(!(TWCR & (1<<TWINT))){};
		}

/*Послать контроольный байт и байт данных по шине I2C*/
void OLED_Command(int command){

		TWDR=0x80;								
		TWCR=(1<<TWINT)|(1<<TWEN);				
		while(!(TWCR & (1<<TWINT))){};

		TWDR=command;							
		TWCR=(1<<TWINT)|(1<<TWEN);				
		while(!(TWCR & (1<<TWINT))){};
		}

/*инициализация дисплея*/
void OLED_Init(void){
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
		}

/*аналог команды twi_SendByte*/
void OLED_Data(int data){
		/*TWDR=0xC0;//При желании можно настроить на пепредачу одного байта данных
		TWCR=(1<<TWINT)|(1<<TWEN);		
		while(!(TWCR & (1<<TWINT))){};*/

		TWDR=data;							
		TWCR=(1<<TWINT)|(1<<TWEN);				
		while(!(TWCR & (1<<TWINT))){};
		}

/*ОСНОВНАЯ ФУНКЦИЯ*/
int main(void){
		twi_Init();
		twi_Start();
		twi_SendAdress();
		OLED_Init();

/*Очистка дисплея и вывод начального текста*/
		OLED_Command(0x20);
			OLED_Command(0x00);
		OLED_Command(0x21);
			OLED_Command(0);
			OLED_Command(127);
		OLED_Command(0x22);
			OLED_Command(0);
			OLED_Command(7);
		TWDR=0x40;//Указание что следующиеи байты данных
		TWCR=(1<<TWINT)|(1<<TWEN);		
		while(!(TWCR & (1<<TWINT))){};
		for(int i=0; i<386; i++){OLED_Data(0);}
/*текст*/
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);OLED_Data(0b00010100);OLED_Data(0b00100010);OLED_Data(0b01000001);OLED_Data(0b10000000); 	//К
		OLED_Data(0b00000000);
		OLED_Data(0b11111100);OLED_Data(0b00001010);OLED_Data(0b00001001);OLED_Data(0b00001010);OLED_Data(0b11111100); 	//А
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);OLED_Data(0b00010000);OLED_Data(0b00010000);OLED_Data(0b00010000);OLED_Data(0b11111111); 	//Н
		OLED_Data(0b00000000);
		OLED_Data(0b11111100);OLED_Data(0b00001010);OLED_Data(0b00001001);OLED_Data(0b00001010);OLED_Data(0b11111100); 	//А
		OLED_Data(0b00000000);
		OLED_Data(0b10000000);OLED_Data(0b01111110);OLED_Data(0b00000001);OLED_Data(0b00000001);OLED_Data(0b11111111); 	//Л
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);																							//1
		OLED_Data(0b00000000);
		OLED_Data(0b00010000);OLED_Data(0b00010000);OLED_Data(0b00010000);												//-
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);OLED_Data(0b10001001);OLED_Data(0b10001001);OLED_Data(0b10001111);OLED_Data(0b11111000); 	//В
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);OLED_Data(0b10001000);OLED_Data(0b10001000);OLED_Data(0b10001000);OLED_Data(0b11110000); 	//Ы
		OLED_Data(0b00000000);OLED_Data(0b11111111);
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);OLED_Data(0b00010100);OLED_Data(0b00100010);OLED_Data(0b01000001);OLED_Data(0b10000000); 	//К
		OLED_Data(0b00000000);		
		OLED_Data(0b10000000);OLED_Data(0b01111110);OLED_Data(0b00000001);OLED_Data(0b00000001);OLED_Data(0b11111111); 	//Л
		OLED_Data(0b00000000);
		for(int i=0; i<63; i++){OLED_Data(0);}

		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);OLED_Data(0b00010100);OLED_Data(0b00100010);OLED_Data(0b01000001);OLED_Data(0b10000000); 	//К
		OLED_Data(0b00000000);
		OLED_Data(0b11111100);OLED_Data(0b00001010);OLED_Data(0b00001001);OLED_Data(0b00001010);OLED_Data(0b11111100); 	//A
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);OLED_Data(0b00010000);OLED_Data(0b00010000);OLED_Data(0b00010000);OLED_Data(0b11111111); 	//Н
		OLED_Data(0b00000000);
		OLED_Data(0b11111100);OLED_Data(0b00001010);OLED_Data(0b00001001);OLED_Data(0b00001010);OLED_Data(0b11111100); 	//A
		OLED_Data(0b00000000);
		OLED_Data(0b10000000);OLED_Data(0b01111110);OLED_Data(0b00000001);OLED_Data(0b00000001);OLED_Data(0b11111111); 	//Л
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);																							//2
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);																							
		OLED_Data(0b00000000);
		OLED_Data(0b00010000);OLED_Data(0b00010000);OLED_Data(0b00010000);												//-
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);OLED_Data(0b10001001);OLED_Data(0b10001001);OLED_Data(0b10001111);OLED_Data(0b11111000); 	//В
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);OLED_Data(0b10001000);OLED_Data(0b10001000);OLED_Data(0b10001000);OLED_Data(0b11110000); 	//Ы
		OLED_Data(0b00000000);OLED_Data(0b11111111);
		OLED_Data(0b00000000);
		OLED_Data(0b11111111);OLED_Data(0b00010100);OLED_Data(0b00100010);OLED_Data(0b01000001);OLED_Data(0b10000000); 	//К
		OLED_Data(0b00000000);		
		OLED_Data(0b10000000);OLED_Data(0b01111110);OLED_Data(0b00000001);OLED_Data(0b00000001);OLED_Data(0b11111111); 	//Л
		OLED_Data(0b00000000);
		for(int i=0; i<445; i++){OLED_Data(0);}
		twi_Stop();

/*инициализация портта и объявлениее переменных*/
		DDRD=0b11111001;
		unsigned int d;//под дребезг
		unsigned int position;//выбранная позиция меню
		position=0;
		int number1, number2;//состояние пунктов меню
		number1=0;
		number2=0;
	while(1){
			d=0;//каждый раз при начале бесконечного цикла сбрасывается в ноль

/*КНОПКА 1*/
		if ((PIND&0b00000100)==0b00000100){
			while(((PIND&0b00000100)==0b00000100)&&(d<1000)){d++;}
			if (d>999){
				position++;//увеличить позицию на один по нажатию
				if (position==3) {position=1;}//в конце последнего пункта вернуть на первый
			}
			while((PIND&0b00000100)==0b00000100){PORTD=PORTD;}

			twi_Start();
			twi_SendAdress();
			OLED_Command(0x20);
				OLED_Command(0x00);

			OLED_Command(0x21);
				OLED_Command(0);
				OLED_Command(1);

			OLED_Command(0x22);
				OLED_Command(3);
				OLED_Command(4);

			TWDR=0x40;
			TWCR=(1<<TWINT)|(1<<TWEN);		
			while(!(TWCR & (1<<TWINT))){};
			switch (position) {
				case 1:{
					OLED_Data(0b00011000);
					OLED_Data(0b00011000);
					OLED_Data(0b00000000);
					OLED_Data(0b00000000);																						
				}break;
				case 2:{
					OLED_Data(0b00000000);
					OLED_Data(0b00000000);
					OLED_Data(0b00011000);
					OLED_Data(0b00011000);
				}break;
				default:PORTD = PORTD;
				break;
			}
		}
			
/*КНОПКА 2*/
		if ((PIND&0b00000010)==0b00000010){
			while(((PIND&0b00000010)==0b00000010)&&(d<1000)){d++;}
				if (d>999){
					if (position==1){number1=number1+1; if (number1==3) {number1=1;}}
					if (position==2){number2=number2+1; if (number2==3) {number2=1;}}
				}
			while((PIND&0b00000010)==0b00000010){PORTD=PORTD;}

			twi_Start();
			twi_SendAdress();
			OLED_Command(0x20);
			OLED_Command(0x00);

			OLED_Command(0x21);
			OLED_Command(39);
			OLED_Command(127);

			OLED_Command(0x22);
			OLED_Command(3);
			OLED_Command(4);

			TWDR=0x40;
			TWCR=(1<<TWINT)|(1<<TWEN);		
			while(!(TWCR & (1<<TWINT))){};

			switch (number1) {
				case 1:{
					OLED_Data(0b11111111);OLED_Data(0b10001001);OLED_Data(0b10001001);OLED_Data(0b10001111);OLED_Data(0b11111000); 	//A
					OLED_Data(0b00000000);
					OLED_Data(0b11111111);OLED_Data(0b00010100);OLED_Data(0b00100010);OLED_Data(0b01000001);OLED_Data(0b10000000); 	//E
					OLED_Data(0b00000000);		
					OLED_Data(0b10000000);OLED_Data(0b01111110);OLED_Data(0b00000001);OLED_Data(0b00000001);OLED_Data(0b11111111); 	//E
					OLED_Data(0b00000000);
					for(int i=0; i<10; i++){OLED_Data(0);}
					PORTD=PORTD|0b00100000;//PD5
					};break;																								
				case 2:{
					OLED_Data(0b11111111);OLED_Data(0b10001001);OLED_Data(0b10001001);OLED_Data(0b10001111);OLED_Data(0b11111000); 	//A
					OLED_Data(0b00000000);
					OLED_Data(0b11111111);OLED_Data(0b10001000);OLED_Data(0b10001000);OLED_Data(0b10001000);OLED_Data(0b11110000); 	//U
					OLED_Data(0b00000000);OLED_Data(0b11111111);
					OLED_Data(0b00000000);
					OLED_Data(0b11111111);OLED_Data(0b00010100);OLED_Data(0b00100010);OLED_Data(0b01000001);OLED_Data(0b10000000); 	//E
					OLED_Data(0b00000000);		
					OLED_Data(0b10000000);OLED_Data(0b01111110);OLED_Data(0b00000001);OLED_Data(0b00000001);OLED_Data(0b11111111); 	//E
					OLED_Data(0b00000000);
					PORTD=PORTD&0b11011111;
					};break;
			}
			twi_Stop();

			twi_Start();
			twi_SendAdress();
			OLED_Command(0x20);
				OLED_Command(0x00);
			OLED_Command(0x21);
				OLED_Command(41);
				OLED_Command(127);
			OLED_Command(0x22);
				OLED_Command(4);
				OLED_Command(5);

			TWDR=0x40;
			TWCR=(1<<TWINT)|(1<<TWEN);		
			while(!(TWCR & (1<<TWINT))){};
			switch (number2) {
				case 1:{
					OLED_Data(0b11111111);OLED_Data(0b10001001);OLED_Data(0b10001001);OLED_Data(0b10001111);OLED_Data(0b11111000); 	//A
					OLED_Data(0b00000000);
					OLED_Data(0b11111111);OLED_Data(0b00010100);OLED_Data(0b00100010);OLED_Data(0b01000001);OLED_Data(0b10000000); 	//E
					OLED_Data(0b00000000);		
					OLED_Data(0b10000000);OLED_Data(0b01111110);OLED_Data(0b00000001);OLED_Data(0b00000001);OLED_Data(0b11111111); 	//E
					OLED_Data(0b00000000);
					for(int i=0; i<10; i++){OLED_Data(0);}
					PORTD=PORTD|0b00000001;
					};break;																								
				case 2:{
					OLED_Data(0b11111111);OLED_Data(0b10001001);OLED_Data(0b10001001);OLED_Data(0b10001111);OLED_Data(0b11111000); 	//A
					OLED_Data(0b00000000);
					OLED_Data(0b11111111);OLED_Data(0b10001000);OLED_Data(0b10001000);OLED_Data(0b10001000);OLED_Data(0b11110000); 	//U
					OLED_Data(0b00000000);OLED_Data(0b11111111);
					OLED_Data(0b00000000);
					OLED_Data(0b11111111);OLED_Data(0b00010100);OLED_Data(0b00100010);OLED_Data(0b01000001);OLED_Data(0b10000000); 	//E
					OLED_Data(0b00000000);		
					OLED_Data(0b10000000);OLED_Data(0b01111110);OLED_Data(0b00000001);OLED_Data(0b00000001);OLED_Data(0b11111111); 	//E
					OLED_Data(0b00000000);
					PORTD=PORTD&0b11111110;
					};break;
			}
		}
	}
}


