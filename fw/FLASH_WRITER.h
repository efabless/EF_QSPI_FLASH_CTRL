#ifndef FLASH_WRITER_H
#define FLASH_WRITER_H

//#define USE_GETTERS

#define     FW_BASE                                 0x22000000

#define     FW_WE_REG_ADDR                          (FW_BASE + 0x00000000)
#define     FW_SS_REG_ADDR                          (FW_BASE + 0x00000004)
#define     FW_SCK_REG_ADDR                         (FW_BASE + 0x00000008)
#define     FW_OE_REG_ADDR                          (FW_BASE + 0x0000000C)
#define     FW_SO_REG_ADDR                          (FW_BASE + 0x00000010)
#define     FW_SI_REG_ADDR                          (FW_BASE + 0x00000014)
#define     FW_ID_REG_ADDR                          (FW_BASE + 0x00000018)

void FW_setWEreg(int value){
    (*(volatile int*)FW_WE_REG_ADDR) = value;
}

#ifdef USE_GETTERS
int FW_getWEreg(){
    return (*(volatile int*)FW_WE_REG_ADDR);
}
#endif

void FW_setSSreg(int value){
    (*(volatile int*)FW_SS_REG_ADDR) = value;
}

#ifdef USE_GETTERS

int FW_getSSreg(){
    return (*(volatile int*)FW_SS_REG_ADDR);
}
#endif

void FW_setSCKreg(int value){
    (*(volatile int*)FW_SCK_REG_ADDR) = value;
}

#ifdef USE_GETTERS
int FW_getSCKreg(){
    return (*(volatile int*)FW_SCK_REG_ADDR);
}
#endif

void FW_setOEreg(int value){
    (*(volatile int*)FW_OE_REG_ADDR) = value;
}

#ifdef USE_GETTERS
int FW_getOEreg(){
    return (*(volatile int*)FW_OE_REG_ADDR);
}
#endif

void FW_setSOreg(int value){
    (*(volatile int*)FW_SO_REG_ADDR) = value;
}

#ifdef USE_GETTERS
int FW_getSOreg(){
    return (*(volatile int*)FW_SO_REG_ADDR);
}

void FW_setSIreg(int value){
    (*(volatile int*)FW_SI_REG_ADDR) = value;
}

#endif

int FW_getSIreg(){
    return (*(volatile int*)FW_SI_REG_ADDR);
}


#ifdef USE_GETTERS

int FW_getIDreg(){
    return (*(volatile int*)FW_ID_REG_ADDR);
}
#endif

void FW_enable () {

    FW_setOEreg (0x1);
    FW_setWEreg (0xA5A85501); // a number which enables the flash writer

}

void SPI_start () {

    FW_setOEreg (0x1);
    FW_setSSreg (0x0);
}

void SPI_stop () {

    FW_setSSreg (0x1);

}

void SPI_byteWrite (char byte) {
    int bit;
    for (int i = 7; i >= 0; i--) {
        bit = (byte >> i) & 1;
        FW_setSOreg (bit);
        FW_setSCKreg (1);
        FW_setSCKreg (0);
    }
   
}


char SPI_byteRead() {
    unsigned int word = 0;
    unsigned int byte = 0;
    unsigned char bit = 0;

    for (int i = 7; i >= 0; i--) {
        FW_setSCKreg(1);
        word = FW_getSIreg();
        bit = word & 0x01;
        FW_setSCKreg(0);

        if (bit) {
            byte |= (1 << i);
        }
    }

    return (char)byte;
}

void FLASH_writeEnable(){
    SPI_start();
    SPI_byteWrite(0x06);
    SPI_stop();
}

void FLASH_blockErase(int addr) {

    FW_setOEreg (0x1);
    SPI_start();
    SPI_byteWrite(0xD8);

    SPI_byteWrite((addr >> 16) & 0xFF);
    SPI_byteWrite((addr >> 8) & 0xFF);
    SPI_byteWrite(addr & 0xFF);

    SPI_stop();
}

void FLASH_ChipErase() {

    FW_setOEreg (0x1);
    SPI_start();
    SPI_byteWrite(0xC7);
    SPI_stop();
}


#endif
