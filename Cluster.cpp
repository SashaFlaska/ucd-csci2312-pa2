//
//  Cluster.cpp
//  PA2
//
//  Created by Alexander Flaska on 2/24/16.
//  Copyright © 2016 Alexander Flaska. All rights reserved.
//

#include <cstdlib>
#include <assert.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "Cluster.h"

namespace Clustering
{
    
    LNode::LNode(const Point &p, LNodePtr n = nullptr) : point(p)// node
    {
        point = p;
        next = n;
    }
    
    Cluster::Cluster()// default constructor
    {
        __size = 0;
        __points = nullptr;
    }
    
    Cluster::Cluster(const Cluster &cluster) : Cluster()// copy constructor
    {
        for(int count = 0; count < cluster.__size; ++count)
        {
            add(cluster[count]);
        }
    }
    int Cluster::getSize() const// getsize, pretty obvious
    {
        return __size;
    }

    
    Cluster & Cluster::operator=(const Cluster &cluster)// overloaded = operator
    {
        if(this == &cluster)// returns if current is equal to the cluster.
            return *this;
        
        LNodePtr current = __points;// lnodeptr's
        LNodePtr next;
        
        
        while (current != nullptr)// as long as it isn't null
        {
            next = current->next;// assign
            delete current;// delete
            current = next;// assign
            __size--;// subtract 1 from size
        }
        
        __size = 0;
        __points = nullptr;
        
        LNodePtr rightPointer = cluster.__points;
        
        
        for (int pos = 0; pos < cluster.getSize() ; ++pos)
        {
            add(rightPointer->point);
            rightPointer = rightPointer->next;
        }
        return *this;
    }
    Cluster::~Cluster()// destructor
    {
        LNodePtr current;
        while (__points != nullptr) {
            
            current = __points;
            __points = current->next;
            
            delete current;
        }
        
        
        
        
    }
    
      void Cluster::add(const Point &point)
    {
        
        LNodePtr node = new LNode(point, nullptr);// makes a new node using constructor
        
        if (__points == nullptr)// if it's null
        {
            __points = node;// put a node in
        }
        else if (__points->point > point)// using overloaded > check pointers
        {
            node->next = __points;// asign appropriately
            __points = node;
        }
        else
        {
            LNodePtr current = __points, next = __points->next;// add the pointer
            while (true)
            {
                if (next == nullptr || next->point > point)
                {
                    current->next = node;
                    node->next = next;
                    break;// break if this happens, otherwise weird things take place
                }
                else
                {
                    current = next;
                    next = next->next;
                }
            }
        }
        
        __size++;
    }
    
    const Point &Cluster::remove(const Point &point)
    {
        LNodePtr previous = nullptr;// assign previous to null
        LNodePtr current = __points;// and the current to whatever __points is
        if (current !=nullptr && current->point == point)// if current not null, and it's equal to the pt,
        {
            __points = __points->next;// assign to next, delete current, decrement size, and return
            delete current;
            __size--;
            return point;
        }
        while (current != nullptr && point != current->point)// if both not equal, make previous = current, and make current point to next.
        {
            previous =current;
            current = current->next;
        }
        previous->next = current->next;// make previous the current, and delete the real current. decrement size. return.
        delete current;
        __size--;
        return point;
        
    }
    
    bool Cluster::contains(const Point &point)
    {
        
        LNodePtr current = __points;// make current =to points
        
        while (current != nullptr)// as long as not equal, check if current pointer is equal to point, otherwise make current=nextptr.
        {
            if (current->point == point)
                return true;
            else
                current = current->next;
            
        }
        
        return false;
    }
    
    const Point &Cluster::operator[](unsigned int index) const
    {
        assert(__points != nullptr && index < __size);// make sure that it's not null and it's not overflowing.
        LNodePtr cursor = __points;
        
        for (int pos = 0; pos < index; ++pos)
        {
            cursor = cursor->next;
        }
        return cursor->point;
    }
    std::ostream &operator<<(std::ostream &outputStream, const Cluster &cluster)// overloaded <<
    {
        LNodePtr outputPointer = cluster.__points; // makes outputPointer, while it exists.
        while (outputPointer)
        {
            outputStream << outputPointer->point;//finds the pointer, gives it an endl char, and equates it to next ptr.
            outputStream << std::endl;
            outputPointer = outputPointer->next;
        }
        return outputStream;
    }
    
     std::istream &operator>>(std::istream &inputStream, Clustering::Cluster &cluster)
    {
        std::string tempString;// temp variable for getline
        while (getline(inputStream,tempString) && inputStream.good())// while everthing is good to go
        {
            int tempInt = std::count(tempString.begin(), tempString.end(), ',');// temp int holds the size using std::count
            Point point(tempInt+1);// makes a new point at size+1
            std::stringstream ss(tempString);// string stream for the pointer
            ss >> point;
            cluster.add(point);// adds it
        }
        return inputStream;
        
    }
    
    bool operator!=(const Cluster &leftCluster, const Cluster &rightCluster)
    {
        return(!(leftCluster == rightCluster));
    }
    
    Cluster &Cluster::operator+=(const Point &point)// overloaded+= uses add function
    {
        add(point);
        return *this;
    }
    
    Cluster &Cluster::operator-=(const Point &point)// overloaded-= using remove function
    {
        remove(point);
        return *this;
    }
    
    Cluster &Cluster::operator+=(const Cluster &cluster)// overloaded += for clusters
    {
        for(int pos = 0; pos < cluster.__size; ++pos)// lops, if it doesn't exist, add
        {
            if(!contains(cluster[pos]))
                add(cluster[pos]);
        }
        return *this;
    }
    
    Cluster &Cluster::operator-=(const Cluster &cluster)// same as above, just removes.
    {
        for(int count = 0; count < cluster.__size; ++count)
        {
            if(contains(cluster[count]))
                remove(cluster[count]);
        }
        return *this;
    }
    
    // functions below are super simple. Just returns using ovelroaded operators. 
    const Cluster operator+(const Cluster &cluster, const Point &point)
    {
        Cluster c(cluster);
       return  c+=point;
    }
    
    const Cluster operator-(const Cluster &cluster, const Point &point)
    {
        Cluster c(cluster);
        return c-=point;
    }
    
    const Cluster operator+(const Cluster &cluster, const Cluster &cluster1)
    {
        Cluster c(cluster);
        return  c+=cluster1;
    }
    
    const Cluster operator-(const Cluster &cluster, const Cluster &cluster1)
    {
        Cluster c(cluster);
       return  c-=cluster1;
    }
    bool operator==(const Cluster &leftCluster, const Cluster &rightCluster)
    {
        if (leftCluster.getSize() != rightCluster.getSize())// if the cluster sizes arent equal, return false
            return false;
        
        for (int pos = 0; pos < leftCluster.getSize(); ++pos)// otherwise, loop, and check each one to see if they're equal.
        {
            if (leftCluster[pos] != rightCluster[pos])
                return false;
        }
        
        return true;// return true only if nothing is equal
    }
    

}
