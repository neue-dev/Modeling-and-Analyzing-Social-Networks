/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-24 18:04:46
 * @ Modified time: 2024-07-24 22:06:37
 * @ Description:
 * 
 * Utilities for managing bmp state while creating bmp files.
 */

#ifndef BMP_C
#define BMP_C

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BMP_HEADER_SIZE 14
#define BMP_INFOHEADER_SIZE 40

#define BMP_COLOR_BITS 24
#define BMP_COLOR_BYTES (BMP_COLOR_BITS / 8)
#define BMP_RESOLUTION_X 4800
#define BMP_RESOLUTION_Y 4800

typedef struct BMP BMP;

struct BMP {

  // The size of the buffer, in bytes
  uint32_t size;

  // The width and height of the buffer, in bytes
  uint32_t width;
  uint32_t height;

  // The offset of the pixel data from the start of the file
  // The current index in the array
  uint32_t offset;
  uint32_t ptr;

  // The array of data to store
  uint8_t *buffer;
};

/**
 * Encodes a 8-bit data point into the buffer.
 * 
 * @param   { BMP * }     pBMP      The bmp struct to use.
 * @param   { uint8_t }   i         The integer to encode.
*/
static inline void _BMP_encode8(BMP *pBMP, uint8_t i) {
  
  // Grab a reference to the pointer
  uint32_t *ptr = &pBMP->ptr;

  // Encode each of the bytes
  // By encoding this way, we also convert the int into little endian
  pBMP->buffer[(*ptr)++] = (uint8_t) (i & 0xff);
}

/**
 * Encodes a 16-bit data point into the buffer.
 * 
 * @param   { BMP * }     pBMP      The bmp struct to use.
 * @param   { uint16_t }  i         The integer to encode.
*/
static inline void _BMP_encode16(BMP *pBMP, uint16_t i) {
  
  // Grab a reference to the pointer
  uint32_t *ptr = &pBMP->ptr;

  // Encode each of the bytes
  // By encoding this way, we also convert the int into little endian
  pBMP->buffer[(*ptr)++] = (uint8_t) (i & 0xff); i >>= 8;
  pBMP->buffer[(*ptr)++] = (uint8_t) (i & 0xff);
}

/**
 * Encodes a 24-bit data point into the buffer.
 * Ignores the most largest portion of the number.
 * 
 * @param   { BMP * }     pBMP      The bmp struct to use.
 * @param   { uint32_t }  i         The integer to encode.
*/
static inline void _BMP_encode24(BMP *pBMP, uint32_t i) {
  
  // Grab a reference to the pointer
  uint32_t *ptr = &pBMP->ptr;

  // Encode each of the bytes
  // By encoding this way, we also convert the int into little endian
  pBMP->buffer[(*ptr)++] = (uint8_t) (i & 0xff); i >>= 8;
  pBMP->buffer[(*ptr)++] = (uint8_t) (i & 0xff); i >>= 8;
  pBMP->buffer[(*ptr)++] = (uint8_t) (i & 0xff);
}

/**
 * Encodes a 32-bit data point into the buffer.
 * 
 * @param   { BMP * }     pBMP      The bmp struct to use.
 * @param   { uint32_t }  i         The integer to encode.
*/
static inline void _BMP_encode32(BMP *pBMP, uint32_t i) {
  
  // Grab a reference to the pointer
  uint32_t *ptr = &pBMP->ptr;

  // Encode each of the bytes
  // By encoding this way, we also convert the int into little endian
  pBMP->buffer[(*ptr)++] = (uint8_t) (i & 0xff); i >>= 8;
  pBMP->buffer[(*ptr)++] = (uint8_t) (i & 0xff); i >>= 8;
  pBMP->buffer[(*ptr)++] = (uint8_t) (i & 0xff); i >>= 8;
  pBMP->buffer[(*ptr)++] = (uint8_t) (i & 0xff);
}

/**
 * Encodes the header of a BMP file.
 * 
 * @param   { BMP * }   pBMP  The bmp struct to use.
*/
static inline void _BMP_header(BMP *pBMP) {

  // The first two bytes
  _BMP_encode8(pBMP, 'B');
  _BMP_encode8(pBMP, 'M');

  // The size of the file
  _BMP_encode32(pBMP, pBMP->size);

  // Some optional stuff
  _BMP_encode32(pBMP, 0);

  // // The offset of the pixel data
  _BMP_encode32(pBMP, BMP_HEADER_SIZE + BMP_INFOHEADER_SIZE);
}

/**
 * Encodes the bmp info into the buffer.
 * 
 * @param   { BMP * }     pBMP      The bmp struct to update.
 * @param   { uint32_t }  width     The width of the image file.  
 * @param   { uint32_t }  height    The height of the image file.  
*/
static inline void _BMP_info(BMP *pBMP, uint32_t width, uint32_t height) {

  // The size of the info header
  _BMP_encode32(pBMP, BMP_INFOHEADER_SIZE);
  
  // The width and height
  _BMP_encode32(pBMP, width);
  _BMP_encode32(pBMP, height);

  // Number of planes (1)
  _BMP_encode16(pBMP, 1);

  // Bits per pixel
  _BMP_encode16(pBMP, BMP_COLOR_BITS);

  // Compression and image size, none by default
  // Image size can be 0 if compression is disabled
  _BMP_encode32(pBMP, 0);
  _BMP_encode32(pBMP, 0);
  
  // Horizontal and vertical resolution 
  _BMP_encode32(pBMP, BMP_RESOLUTION_X);
  _BMP_encode32(pBMP, BMP_RESOLUTION_Y);

  // Colors used and number of important colors
  // 0 means all colors are important
  _BMP_encode32(pBMP, 0);
  _BMP_encode32(pBMP, 0);
}

/**
 * Initializes the buffer for use for encoding data.
 * 
 * @param   { BMP * }     pBMP  The bmp struct to initialize.
 * @param   { uint32_t }  size  The size of the pixel data to allocate.
*/
void BMP_create(BMP *pBMP, uint32_t width, uint32_t height) {

  // The offset of the pixel data from the start
  uint32_t offset = 
    BMP_HEADER_SIZE + 
    BMP_INFOHEADER_SIZE;

  // Compute the actual size needed
  uint32_t totalSize = offset + width * height * BMP_COLOR_BYTES + 1;

  // Allocate memory for the buffer and pointer to current loc
  pBMP->buffer = calloc(totalSize, sizeof(uint8_t));
  pBMP->ptr = 0;
  
  // Set the size and other related deets
  pBMP->offset = offset;
  pBMP->width = width;
  pBMP->height = height;
  pBMP->size = totalSize;
  
  // Define the BMP header
  _BMP_header(pBMP);
  _BMP_info(pBMP, width, height);
}

/**
 * Encode a single pixel data into the file.
 * 
 * @param   { BMP * }     pBMP      The bmp struct to use for encoding.
 * @param   { int }       x         The x-coordinate of the pixel.  
 * @param   { int }       y         The y-coordinate of the pixel.  
 * @param   { uint32_t }  hexcode   The hexcode value to encode.  
 * 
*/
void BMP_encodePixel(BMP *pBMP, int x, int y, uint32_t hexcode) {

  // Set the pointer first, if coords are valid
  if(x >= 0 && y >= 0)
    pBMP->ptr = pBMP->offset + (y * pBMP->width + x) * BMP_COLOR_BYTES;

  // Encode the information at that point
  _BMP_encode24(pBMP, hexcode);
}

/**
 * Writes the buffer to a file.
 * 
 * @param   { BMP * }     pBMP      A pointer to the bmp struct to use.
 * @param   { char * }    filename  The name of the file to save.
*/
void BMP_writeFile(BMP *pBMP, char* filename) {

  // Open the file for writing
  FILE *pFile = fopen(filename, "wb");

  // Write the buffer data
  fwrite(pBMP->buffer, sizeof(uint8_t), pBMP->size, pFile);

  // Close the file
  fclose(pFile);
}

/**
 * Kills the buffer within current bmp instance.
 * Performs other cleanup (if needed) for BMP file.
 * 
 * @param   { BMP * }   pBMP  The bmp file to clean up.
*/
void BMP_kill(BMP *pBMP) {
  free(pBMP->buffer);
}

#endif