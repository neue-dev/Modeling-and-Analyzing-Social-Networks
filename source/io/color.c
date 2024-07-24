/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-24 20:54:05
 * @ Modified time: 2024-07-24 21:36:54
 * @ Description:
 * 
 * Utilities for manipulating colors.
 */

#ifndef COLOR_C
#define COLOR_C

#include <stdint.h>

typedef int color;

/**
 * Gets the red channel of the color.
 * 
 * @param   { color }   c   The color to get the red channel of.
 * @return  { int }         The red channel value of the color.
*/
static inline int _Color_r(color c) {
  return (c >> 16) & 0xff;
};

/**
 * Gets the green channel of the color.
 * 
 * @param   { color }   c   The color to get the green channel of.
 * @return  { int }         The green channel value of the color.
*/
static inline int _Color_g(color c) {
  return (c >> 8) & 0xff;
};

/**
 * Gets the blue channel of the color.
 * 
 * @param   { color }   c   The color to get the blue channel of.
 * @return  { int }         The blue channel value of the color.
*/
static inline int _Color_b(color c) {
  return (c >> 0) & 0xff;
};

/**
 * Returns the hex color associated with the set of rgb values.
 * 
 * @param   { int }   r   The red channel value. 
 * @param   { int }   g   The green channel value. 
 * @param   { int }   b   The blue channel value. 
 * @return  { int }       The hex value associated with the provided RGB.
*/
color Color_fromRGB(int r, int g, int b) {
  
  // In case some are negative
  if(r < 0) b = g = r = 0;
  if(g < 0) b = g = r;
  if(b < 0) b = g;

  // Convert to hex
  return 
    ((r & 0xff) << 16) |
    ((g & 0xff) << 8) |
    ((b & 0xff) << 0);
}

/**
 * Lerps between two colors by the specified amount.
 * 0 means 100% of color1 while 1 means 100% of color2.
 * 
 * @param   { color }   color1  The first color to use.
 * @param   { color }   color2  The second color to use.
 * @param   { double }  amount  The amount to lerp the two colors.
 * @return  { color }           The lerped color.
*/
color Color_lerp(color color1, color color2, double amount) {

  // Rgb blending
  int r = _Color_r(color1) * (1.0 - amount) + _Color_r(color2) * (amount);
  int g = _Color_g(color1) * (1.0 - amount) + _Color_g(color2) * (amount);
  int b = _Color_b(color1) * (1.0 - amount) + _Color_b(color2) * (amount);

  // Convert to hex
  return Color_fromRGB(r, g, b);
}

#endif