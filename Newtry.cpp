#include "SPI.cpp"
#include "gpio-sysfs.h"
#include "gpio-sysfs.cpp"
using namespace std;

  unsigned int NRSTPD = 22;
  int ChipSelect=0;
  
  unsigned char MAX_LEN = 0x10;
  
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
  unsigned char PICC_SELECTTAG = 0x93;
  unsigned char PICC_AUTHENT1A = 0x60;
  unsigned char PICC_AUTHENT1B = 0x61;
  unsigned char PICC_READ      = 0x30;
  unsigned char PICC_WRITE     = 0xA0;
  unsigned char PICC_DECREMENT = 0xC0;
  unsigned char PICC_INCREMENT = 0xC1;
  unsigned char PICC_RESTORE   = 0xC2;
  unsigned char PICC_TRANSFER  = 0xB0;
  unsigned char PICC_HALT      = 0x50;
  
  unsigned char MI_OK       = 0;
  unsigned char MI_NOTAGERR = 1;
  unsigned char MI_ERR      = 2;
  
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
  unsigned char FIFOLevelReg   = 0x0A;
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
    
  unsigned char* serNum={};
class MFRC522 {
//unsigned char addr, val;
public:
void MFRC522_Reset();
void MFRC522_Init();
void Write_MFRC522(unsigned char addr, unsigned char val);
unsigned char Read_MFRC522(unsigned char addr);
void SetBitMask(unsigned char reg, unsigned char mask);
void ClearBitMask(unsigned char reg, unsigned char mask);
void AntennaOn();
void AntennaOff();
unsigned char MFRC522_ToCard(unsigned char command, unsigned char* sendData, unsigned char* backData);
unsigned char MFRC522_Request(unsigned char reqMode);
unsigned char MFRC522_Anticoll(unsigned char* backData);
unsigned char* CalulateCRC(unsigned char* pIndata);
unsigned char MFRC522_SelectTag(unsigned char* serNum);
unsigned char MFRC522_Auth(unsigned char authMode, unsigned char BlockAddr, unsigned char* Sectorkey, unsigned char* serNum);
void MFRC522_StopCrypto1();
void MFRC522_Read(unsigned char blockAddr);
void MFRC522_Write(unsigned char blockAddr, unsigned char* writeData);
void MFRC522_DumpClassic1K(unsigned char* key, unsigned char* uid);
};

void MFRC522::MFRC522_Reset(){
  Write_MFRC522(CommandReg, PCD_RESETPHASE);
} 

void MFRC522::MFRC522_Init(){
  gpio_export(NRSTPD);
  gpio_set_dir(NRSTPD,1);
  gpio_set_value(NRSTPD,1);
  SpiOpenPort(ChipSelect);
  MFRC522_Reset();
  Write_MFRC522(TModeReg, 0x8D);
  Write_MFRC522(TPrescalerReg, 0x3E);
  Write_MFRC522(TReloadRegL, 30);
  Write_MFRC522(TReloadRegH, 0);
  Write_MFRC522(TxAutoReg, 0x40);
  Write_MFRC522(ModeReg, 0x3D);
  AntennaOn();
} 

void MFRC522::Write_MFRC522(unsigned char addr, unsigned char val){
  unsigned char data[2];
  data[0]=((addr<<1)&0x7E);
  data[1]=val;
  SpiWriteAndRead(ChipSelect,data,2);
} unsigned char MFRC522::Read_MFRC522(unsigned char addr){
  unsigned char data[2];
  data[0]=((addr<<1)&0x7E) | 0x80;
  SpiWriteAndRead(ChipSelect,data,2);
  return data[0];
} 

void MFRC522::SetBitMask(unsigned char reg, unsigned char mask){
   unsigned char tmp = Read_MFRC522(reg);
   Write_MFRC522(reg, tmp | mask);
} 

void MFRC522::ClearBitMask(unsigned char reg, unsigned char mask){
    unsigned char tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & (~mask));
} 

void MFRC522::AntennaOn(){
    unsigned char temp = Read_MFRC522(TxControlReg);
    if(~(temp & 0x03)){
      SetBitMask(TxControlReg, 0x03);};
} 

void MFRC522::AntennaOff(){
    ClearBitMask(TxControlReg, 0x03);
} 
unsigned char MFRC522::MFRC522_ToCard(unsigned char command, unsigned char* sendData, unsigned char* backData ){
    //std::vector<unsigned char*> v;
    //unsigned char* backData = {};
    unsigned char backLen = 0;
    unsigned char status = MI_ERR;
    unsigned char irqEn = 0x00;
    unsigned char waitIRq = 0x00;
    bool lastBits = false;
    unsigned char n = 0;
    int i = 0;
    
    if (command == PCD_AUTHENT){
      irqEn = 0x12;
      waitIRq = 0x10;};
    if (command == PCD_TRANSCEIVE){
      irqEn = 0x77;
      waitIRq = 0x30;};
    
    Write_MFRC522(CommIEnReg, irqEn|0x80);
    ClearBitMask(CommIrqReg, 0x80);
    SetBitMask(FIFOLevelReg, 0x80);
    
    Write_MFRC522(CommandReg, PCD_IDLE); 
    
    while(i<sizeof(sendData)){
      Write_MFRC522(FIFODataReg, sendData[i]);
      i = i+1;}

    Write_MFRC522(CommandReg, command);
    if (command == PCD_TRANSCEIVE){
      SetBitMask(BitFramingReg, 0x80);}
    
    i = 2000;
    while (1){
    n = Read_MFRC522(CommIrqReg);
    i = i - 1;
    if (~((i!=0) and ~(n&0x01) and ~(n&waitIRq))){
    break;};
    }
    
    ClearBitMask(BitFramingReg, 0x80);
    
    if (i != 0){
      if ((Read_MFRC522(ErrorReg) & 0x1B)==0x00){
        status = MI_OK;}

      if (n & irqEn & 0x01){
          status = MI_NOTAGERR;}

      if (command == PCD_TRANSCEIVE){
          n = Read_MFRC522(FIFOLevelReg);
          lastBits = Read_MFRC522(ControlReg) & 0x07;
          if (lastBits != 0){
            backLen = (n-1)*8 + lastBits;}}
          else{
            backLen = n*8;}
      if (n == 0){
            n = 1;}

      if (n > MAX_LEN){
            n = MAX_LEN;}

      i = 0;
          while (i<n){
            backData[i]=(Read_MFRC522(FIFODataReg));
            i = i + 1;}
       }
    else{
        status = MI_ERR;}
    
     return status, backLen;
} 

unsigned char MFRC522::MFRC522_Request(unsigned char reqMode){
    unsigned char status = false;
    unsigned char backBits = false;
    unsigned char* TagType = {};
    unsigned char val1;
    TagType=&val1;
    unsigned char* backData = {};
    unsigned char val2;
    backData=&val2;
    
    Write_MFRC522(BitFramingReg, 0x07);
    
    TagType[0]=reqMode;
    (status,backBits) = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, backData);
    
    if (((status != MI_OK) | (backBits != 0x10))){
      status = MI_ERR;};

    return (status,backBits);
} 

unsigned char MFRC522::MFRC522_Anticoll(unsigned char* backData){
   //unsigned char* backData = {};
   int serNumCheck = 0;
   unsigned char* serNum = {};
   unsigned char status = false;
   unsigned char backBits = false;
   int i=0;
  
   Write_MFRC522(BitFramingReg, 0x00);
   
   serNum[0] = PICC_ANTICOLL;
   serNum[1] = (0x20);
   
   (status,backBits) = MFRC522_ToCard(PCD_TRANSCEIVE,serNum,backData);   
   
   if(status == MI_OK){
      i = 0;
      if (sizeof(backData)==5){
        while (i<4){
          serNumCheck = serNumCheck ^ backData[i];
          i = i + 1;}
        if (serNumCheck != backData[i]){
          status = MI_ERR;}}
      else{
        status = MI_ERR;}
      }
    return (status);}
    
    unsigned char* MFRC522::CalulateCRC(unsigned char* pIndata){
    ClearBitMask(DivIrqReg, 0x04);
    SetBitMask(FIFOLevelReg, 0x80);
    int i = 0;
    unsigned char n;
    while (i<sizeof(pIndata)){
      Write_MFRC522(FIFODataReg, pIndata[i]);
      i = i + 1;}

    Write_MFRC522(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    
    while (true){
      n = Read_MFRC522(DivIrqReg);
      i = i - 1;
      if (not ((i != 0) and not (n&0x04))){
        break;}}

    static unsigned char* pOutData = {};
    pOutData[0]=(Read_MFRC522(CRCResultRegL));
    pOutData[1]=(Read_MFRC522(CRCResultRegM));
    return pOutData;
} 
unsigned char MFRC522::MFRC522_SelectTag(unsigned char* serNum){
    unsigned char* backData = {};
    unsigned char* buf = {}; 
    unsigned char status = false;
    unsigned char backLen = false;
    
    buf[0]=(PICC_SELECTTAG);
    buf[1]=(0x70);
    
    int i = 0;
    while (i<5){
      buf[i+2]=(serNum[i]);
      i = i + 1;}
    
    unsigned char* pOut;
    pOut = CalulateCRC(buf);
    buf[7]=(*pOut);
    buf[8]=(*(pOut+1));
    
    (status, backLen) = MFRC522_ToCard(PCD_TRANSCEIVE, buf, backData);
    
    if ((status == MI_OK) and (backLen == 0x18)){
      cout<<"Size: " <<+(backData[0])<<endl;
      return    backData[0];}
    else{
      return 0;}
}
unsigned char MFRC522::MFRC522_Auth(unsigned char authMode, unsigned char BlockAddr, unsigned char* Sectorkey, unsigned char* serNum){
    unsigned char* buff = {};
    buff[0]=authMode;
    buff[1]=BlockAddr;
    
    int i = 0;
    while(i < sizeof(Sectorkey)){
      buff[i+2]=(Sectorkey[i]);
      i = i + 1;}
    
    i = 0;
    while(i < 4){
      buff[8+i]=(serNum[i]);
      i = i +1;}

    unsigned char* backData= {};
    unsigned char status = false;
    unsigned char backLen = false;
    (status, backLen) = MFRC522_ToCard(PCD_AUTHENT,buff, backData);
    
    if (not(status == MI_OK)){
      cout<<"AUTH ERROR!!"<<endl;}
    if (not (Read_MFRC522(Status2Reg) & 0x08) != 0){
      cout<<"AUTH ERROR(status2reg & 0x08) != 0"<<endl;}

    return status;}
    
    void MFRC522::MFRC522_StopCrypto1(){
   ClearBitMask(Status2Reg, 0x08);
} 

void MFRC522::MFRC522_Read(unsigned char blockAddr){
   unsigned char* recvData = {};
   recvData[0]=(PICC_READ);
   recvData[1]=(blockAddr);
   unsigned char* pOut;
   pOut = CalulateCRC(recvData);
   recvData[2]=(*pOut);
   recvData[3]=(*(pOut+1));
   unsigned char* backData={};
   unsigned char status = false;
   unsigned char backLen = false;
   (status, backLen) = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, backData);
   if (not(status == MI_OK)){
       cout<<"Error while reading!"<<endl;}
   int i = 0;
    if (sizeof(backData) == 16){
      cout<<"Sector "<<+(blockAddr)<<" "<<+(backData)<<endl;}
} 
void MFRC522::MFRC522_Write(unsigned char blockAddr, unsigned char* writeData){
   unsigned char* buff = {};
   buff[0]=(PICC_WRITE);
   buff[1]=(blockAddr);
   unsigned char* crc = {};
   crc = CalulateCRC(buff);
   buff[2]=(*crc);
   buff[3]=(*(crc+1));
   unsigned char* backData={};
   unsigned char status = false;
   unsigned char backLen = false;
   (status, backLen) = MFRC522_ToCard(PCD_TRANSCEIVE, buff, backData);
   
   if (not(status == MI_OK) or not(backLen == 4) or not((backData[0] & 0x0F) == 0x0A)){
        status = MI_ERR;}
   cout<<+backLen<<" backdata &0x0F == 0x0A "<<(+backData[0]&0x0F)<<endl;
   
   if (status == MI_OK){
    int i = 0;
    unsigned char* buf = {};
    while (i < 16){
            buf[i]=(writeData[i]);
            i = i + 1;}
    unsigned char* crc = {};
    crc = CalulateCRC(buf);
    buf[16]=(crc[0]);
    buf[17]=(crc[1]);
    unsigned char* backData={};
    unsigned char status = false;
    unsigned char backLen = false;
    (status, backLen) = MFRC522_ToCard(PCD_TRANSCEIVE,buf, backData);
    if (not(status == MI_OK) or not(backLen == 4) or not((backData[0] & 0x0F) == 0x0A)){
            cout<<"Error while writing"<<endl;}
    if (status == MI_OK){
            cout<<"Data written"<<endl;}
   }
} 
void MFRC522::MFRC522_DumpClassic1K(unsigned char* key, unsigned char* uid){
    int i = 0;
    unsigned char status = false;
    while (i < 64){
    status = MFRC522_Auth(PICC_AUTHENT1A, i, key, uid);
    if (status == MI_OK){
            MFRC522_Read(i);}
    else{
            cout<< "Authentication error"<<endl;}
        i = i+1;
    }
} 
int main(){
  int continue_reading = true;
  MFRC522 MIFAREReader;
  //Initializing the Reader
  gpio_export(NRSTPD);
  gpio_set_dir(NRSTPD,1);
  gpio_set_value(NRSTPD,1);
  SpiOpenPort(ChipSelect);
  unsigned char result;
  MIFAREReader.Write_MFRC522(CommandReg,PCD_RESETPHASE);
  result=MIFAREReader.Read_MFRC522(CommandReg);
  cout<<std::hex<<+result<<endl;
  //MIFAREReader.MFRC522_Init();
  MIFAREReader.Write_MFRC522(TModeReg,0x8D);
  result=MIFAREReader.Read_MFRC522(TModeReg);
  cout<<std::hex<<+result<<endl;

  MIFAREReader.Write_MFRC522(TPrescalerReg,0x3E);
  result=MIFAREReader.Read_MFRC522(TPrescalerReg);
  cout<<std::hex<<+result<<endl;

  MIFAREReader.Write_MFRC522(TReloadRegL,30);
  result=MIFAREReader.Read_MFRC522(TReloadRegL);
  cout<<std::hex<<+result<<endl;

  MIFAREReader.Write_MFRC522(TReloadRegH,0);
  result=MIFAREReader.Read_MFRC522(TReloadRegH);
  cout<<std::hex<<+result<<endl;

  MIFAREReader.Write_MFRC522(TxAutoReg,0x40);
  result=MIFAREReader.Read_MFRC522(TxAutoReg);
  cout<<std::hex<<+result<<endl;

  MIFAREReader.Write_MFRC522(ModeReg,0x3D);
  result=MIFAREReader.Read_MFRC522(ModeReg);
  cout<<std::hex<<+result<<endl;
 //Turning Antenna On
  unsigned char temp= MIFAREReader.Read_MFRC522(TxControlReg);
  if(~(temp & 0x03)){
   unsigned char tmp= MIFAREReader.Read_MFRC522(TxControlReg);
   MIFAREReader.Write_MFRC522(TxControlReg, (tmp & (~0x03)));
    }

  while(1){
  unsigned char status;
  unsigned char backBits; 
  unsigned char* TagType={};
  unsigned char val1;
  TagType=&val1;
  unsigned char* backData= {};
  unsigned char val2;
  backData=&val2;
  //while(1){
  MIFAREReader.Write_MFRC522(BitFramingReg, 0x07);
  TagType[0]=PICC_REQIDL;

   //To card function
    unsigned char command=PCD_TRANSCEIVE;
    unsigned char backLen=0;
    status=MI_ERR;
    unsigned char irqEn =0x00;
    unsigned char waitIRq =0x00;
    unsigned char lastBits=false;
    unsigned char n=0;
    int i=0;
  if(command == PCD_AUTHENT){
      irqEn= 0x12;
      waitIRq= 0x10;}
    if(command == PCD_TRANSCEIVE){
      irqEn= 0x77;
      waitIRq= 0x30;}
    MIFAREReader.Write_MFRC522(CommIEnReg, (irqEn|0x80));
    MIFAREReader.ClearBitMask(CommIrqReg, 0x80);
    MIFAREReader.SetBitMask(FIFOLevelReg,0x80);

    MIFAREReader.Write_MFRC522(CommandReg, PCD_IDLE);

   while(i<sizeof(TagType)){
   MIFAREReader.Write_MFRC522(FIFODataReg, TagType[i]);
   i=i+1;}

   MIFAREReader.Write_MFRC522(CommandReg, command);

   if(command == PCD_TRANSCEIVE){
   MIFAREReader.SetBitMask(BitFramingReg,0x80);}

   i=2000;
   while(1){
   n=MIFAREReader.Read_MFRC522(CommIrqReg);
   i=i-1;
   if((~((i!=0)) and (~(n&0x01)) and (~(n&waitIRq)))){
   break;}
   }
  MIFAREReader.ClearBitMask(BitFramingReg, 0x80);
  
  if(i != 0){
     if(((MIFAREReader.Read_MFRC522(ErrorReg)) & 0x1B)==0x00){
        status=MI_OK;
        if(n & irqEn & 0x01){
                status=MI_NOTAGERR;}
        if(command == PCD_TRANSCEIVE){
                n=MIFAREReader.Read_MFRC522(FIFOLevelReg);
                lastBits=(MIFAREReader.Read_MFRC522(ControlReg)) & 0x07;
                if(lastBits != 0){
                        backLen= ((n-1)*8)+lastBits;}
                else{
                        backLen=n*8;}

                if(n==0){
                        n=1;}
                if(n>MAX_LEN){
                        n=MAX_LEN;}
                i=0;
                while(i<n){
                        backData[i]=MIFAREReader.Read_MFRC522(FIFODataReg);
                        i=i+1;}}}
      else{
         status=MI_ERR;}}
    
    if(((status != MI_OK) | (backLen != 0x10))){
     status=MI_ERR;}

     cout<<+status<<endl;
     cout<<+backLen<<endl;
     }


  /*unsigned char status=0;
  unsigned char TagType;

  while(continue_reading){
      (status,TagType) = MIFAREReader.MFRC522_Request(PICC_REQIDL);
  if (status == MI_OK){
       cout<< "Card detected"<<endl;};
  }*/
 return 0;   
};
    
    
} 



