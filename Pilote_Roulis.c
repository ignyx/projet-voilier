#include "stm32f10x.h"
#include "MySPI.h"

static const char SPI_WRITE = 0x0 << 7;
static const char SPI_READ = 0x1 << 7;
static const char SPI_MULTIPLE_BYTES = 0x1 << 6;
static const char AD_POWER_CTL_D3_MEASURE = 1 << 3;
static const char AD_POWER_CTL = 0x2D;
static const char AD_DATA_FORMAT = 0x31;
static const char AD_DATA_FORMAT_FULL_RES = 0x1 << 3;
static const char AD_DATA_FORMAT_RANGE_16G = 0x3;
static const char AD_BW_RATE = 0x2C;
static const char AD_DATAX0 = 0x32;
static const char AD_DATAY0 = 0x34;


void Roulis_init ( SPI_TypeDef * spi ) {
	MySPI_Init (spi);
	
	// Write to POWER_CTL and enable measurement
	MySPI_Clear_NSS();
	MySPI_Send(SPI_WRITE | AD_POWER_CTL);
	MySPI_Send(AD_POWER_CTL_D3_MEASURE);
	MySPI_Set_NSS();
	
	// Measurement frequency defaults to 100Hz, so no need to explicitly configure it.
	
	// Set measurement range to "Full Resolution" with +/- 16g range
	MySPI_Clear_NSS();
	MySPI_Send(SPI_WRITE | AD_DATA_FORMAT);
	MySPI_Send(AD_DATA_FORMAT_FULL_RES | AD_DATA_FORMAT_RANGE_16G);
	MySPI_Set_NSS();
}



// Returns 1 if abs(angle) > 45 deg
int Roulis_isBoatInBadShape() {
	MySPI_Clear_NSS();
	// Read data for all three axes
	MySPI_Send(SPI_READ | AD_DATAX0 | SPI_MULTIPLE_BYTES);
	const short x = MySPI_Read() | MySPI_Read() << 8 ;
	const short y = MySPI_Read() | MySPI_Read() << 8 ;
	const short z = MySPI_Read() | MySPI_Read() << 8 ;
	MySPI_Set_NSS();
	
	return y*y > z*z; // Implies angle > 45 deg
}