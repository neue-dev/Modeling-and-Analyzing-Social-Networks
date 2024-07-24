/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-24 22:43:34
 * @ Modified time: 2024-07-24 22:59:58
 * @ Description:
 * 
 * A vector class.
 * Helps us with manipulating vector values.
 */

#ifndef POINT_C
#define POINT_C

typedef struct Point Point;

struct Point {
  double x;
  double y;
  double w;

  double fx;
  double fy;
};

/**
 * Initializes the point with the given values.
 * A point can experience forces and have velocity.
 * 
 * @param   { double }  x   The x-coordinate of the point.
 * @param   { double }  y   The y-coordinate of the point.
 * @param   { double }  w   The weight of the point.
*/
void Point_init(Point *this, double x, double y, double w) {

  // The location of the point
  this->x = x;
  this->y = y;
  this->w = w;

  // The forces to apply
  this->fx = 0;
  this->fy = 0;
}

/**
 * Sets the existing force values to the given ones.
 * 
 * @param   { int }   fx  The x value of the force.
 * @param   { int }   fy  The y value of the force.
*/
void Point_setForce(Point *this, double fx, double fy) {
  this->fx = fx;
  this->fy = fy;
}

/**
 * Adds the given force values to the existing ones.
 * 
 * @param   { int }   fx  The x value of the force.
 * @param   { int }   fy  The y value of the force.
*/
void Point_addForce(Point *this, double fx, double fy) {
  this->fx += fx;
  this->fy += fy;
}

/**
 * Moves the given point based on its acting forces and weight.
 * 
 * @param   { Point * }   this  The point to move.
*/
void Point_move(Point *this) {
  this->x += this->fx / this->w;
  this->y += this->fy / this->w;
}

/**
 * Gets the distance along the x-axis of the point to the given location.
 * 
 * @param   { Point * }   this  The point to get the dist of.
 * @param   { double }    x     The location to get the dist with.
 * @return  { double }          The x distance of the point.
*/
double Point_getXDist(Point *this, double x) {
  return x - this->x;
}

/**
 * Gets the distance along the y-axis of the point to the given location.
 * 
 * @param   { Point * }   this  The point to get the dist of.
 * @param   { double }    y     The location to get the dist with.
 * @return  { double }          The y distance of the point.
*/
double Point_getXDist(Point *this, double y) {
  return y - this->y;
}

#endif