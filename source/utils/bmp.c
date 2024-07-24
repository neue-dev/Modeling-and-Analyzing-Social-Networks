/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-24 18:04:46
 * @ Modified time: 2024-07-25 02:58:17
 * @ Description:
 * 
 * Utilities for managing bmp state while creating bmp files.
 */

#ifndef BMP_C
#define BMP_C

#include "../utils/color.c"

#include <math.h>
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
 * @param   { BMP * }   pBMP  The bmp struct to use for encoding.
 * @param   { int }     x     The x-coordinate of the pixel.  
 * @param   { int }     y     The y-coordinate of the pixel.  
 * @param   { color }   c     The hexcode value to encode.  
 * 
*/
void BMP_encodePixel(BMP *pBMP, int x, int y, color c) {

  // Check pixel if in bounds
  if(x < 0 || x >= pBMP->width ||
    y < 0 || y >= pBMP->height)
    return;

  // Set the pointer first, if coords are valid
  pBMP->ptr = pBMP->offset + (y * pBMP->width + x) * BMP_COLOR_BYTES;

  // Encode the information at that point
  _BMP_encode24(pBMP, c);
}

/**
 * Blends the given value with the current value of the pixel.
 * 
 * @param   { BMP * }     pBMP      The bmp struct to use for encoding.
 * @param   { int }       x         The x-coordinate of the pixel.  
 * @param   { int }       y         The y-coordinate of the pixel.  
 * @param   { color }     c         The hexcode value to encode.
 * @param   { double }    amount    The amount of blending to do.
*/
void BMP_encodeBlend(BMP *pBMP, int x, int y, color c, double amount) {
  
  // Check pixel if in bounds
  if(x < 0 || x >= pBMP->width ||
    y < 0 || y >= pBMP->height)
    return;

  // Clamp the amount
  if(amount < 0) amount = 0;
  if(amount > 1) amount = 1;

  // Set the pointer first, if coords are valid
  pBMP->ptr = pBMP->offset + (y * pBMP->width + x) * BMP_COLOR_BYTES;

  // Read the values at the location
  int r = pBMP->buffer[pBMP->ptr + 0];
  int g = pBMP->buffer[pBMP->ptr + 1];
  int b = pBMP->buffer[pBMP->ptr + 2];
  int oldC = (r << 16) | (g << 8) | (b << 0);

  // Blend the colors
  c = Color_lerp(oldC, c, amount);

  // Encode the information at that point
  _BMP_encode24(pBMP, c);
}

/**
 * Writes a line into the buffer.
 * 
 * @param   { BMP * }     pBMP  The bitmap to modify.
 * @param   { int }       x1    The x-coordinate of the first point.  
 * @param   { int }       y1    The y-coordinate of the first point.
 * @param   { int }       x2    The x-coordinate of the second point.  
 * @param   { int }       y2    The y-coordinate of the second point.
 * @param   { color }     c     The color to encode.
 * @param   { double }    a     The alpha value of the line.
*/
void BMP_encodeLine(BMP *pBMP, int x1, int y1, int x2, int y2, color c, double a) {
  
  // Get the differences
  double dx = x2 - x1;
  double dy = y2 - y1;
  double m = dy / dx;
  double s = 0;
  int i = 0;

  // The drawing point
  double px = x1;
  double py = y1;

  // Get the step size
  if(fabs(dx) >= fabs(dy))
    s = fabs(dx);
  else
    s = fabs(dy);

  // Step size too small anyway
  if(s < 1)
    return;

  // Divide differences by step size
  dx /= s;
  dy /= s;

  // While we haven't reached the end
  while(i++ < s) {
    
    // Encode the pixel
    BMP_encodeBlend(pBMP, px, py, c, a);
    
    // March the line
    px += dx;
    py += dy;
  }

}

/**
 * Draws a circle on the bitmap.
 * 
 * @param   { BMP * }     pBMP  The bitmap to modify.
 * @param   { int }       x     The x-coordinate of the circle.  
 * @param   { int }       y     The y-coordinate of the circle.
 * @param   { double }    r     The radius of the circle.  
 * @param   { color }     c     The hexcode value to encode.  
 * @param   { double }    a     The alpha value of the circle.
*/
void BMP_encodeCircle(BMP *pBMP, int x, int y, double r, color c, double a) {

  // We go through the x and y values possibly in the circle
  for(int i = -r; i <= r; i++) {
    for(int j = -r; j <= r; j++) {

      // If not in circle
      if(i * i + j * j > r * r)
        continue;
      
      // Otherwise, shade it in
      BMP_encodeBlend(pBMP, x + i, y + j, c, a);
    }  
  }
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