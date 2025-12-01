#include "stm32f10x.h"
#include "MyI2C.h"

static I2C_TypeDef * Horloge_I2c;

static char RTC_SLAVE = 0x68;

static void donothing() {
	MyI2C_Err_Enum err = MyI2C_Get_Error(Horloge_I2c);
}

// Setup I2C connection
void Horloge_init (I2C_TypeDef  * i2c ) {
	Horloge_I2c = i2c;
	MyI2C_Init(i2c, 5, donothing);
}

// for c between 0 and 9 inclusive
static char decimal_to_ascii(char c) {
	return c + 0x30;
}

// Get datetime in human string format
// MUST be called after Horloge_init
void Horloge_GetTimeString(char result[10]) {
	// Read data from registers 00h -> 06h
	char time_data[7]; 
	MyI2C_RecSendData_Typedef data = {
		.SlaveAdress7bits = RTC_SLAVE,
		.Ptr_Data = time_data,
		.Nb_Data = 7,
	};
	MyI2C_GetString(Horloge_I2c, 0x00, &data);
	
	const char seconds_01 = time_data[0] & 0x0F; // select last 4 bits
	const char seconds_10 = (time_data[0] & 0xF0) >> 4; // select first 4 bits 
	
	const char minutes_01 = time_data[1] & 0x0F; // select last 4 bits
	const char minutes_10 = (time_data[1] & 0xF0) >> 4; // select first 4 bits 
	
	const char hours_01 = time_data[2] & 0x0F; // select last 4 bits
	const char hours_10 = (time_data[2] & 0x10) >> 4; // select the thrid bit where the 10 hours is
	
	
	// "10h30 45s" (ends in 0x00)
	result[0] = decimal_to_ascii(hours_10);
	result[1] = decimal_to_ascii(hours_01);
	result[2] = 'h';
	result[3] = decimal_to_ascii(minutes_10);
	result[4] = decimal_to_ascii(minutes_01);
	result[5] = ' ';
	result[6] = decimal_to_ascii(seconds_10);
	result[7] = decimal_to_ascii(seconds_01);
	result[8] = 's';
	result[9] = 0;
}
