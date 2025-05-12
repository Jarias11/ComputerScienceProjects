// Circle class 
#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"  // shape class definition
#include<iostream>
using namespace std;

class Circle : public Shape {

public:

   // default constructor
   Circle( double);  

   void setRadius( double );   // set radius
   double getRadius() const;   // return radius

   double getDiameter() const;       // return diameter
   double getCircumference() const;  // return circumference
   virtual double getArea() const;   // return area

   // return name of shape (i.e., "Circle")
   virtual string getName() const;
   // output Circle object data member
   virtual void print() const;  

private: 
   double radius;  // Circle's radius

}; // end class Circle

Circle::Circle(double r = 0.0)
{
    radius = r;
}

void Circle::setRadius(double r)
{
    radius = r;
}
double Circle::getRadius() const
{
    return radius;
}

double Circle::getDiameter() const
{
    return 2 * radius;
} // return diameter

double Circle::getCircumference() const
{
    return (2 * PI * radius);
} // return circumference

double Circle::getArea() const
{
    return (PI * radius * radius);
} // return area

// return name of shape (i.e., "Circle")
string Circle::getName() const
{
    return "Circle";
}
// output Circle object data member
void Circle::print() const
{
    cout << "Radius is " << radius << " Area is " << getArea() << " and circumference is " << getCircumference()<<endl;
}


#endif 
