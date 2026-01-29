/*
 * pixyCam2.h
 *
 *  Created on: Jan 19, 2021
 *      Author: kerhoas
 */

#ifndef PIXYCAM2_H_
#define PIXYCAM2_H_

#include "drv_spi.h"
#include <stdbool.h>

int pixyCam2_recv(uint8_t* buf, uint8_t length, uint16_t *cs);
int pixyCam2_recvByte(uint8_t* buf);
int pixyCam2_getSync();
int pixyCam2_recvPacket();
int pixyCam2_sendPacket(uint8_t* buf_to_send, uint8_t type,uint8_t length);
int pixyCam2_send(uint8_t* buf, uint8_t length);
void pixyCam2_Init();
uint8_t pixyCam2_setLED(uint8_t r, uint8_t g, uint8_t b);
uint8_t pixyCam2_getBlocks(bool wait, uint8_t sigmap, uint8_t maxBlocks);
uint8_t pixyCam2_getVersion();
void pixyCam2_Test(void);
void pixyCam2_Get(uint16_t* tabXY,uint16_t* tabWH);



#endif /* PIXYCAM2_H_ */
