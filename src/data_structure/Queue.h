#pragma once

#include <vector>
#include "Node.h"

template <class DataType>
class Queue
{
private:
    Node<DataType>* _pHead;
    Node<DataType>* _pTail;
    int             _iSize;

public:
    Queue();
    ~Queue();

    // ==== CORE ====
    void        enQueue(const DataType &data);
    DataType    deQueue();
    const DataType& front() const;
    bool        isEmpty() const;
    int         getSize() const;
    void        clear();

    // ==== UTILITY ====
    const DataType& back() const;
    bool            contains(const DataType &data) const;
    std::vector<DataType> toVector() const;
};

#include "Queue.tpp"
