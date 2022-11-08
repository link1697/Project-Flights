/**
 * @file flyheap.cpp
 * Implementation of a flyheap class.
 * 
 * @author Haoyu Li
 * @date Fall 2021
 */

#include "includes/flyheap.h"
size_t flyheap::root()  
{
    return 1;
}

size_t flyheap::leftChild(size_t currentIdx)  
{
    return currentIdx * 2;
}

size_t flyheap::rightChild(size_t currentIdx)  
{
    return currentIdx * 2 + 1;
}

size_t flyheap::parent(size_t currentIdx)  
{
    return currentIdx / 2;
}

bool flyheap::hasAChild(size_t currentIdx)  
{
    if ((currentIdx * 2) < _elems.size()) {
        return true;
    }
    return false;
}

size_t flyheap::maxPriorityChild(size_t currentIdx)  
{
    if( 2*currentIdx+1<_elems.size() ){
       if (smaller(_elems[2*currentIdx], _elems[2*currentIdx+1]))
       return 2*currentIdx;
       else return 2*currentIdx+1;
   }
   else if(2*currentIdx<_elems.size()){
       return 2*currentIdx;
   } 
    return 0;
  
}

void flyheap::flyheapifyDown(size_t currentIdx)
{
    if (!hasAChild(currentIdx)) return;
    int c=maxPriorityChild(currentIdx);
    if (smaller(_elems[c], _elems[currentIdx])) {
        std::swap(_elems[currentIdx], _elems[c]);
        std::swap(index[currentIdx],index[c]);
        flyheapifyDown(c);
    }
 
}

void flyheap::flyheapifyUp(size_t currentIdx)
{
    if (currentIdx ==1) return;
    size_t parentIdx = parent(currentIdx);
    if (smaller(_elems[currentIdx], _elems[parentIdx])) {
        int temp=_elems[currentIdx];
        _elems[currentIdx] = _elems[parentIdx];
        _elems[parentIdx]=temp;  
        std::swap(index[currentIdx],index[parentIdx]);
        flyheapifyUp(parentIdx);
    }
}

flyheap::flyheap(vector<double>* vectorf)
{
    _elems.push_back(-1);
    f = &(*vectorf);
    for(unsigned i=0;i<(*f).size();i++){
        index.push_back(0);
    }
}


int flyheap::pop()
{
    if(_elems.size()>1){
        int t= _elems[1];
        std::swap(_elems[1],_elems[_elems.size()-1]);
        std::swap(index[1],index[_elems.size()-1]);
        _elems.pop_back();
        for(unsigned i=parent(_elems.size());i>0;i--){
            flyheapifyDown(1);
        }
        flyheapifyDown(1);
        return t;
    } 
 
    return -1;
}

int flyheap::peek()  
{
    if (_elems.size()>1) {
        return _elems[1];
    } else {
        return -1;
    }
}

void flyheap::push(int elem)
{
    int temp = elem;
    index[elem]=_elems.size();
    _elems.push_back(temp);

    flyheapifyUp(_elems.size() - 1);
}
 
void flyheap::updateElem(int i)
{
    int idx=index[i];
    flyheapifyUp(idx);
    flyheapifyDown(idx);

}

bool flyheap::empty()  
{
    if (_elems.size() <= 1) {
        return true;
    }
    return false;
}


bool flyheap::smaller(int a, int b) {
    return ((*f)[a] < (*f)[b]);
}