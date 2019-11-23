// this file contain the io driver of an sd card, implemented for freertos


#ifndef _SD_SPI_IO_H_
#define _SD_SPI_IO_H_

#include "diskio.h"
#include "stdint.h"



/* Definitions for MMC/SDC command */
#define CMD0     (0x40+0)     /* GO_IDLE_STATE */
#define CMD1     (0x40+1)     /* SEND_OP_COND */
#define CMD8     (0x40+8)     /* SEND_IF_COND */
#define CMD9     (0x40+9)     /* SEND_CSD */
#define CMD10    (0x40+10)    /* SEND_CID */
#define CMD12    (0x40+12)    /* STOP_TRANSMISSION */
#define CMD13    (0x40+13)    /* SEND STATUS*/
#define CMD16    (0x40+16)    /* SET_BLOCKLEN */
#define CMD17    (0x40+17)    /* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    /* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    /* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    /* WRITE_BLOCK */
#define CMD25    (0x40+25)    /* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    /* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    /* APP_CMD */
#define CMD58    (0x40+58)    /* READ_OCR */


#define SD_TRANSFER_CPLT_SIGNAL (1<<0)

DSTATUS sd_powerOn(uint8_t diskNo);

DSTATUS sd_initialize(uint8_t diskNo);

DSTATUS sd_status(uint8_t diskNo);

DRESULT sd_read(uint8_t diskno,uint8_t* buff,uint32_t sector,uint32_t lbano);

DRESULT sd_write(uint8_t diskno,uint8_t* buff,uint32_t sector,uint32_t lbano);

#endif //_SD_SPI_IO_H_
