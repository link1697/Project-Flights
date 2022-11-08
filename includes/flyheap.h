/**
 * @file flyheap.h
 * Definitinon of a flyheap class.
 */

#pragma once

#include <vector>
#include <math.h>
#include <iostream>
#include <stddef.h>
#include <map>

using namespace std;

class flyheap
{
  public:
    flyheap(vector<double>* f);
    int pop();
    int peek()  ;
    void push(int elem);
    void updateElem(int i);
    bool empty()  ;
    size_t root()  ;
    vector<int> index;
  private:
    std::vector<int> _elems;

    size_t leftChild(size_t currentIdx)  ;

    size_t rightChild(size_t currentIdx)  ;

    size_t parent(size_t currentIdx)  ;

    bool hasAChild(size_t currentIdx)  ;

    size_t maxPriorityChild(size_t currentIdx)  ;


    void flyheapifyDown(size_t currentIdx);

    void flyheapifyUp(size_t currentIdx);
    bool smaller(int a, int b);

    vector<double>* f;

};
