#include <fcntl.h>				//Needed for SPI port
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <unistd.h>			//Needed for SPI port
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>

int spi_cs0_fd;				//file descriptor for the SPI device
int spi_cs1_fd;				//file descriptor for the SPI device
unsigned char spi_mode;
unsigned char spi_bitsPerWord;
unsigned int spi_speed;




//***********************************
//***********************************
//********** SPI OPEN PORT **********
//***********************************
//***********************************
//spi_device	0=CS0, 1=CS1
int SpiOpenPort (int spi_device)
{
	int status_value = -1;
    int *spi_cs_fd;


    //----- SET SPI MODE -----
    //SPI_MODE_0 (0,0) 	CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
    //SPI_MODE_1 (0,1) 	CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
    //SPI_MODE_2 (1,0) 	CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
    //SPI_MODE_3 (1,1) 	CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge
    spi_mode = SPI_MODE_0;
    
    //----- SET BITS PER WORD -----
    spi_bitsPerWord = 8;
    
    //----- SET SPI BUS SPEED -----
    spi_speed = 1000000;		//1000000 = 1MHz (1uS per bit) 


    if (spi_device)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;


    if (spi_device)
    	*spi_cs_fd = open(std::string("/dev/spidev0.1").c_str(), O_RDWR);
    else
    	*spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);

    if (*spi_cs_fd < 0)
    {
        perror("Error - Could not open SPI device");
        exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
    if(status_value < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
    if(status_value < 0)
    {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }
    return(status_value);
}



//************************************
//************************************
//********** SPI CLOSE PORT **********
//************************************
//************************************
int SpiClosePort (int spi_device)
{
	int status_value = -1;
    int *spi_cs_fd;

    if (spi_device)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;


    status_value = close(*spi_cs_fd);
    if(status_value < 0)
    {
    	perror("Error - Could not close SPI device");
    	exit(1);
    }
    return(status_value);
}



//*******************************************
//*******************************************
//********** SPI WRITE & READ DATA **********
//*******************************************
//*******************************************
//data		Bytes to write.  Contents is overwritten with bytes read.
int SpiWriteAndRead (int spi_device, unsigned char *data, int length)
{
	struct spi_ioc_transfer spi[length];
	int i = 0;
	int retVal = -1;
    int *spi_cs_fd;

    if (spi_device)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;

	//one spi transfer for each byte

	for (i = 0 ; i < length ; i++)
	{
		memset(&spi[i], 0, sizeof (spi[i]));
		spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
		spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
		spi[i].len           = sizeof(*(data + i)) ;
		spi[i].delay_usecs   = 0 ;
		spi[i].speed_hz      = spi_speed ;
		spi[i].bits_per_word = spi_bitsPerWord ;
		spi[i].cs_change = 0;
	}

	retVal = ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(length), &spi) ;

	if(retVal < 0)
	{
		perror("Error - Problem transmitting spi data..ioctl");
		exit(1);
	}

	return retVal;
}


//#include <fcntl.h>				//Needed for SPI port
//#include <sys/ioctl.h>			//Needed for SPI port
//#include <linux/spi/spidev.h>	//Needed for SPI port
//#include <unistd.h>			//Needed for SPI port
//#include <stdio.h>
//#include <stdlib.h>
//#include <string>
//#include <iostream>
//#include <unistd.h>
//#include <cstring>
//#include "SPI.cpp"
#include "gpio-sysfs.h"
//#include "gpio-sysfs.cpp"
//using namespace std;

  unsigned int NRSTPD = 22;
  int ChipSelect=0;
  
  int MAX_LEN = 16;
  
  unsigned char PCD_IDLE       = 0x00;
  unsigned char PCD_AUTHENT    = 0x0E;
  unsigned char PCD_RECEIVE    = 0x08;
  unsigned char PCD_TRANSMIT   = 0x04;
  unsigned char PCD_TRANSCEIVE = 0x0C;
  unsigned char PCD_RESETPHASE = 0x0F;
  unsigned char PCD_CALCCRC    = 0x03;
  
  unsigned char PICC_REQIDL    = 0x26;
  unsigned char PICC_REQALL    = 0x52;
  unsigned char PICC_ANTICOLL  = 0x93;
  unsigned char PICC_SElECTTAG = 0x93;
  unsigned char PICC_AUTHENT1A = 0x60;
  unsigned char PICC_AUTHENT1B = 0x61;
  unsigned char PICC_READ      = 0x30;
  unsigned char PICC_WRITE     = 0xA0;
  unsigned char PICC_DECREMENT = 0xC0;
  unsigned char PICC_INCREMENT = 0xC1;
  unsigned char PICC_RESTORE   = 0xC2;
  unsigned char PICC_TRANSFER  = 0xB0;
  unsigned char PICC_HALT      = 0x50;
  
  char MI_OK       = 0;
  char MI_NOTAGERR = 1;
  char MI_ERR      = 2;
  
  unsigned char Reserved00     = 0x00;
  unsigned char CommandReg     = 0x01;
  unsigned char CommIEnReg     = 0x02;
  unsigned char DivlEnReg      = 0x03;
  unsigned char CommIrqReg     = 0x04;
  unsigned char DivIrqReg      = 0x05;
  unsigned char ErrorReg       = 0x06;
  unsigned char Status1Reg     = 0x07;
  unsigned char Status2Reg     = 0x08;
  unsigned char FIFODataReg    = 0x09;
  unsigned char  FIFOLevelReg   = 0x0A;
  unsigned char WaterLevelReg  = 0x0B;
  unsigned char ControlReg     = 0x0C;
  unsigned char BitFramingReg  = 0x0D;
  unsigned char CollReg        = 0x0E;
  unsigned char Reserved01     = 0x0F;
  
  unsigned char Reserved10     = 0x10;
  unsigned char ModeReg        = 0x11;
  unsigned char TxModeReg      = 0x12;
  unsigned char RxModeReg      = 0x13;
  unsigned char TxControlReg   = 0x14;
  unsigned char TxAutoReg      = 0x15;
  unsigned char TxSelReg       = 0x16;
  unsigned char RxSelReg       = 0x17;
  unsigned char RxThresholdReg = 0x18;
  unsigned char DemodReg       = 0x19;
  unsigned char Reserved11     = 0x1A;
  unsigned char Reserved12     = 0x1B;
  unsigned char MifareReg      = 0x1C;
  unsigned char Reserved13     = 0x1D;
  unsigned char Reserved14     = 0x1E;
  unsigned char SerialSpeedReg = 0x1F;
  
  unsigned char Reserved20        = 0x20; 
  unsigned char CRCResultRegM     = 0x21;
  unsigned char CRCResultRegL     = 0x22;
  unsigned char Reserved21        = 0x23;
  unsigned char ModWidthReg       = 0x24;
  unsigned char Reserved22        = 0x25;
  unsigned char RFCfgReg          = 0x26;
  unsigned char GsNReg            = 0x27;
  unsigned char CWGsPReg          = 0x28;
  unsigned char ModGsPReg         = 0x29;
  unsigned char TModeReg          = 0x2A;
  unsigned char TPrescalerReg     = 0x2B;
  unsigned char TReloadRegH       = 0x2C;
  unsigned char TReloadRegL       = 0x2D;
  unsigned char TCounterValueRegH = 0x2E;
  unsigned char TCounterValueRegL = 0x2F;
  
  unsigned char Reserved30      = 0x30;
  unsigned char TestSel1Reg     = 0x31;
  unsigned char TestSel2Reg     = 0x32;
  unsigned char TestPinEnReg    = 0x33;
  unsigned char TestPinValueReg = 0x34;
  unsigned char TestBusReg      = 0x35;
  unsigned char AutoTestReg     = 0x36;
  unsigned char VersionReg      = 0x37;
  unsigned char AnalogTestReg   = 0x38;
  unsigned char TestDAC1Reg     = 0x39;
  unsigned char TestDAC2Reg     = 0x3A;
  unsigned char TestADCReg      = 0x3B;
  unsigned char Reserved31      = 0x3C;
  unsigned char Reserved32      = 0x3D;
  unsigned char Reserved33      = 0x3E;
  unsigned char Reserved34      = 0x3F;
    
  char serNum;
  
class MFRC522 {
unsigned char addr, val;
public:
void MFRC522_Reset();
void MFRC522_Init();
void Write_MFRC522(unsigned char addr, unsigned char val);
unsigned char Read_MFRC522(unsigned char addr, unsigned char val);
};

void MFRC522::MFRC522_Reset(){
  Write_MFRC522(CommandReg, PCD_RESETPHASE);
} 

void MFRC522::MFRC522_Init(){
  gpio_export(NRSTPD);
  gpio_set_dir(NRSTPD,1);
  gpio_set_value(NRSTPD,1);
  SpiOpenPort(ChipSelect);
} 

void MFRC522::Write_MFRC522(unsigned char addr, unsigned char val){
  unsigned char data[2];
  data[0]=((addr<<1)&0x7E);
  data[1]=val;
  SpiWriteAndRead(ChipSelect,data,2);
} 

unsigned char MFRC522::Read_MFRC522(unsigned char addr, unsigned char val){
  unsigned char data[2];
  data[0]=((addr<<1)&0x7E) | 0x80;
  SpiWriteAndRead(ChipSelect,data,2);
  return data[0];
} 

int main(){
  MFRC522 rect;
  rect.MFRC522_Init();
 return 0;   
};
