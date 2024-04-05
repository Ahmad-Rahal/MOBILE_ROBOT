/*
 * pixyCam2.c
 *
 *  Created on: Jan 19, 2021
 *      Author: kerhoas
 */

#include "pixyCam2.h"


#define PIXY_DEFAULT_ARGVAL                  0x80000000
#define PIXY_BUFFERSIZE                      0x104
#define PIXY_CHECKSUM_SYNC                   0xc1af
#define PIXY_NO_CHECKSUM_SYNC                0xc1ae
#define PIXY_SEND_HEADER_SIZE                4
#define PIXY_MAX_PROGNAME                    33

#define PIXY_TYPE_REQUEST_CHANGE_PROG        0x02
#define PIXY_TYPE_REQUEST_RESOLUTION         0x0c
#define PIXY_TYPE_RESPONSE_RESOLUTION        0x0d
#define PIXY_TYPE_REQUEST_VERSION            0x0e
#define PIXY_TYPE_RESPONSE_VERSION           0x0f
#define PIXY_TYPE_RESPONSE_RESULT            0x01
#define PIXY_TYPE_RESPONSE_ERROR             0x03
#define PIXY_TYPE_REQUEST_BRIGHTNESS         0x10
#define PIXY_TYPE_REQUEST_SERVO              0x12
#define PIXY_TYPE_REQUEST_LED                0x14
#define PIXY_TYPE_REQUEST_LAMP               0x16
#define PIXY_TYPE_REQUEST_FPS                0x18

#define PIXY_RESULT_OK                       0
#define PIXY_RESULT_ERROR                    -1
#define PIXY_RESULT_BUSY                     -2
#define PIXY_RESULT_CHECKSUM_ERROR           -3
#define PIXY_RESULT_TIMEOUT                  -4
#define PIXY_RESULT_BUTTON_OVERRIDE          -5
#define PIXY_RESULT_PROG_CHANGING            -6

#define CCC_RESPONSE_BLOCKS                 0x21
#define CCC_REQUEST_BLOCKS                  0x20
#define CCC_SIG_ALL                  		0xff // all bits or'ed together

#define PIXY_DEBUG							1

// https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:general_api
// https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:ccc_api

uint8_t *m_buf;
uint8_t *m_bufPayload;
uint16_t frameWidth;
uint16_t frameHeight;
uint8_t m_length ;
uint8_t  m_type  ;
bool m_cs;

typedef struct
{
  uint16_t hardware;
  uint8_t firmwareMajor;
  uint8_t firmwareMinor;
  uint16_t firmwareBuild;
  char firmwareType[10];
}Version;

Version *version;

typedef struct
{
	  uint16_t m_signature;
	  uint16_t m_x;
	  uint16_t m_y;
	  uint16_t m_width;
	  uint16_t m_height;
	  int16_t m_angle;
	  uint8_t m_index;
	  uint8_t m_age;
} Block;

uint8_t  header[4];
uint8_t numBlocks;
Block *blocks;

//====================================================================
int pixyCam2_recv(uint8_t* buf, uint8_t length, uint16_t *cs)
{
	uint8_t i=0;

	 if (cs)
	 {*cs = 0;}

	while(i<length)
	{
		buf[i]=spi1Transfer(0x00);
	    if (cs)
	    { *cs += buf[i];}
		i++;
	}
	return length;
}
//====================================================================
int pixyCam2_recvByte(uint8_t* buf)
{
	return spi1Transfer(0x00);
}
//====================================================================
int pixyCam2_getSync()
{
	  uint8_t i, j, c, cprev;
	  int16_t res;
	  uint16_t start;

	  // parse bytes until we find sync
	  cprev=0;
	  j=0;
	  m_cs=true;

	  for(i=0 ; m_cs == true; i++)
	  {
	    res = pixyCam2_recv(&c, 1, NULL);
	    if (res>=PIXY_RESULT_OK)
	    {
	      // since we're using little endian, previous byte is least significant byte
	      start = cprev;
	      // current byte is most significant byte
	      start |= c << 8;
	      cprev = c;
	      if (start==PIXY_CHECKSUM_SYNC)
	      {
	        m_cs = true;
	        return PIXY_RESULT_OK;
	      }
	      if (start==PIXY_NO_CHECKSUM_SYNC)
	      {
	        m_cs = false;
	        return PIXY_RESULT_OK;
	      }
	    }
		  // If we've read some bytes and no sync, then wait and try again.
		  // And do that several more times before we give up.
		  // Pixy guarantees to respond within 100us.
	    if (i>=4)
	    {
	      if (j>=4)
	      {
	#ifdef PIXY_DEBUG
	        term_printf("error: no response \n\r");
	#endif
	        return PIXY_RESULT_ERROR;
	      }
	      HAL_Delay(25);
	      j++;
	      i = 0;
	    }
	  }
	  return 0;
}
//====================================================================
int pixyCam2_recvPacket()
{
	  uint16_t csCalc, csSerial;
	  int16_t res;

	  res = pixyCam2_getSync();

	  if (res<0)
	    return res;

	  if (m_cs)
	    {
	      res = pixyCam2_recv(m_buf, 4, NULL);
	      if (res<0)
	        return res;

	      m_type = m_buf[0];
	      m_length = m_buf[1];

	      csSerial = *(uint16_t *)&m_buf[2];

	      res = pixyCam2_recv(m_buf, m_length, &csCalc);
	      if (res<0)
	        return res;

	      if (csSerial!=csCalc)
	      {
	  #ifdef PIXY_DEBUG
	        term_printf("error: checksum \n\r");
	  #endif
	        return PIXY_RESULT_CHECKSUM_ERROR;
	      }
	    }
	    else
	    {
	      res = pixyCam2_recv(m_buf, 2, NULL);
	      if (res<0)
	        return res;

	      m_type = m_buf[0];
	      m_length = m_buf[1];

	      res = pixyCam2_recv(m_buf, m_length, NULL);
	      if (res<0)
	        return res;
	    }
	    return PIXY_RESULT_OK;
}

//====================================================================
int pixyCam2_sendPacket(uint8_t* buf_to_send, uint8_t type,uint8_t length)
{
	  // write header info at beginnig of buffer
	header[0] = PIXY_NO_CHECKSUM_SYNC&0xff;
	header[1] = PIXY_NO_CHECKSUM_SYNC>>8;
	header[2] = type;
	header[3] = length;
	  // send whole thing -- header and data in one call
	pixyCam2_send(header, PIXY_SEND_HEADER_SIZE);
	pixyCam2_send(buf_to_send, length);

	return 0;
}
//====================================================================
int pixyCam2_send(uint8_t* buf, uint8_t length)
{
	for( int i=0 ; i < length ; i++)
	{
		spi1Transfer(buf[i]);
	}
	return 0;
}
//====================================================================
void pixyCam2_Init()
{
	  // allocate buffer space for send/receive
	  m_buf = (uint8_t *)malloc(PIXY_BUFFERSIZE);
	  // shifted buffer is used for sending, so we have space to write header information
	  m_bufPayload = m_buf + PIXY_SEND_HEADER_SIZE;
	  frameWidth = frameHeight = 0;
	  version = NULL;
}
//====================================================================
uint8_t pixyCam2_setLED(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t res;

	  m_bufPayload[0] = r;
	  m_bufPayload[1] = g;
	  m_bufPayload[2] = b;
	  m_length = 3;
	  m_type = PIXY_TYPE_REQUEST_LED;
	  pixyCam2_sendPacket(m_bufPayload,m_type,m_length);
	  if (pixyCam2_recvPacket()==0 && m_type==PIXY_TYPE_RESPONSE_RESULT && m_length==4)
	  {
	    res = *(uint32_t *)m_buf;
	    return (int8_t)res;
	  }
	  else
	      return PIXY_RESULT_ERROR;  // some kind of bitstream error
}
//====================================================================
uint8_t pixyCam2_getBlocks(bool wait, uint8_t sigmap, uint8_t maxBlocks)
{
  blocks = NULL;
  numBlocks = 0;

  while(1)
  {
    // fill in request data
    m_bufPayload[0] = sigmap;
    m_bufPayload[1] = maxBlocks;
    m_length = 2;
    m_type = CCC_REQUEST_BLOCKS;

    // send request
    pixyCam2_sendPacket(m_bufPayload,m_type ,m_length);//m_pixy->sendPacket();

    if (pixyCam2_recvPacket()==0)
    {
      if (m_type==CCC_RESPONSE_BLOCKS)
      {
        blocks = (Block *)m_buf;
        numBlocks = m_length/sizeof(Block);
        return numBlocks;
      }
	  // deal with busy and program changing states from Pixy (we'll wait)
      else if (m_type==PIXY_TYPE_RESPONSE_ERROR)
      {
        if ((int8_t)m_buf[0]==PIXY_RESULT_BUSY)
        {
          if(!wait)
            return PIXY_RESULT_BUSY; // new data not available yet
		}
	    else if ((int8_t)m_buf[0]!=PIXY_RESULT_PROG_CHANGING)
          return m_buf[0];
      }
    }
    else
      return PIXY_RESULT_ERROR;  // some kind of bitstream error

    // If we're waiting for frame data, don't thrash Pixy with requests.
    // We can give up half a millisecond of latency (worst case)
    HAL_Delay(500);
  }
}
//====================================================================
uint8_t pixyCam2_getVersion()
{
	  m_length = 0;
	  m_type = PIXY_TYPE_REQUEST_VERSION;
	  pixyCam2_sendPacket(m_buf,m_type, m_length);

	  HAL_Delay(100);

	  if (pixyCam2_recvPacket()==0)
	  {
	    if (m_type==PIXY_TYPE_RESPONSE_VERSION)
	    {
	      version = (Version *)m_buf;
	      term_printf("%version hard : %x Maj:%x Min: %x \n\r", version -> hardware, version -> firmwareMajor, version ->firmwareMinor );
	      return m_length;
	    }
	    else if (m_type==PIXY_TYPE_RESPONSE_ERROR)
	      return PIXY_RESULT_BUSY;
	  }
	  return PIXY_RESULT_ERROR;  // some kind of bitstream error
}

//====================================================================
void pixyCam2_Test(void)
{

	pixyCam2_setLED(255,0,0);
	HAL_Delay(500);
	pixyCam2_setLED(0,255,0);
	HAL_Delay(500);
	pixyCam2_setLED(0,0,255);
	HAL_Delay(500);

	while(1)
	{
		pixyCam2_getBlocks(true,CCC_SIG_ALL,0xff);

		if(numBlocks > 0)
		{
			term_printf_stlink("signature = %d \n\r",blocks->m_signature);
			term_printf_stlink("x = %d \n\r",blocks->m_x);
			term_printf_stlink("y = %d \n\r",blocks->m_y);
			term_printf_stlink("width = %d \n\r",blocks->m_width);
			term_printf_stlink("height = %d \n\r",blocks->m_height);
			term_printf_stlink("angle = %d \n\r",blocks->m_angle);
		}
		HAL_Delay(500);
	}
}
//====================================================================
void pixyCam2_Get(uint16_t* tabXY,uint16_t* tabWH)
{
	pixyCam2_getBlocks(true,CCC_SIG_ALL,0xff);

	tabXY[0]=blocks->m_x;
	tabXY[1]=blocks->m_y;

	tabWH[0]=blocks->m_width;
	tabWH[1]=blocks->m_height;
}
//====================================================================
