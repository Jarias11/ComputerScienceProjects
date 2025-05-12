// Cylinder class inherits from class Circle.
#ifndef CYLINDER_H
#define CYLINDER_H

#include "circle.h"  // Circle class definition

class Cylinder : public Circle {

public:

   // default constructor
   Cylinder( double = 0.0, double = 0.0 ); 
      
   void setHeight( double );  // set Cylinder's height
   double getHeight() const;  // return Cylinder's height 

   virtual double getArea() const; // return Cylinder's area
   virtual double getVolume() const; // return Cylinder's volume
   
   // return name of shape (i.e., "Cylinder" )
   virtual string getName() const;

   virtual void print() const;  // output Cylinder

private:
   double height;  // Cylinder's height

}; // end class Cylinder

Cylinder::Cylinder(double r, double h) : Circle(r)
{
    height = h;
}

void Cylinder::setHeight(double h)
{
    height = h;

} // set Cylinder's height

double Cylinder::getHeight() const
{
    return height;
} // return Cylinder's height

double Cylinder::getArea() const
{
    double area = 2.0 * Circle::getArea() + Circle::getCircumference() * height;
    return area;
} // return Cylinder's area
double Cylinder::getVolume() const
{
    return  Circle::getArea() * height;
} // return Cylinder's volume

// return name of shape (i.e., "Cylinder" )
string Cylinder::getName() const
{
    return "Cylinder";
}

void Cylinder::print() const
{
    cout << "Radius is " << getRadius() << " and height is " << height << " Area is " << getArea() << " and volume is " << getVolume() << endl;

} // output Cylinder

#endif



