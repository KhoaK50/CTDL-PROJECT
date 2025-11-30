#pragma once

#include <vector>
#include "Node.h"

template <class DataType>
class Stack
{
private:
    Node<DataType>* _pTop;
    int             _iSize;

public:
    Stack();
    ~Stack();

    // ===== CORE =====
    void            push(const DataType &data);
    DataType        pop();
    const DataType& peek() const;
    bool            isEmpty() const;
    int             getSize() const;
    void            clear();
    void            print() const;      // chỉ dùng debug

    // ===== UTILITY =====
    bool                  contains(const DataType &data) const;
    std::vector<DataType> toVector() const;
};

#include "Stack.tpp"
