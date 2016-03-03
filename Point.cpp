//
//  Point.cpp
//  PA2
//
//  Created by Alexander Flaska on 2/24/16.
//  Copyright © 2016 Alexander Flaska. All rights reserved.
//

#include "Point.h"

#include <cmath>
#include <algorithm> //min, max
#include <string>
#include <sstream>
#include <iostream>

#include "Point.h"

using namespace std;
using namespace Clustering;

namespace Clustering
{
    
    unsigned int Point::__idGen = 0;
    
    Point::Point(int dimensions)// default constructor; creates id, increments, and makes a new array.
    {
        __id = ++__idGen;
        
        
        __dim = dimensions;
        __values = new double[__dim];
        for (int pos = 0; pos < __dim; ++pos)
        {
            __values[pos] = 0.0;
        }
    }
    
    Point::Point(int i, double *array)// construtor that already has an array passed, so it just assigns and inrements.
    {
        __dim = i;
        __values = array;
        __id = __idGen++;
    }
    
    Point::Point(const Point &point) // copy constructor. Does everything the default does. 
    {
        __dim = point.__dim;
        __values = new double[__dim];
        __id = point.__id;
        
        for (int pos = 0; pos < __dim; ++pos)
            __values[pos] = point.__values[pos];
        
        
        
    }
    
    Point &Point::operator=(const Point &point)// overloaded equals operator
    {
        if (&point == this)// returns if it's the thing it's equal to.
        {
            return *this;
        }
        else
        {
            for (int pos = 0; pos < __dim; ++pos)// loops and assigns all pointers in array to the array
            {
                this->__values[pos] = point.getValue(pos);
            }
            
            return *this;
        }
    }
    
    double Point::distanceTo(const Point &point) const// distance to function. pythagorean theorem.
    {
        double totalDistance = 0.0;
        for (int pos = 0; pos < __dim; ++pos)
        {
            totalDistance += pow((__values[pos] - point.__values[pos]), 2);
        }
        
        totalDistance = sqrt(totalDistance);
        
        return totalDistance;
    }

    Point &Point::operator*=(double d)// overloaded *= operator.
    {
        for (int pos = 0; pos < __dim; ++pos)// loops, and *='s everything, and then returns pointer.
            __values[pos] *= d;
        return *this;
    }
    
    Point &Point::operator/=(double d)// overloaded /= operators.
    {
        for (int pos = 0; pos < __dim; ++pos)// same as above, except / instead of *
            __values[pos] /= d;
        return *this;
    }
    
    const Point Point::operator*(double d) const // overloaded * operator
    {
        Point newPoint(*this);// makes a new point, multiplies it, and returns, using overloaded *=
        newPoint *= d;
        
        return newPoint;
    }
    
    const Point Point::operator/(double d) const // overloaded / operator
    {
        Point newPoint(*this); // new point, divides, and returns using overloaded /=
        newPoint /= d;
        
        return newPoint;
    }
  
    double &Point::operator[](int index)// just returns the array at indices.
    {
        return __values[index];
    }
    
    Point &operator+=(Point &leftPoint, const Point &rightPoint)// overloaded +=
    {
        for (int pos = 0; pos < leftPoint.__dim; ++pos)// loops, and +='s left to right, returns left.
            leftPoint.__values[pos] += rightPoint.__values[pos];
        
        return leftPoint;
    }
    
    Point &operator-=(Point &leftPoint, const Point &rightPoint)// overloaded-=
    {
        for (int pos = 0; pos < leftPoint.__dim; ++pos)// loops, and -='s left to right, returns left.
            leftPoint.__values[pos] -= rightPoint.__values[pos];
        
        return leftPoint;
    }
    
    const Point operator+(const Point &leftPoint, const Point &rightPoint)// overloaded +
    {
        Point newPoint(leftPoint);// new point, returns newpoint+= rightpoint using overloaded +=
        
        return newPoint+=rightPoint;
    }
    
    const Point operator-(const Point &leftPoint, const Point &rightPoint)// overloaded -
    {
        Point newPoint(leftPoint);// new point using leftpoing, returns it -=right point using overloaded-=
        
        return newPoint -= rightPoint;
    }
    
    bool operator==(const Point &leftPoint, const Point &rightPoint)// overloaded ==
    {
        if (leftPoint.__id != rightPoint.__id)// if not the same, false.
            return false;
        
        for (int i = 0; i < std::max(leftPoint.__dim, rightPoint.__dim); ++i)// loop through to make the same check.
        {
            if (leftPoint.getValue(i) != rightPoint.getValue(i))
                return false;
        }
        
        // ID's are the same, values are the same
        return true;// if not false, must be true.
    }
    
    bool operator!=(const Point &leftPoint, const Point &rightPoint)
    {
        return(!(leftPoint == rightPoint));// returns if not = to each other.
    }
    
    bool operator<(const Point &leftPoint, const Point &rightPoint)// overloaded <
    {
        
        if (leftPoint != rightPoint)// if they're not equal
        {
            if (leftPoint.__dim == rightPoint.__dim)// check if they're equal
            {
                for (int i = 0; i < leftPoint.__dim; ++i)// loop and return the appropriate bool based on what's greater
                {
                    if (leftPoint.__values[i] > rightPoint.__values[i])
                    return false;
                    
                    else if (leftPoint.__values[i] < rightPoint.__values[i])
                    return true;
                    
                }
            }
        }
        return false;
    }
    
    bool operator>(const Point &leftPoint, const Point &rightPoint)// overloaded >
    {
        return operator<(rightPoint, leftPoint);// using overloaded < just return
    }
    
    bool operator<=(const Point &leftPoint, const Point &rightPoint)// overloaded <= using overloaded operators
    {
        return !(leftPoint > rightPoint);
    }
    
    bool operator>=(const Point &leftPoint, const Point &rightPoint)// overloaded >= using overloaded operators
    {
        return !(leftPoint<rightPoint);
    }
    
    std::ostream &operator<<(ostream &outputStream, const Point &point)// overloaded << operator
    {
        int pos;
        for (pos = 0; pos < point.__dim-1; ++pos)// loops
            outputStream << point.__values[pos] << ", ";// opens ostream with the values in __values
            outputStream << point.__values[pos];// makes sure there are commas
        
        return outputStream;
    }
    
    std::istream &operator>>(std::istream &inputStream, Point &point)// overloaded >> operator, code taken from lecture, tweaked for function
    {
        
       
        string tempstring;
        string buffer;
        double d;
        int index = -1;
        
        while (getline(inputStream, tempstring))
        {
            stringstream lineStream(tempstring);
            
            while (getline(lineStream, buffer, ','))
            {
                d = stod(buffer);
                point.setValue(++index, d);
            }
            
        }
        return inputStream;
    }
    
}
// destructor, and getters/ setters. Pretty obvious as to what they do.
Point::~Point()
{
    delete  [] __values;
}

int Point::getId() const
{
    return __id;
}

int Point::getDims() const
{
    return __dim;
}

void Point::setValue(int i, double d)
{
    __values[i] = d;
}

double Point::getValue(int i) const
{
    return __values[i];
}
