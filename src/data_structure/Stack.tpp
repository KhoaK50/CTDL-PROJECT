#pragma once
#include "Node.h"
#include <iostream>     // cho print()
#include <stdexcept>    // cho runtime_error

using std::cout;

// ==================== Constructor / Destructor ====================

template <class DataType>
Stack<DataType>::Stack()
    : _pTop(nullptr), _iSize(0)
{
}

template <class DataType>
Stack<DataType>::~Stack()
{
    clear();
}

// ==================== CORE ====================

template <class DataType>
void Stack<DataType>::push(const DataType &data)
{
    Node<DataType>* pNew = new Node<DataType>(data);
    pNew->setNext(_pTop);
    _pTop = pNew;
    _iSize++;
}

template <class DataType>
DataType Stack<DataType>::pop()
{
    if (_pTop == nullptr)
    {
        throw std::runtime_error("Stack empty");
    }

    Node<DataType>* pTemp = _pTop;
    DataType data = pTemp->getData();

    _pTop = pTemp->getNext();
    delete pTemp;
    _iSize--;

    return data;
}

template <class DataType>
const DataType& Stack<DataType>::peek() const
{
    if (_pTop == nullptr)
    {
        throw std::runtime_error("Stack empty");
    }

    return _pTop->getData();
}

template <class DataType>
bool Stack<DataType>::isEmpty() const
{
    return (_pTop == nullptr);
}

template <class DataType>
int Stack<DataType>::getSize() const
{
    return _iSize;
}

template <class DataType>
void Stack<DataType>::clear()
{
    while (_pTop != nullptr)
    {
        Node<DataType>* pTemp = _pTop;
        _pTop = _pTop->getNext();
        delete pTemp;
    }

    _iSize = 0;
}

// ==================== PRINT (DEBUG) ====================

template <class DataType>
void Stack<DataType>::print() const
{
    if (_pTop == nullptr)
    {
        cout << "Stack rong!\n";
        return;
    }

    Node<DataType>* p = _pTop;

    while (p != nullptr)
    {
        cout << p->getData() << " ";   // DataType cần có operator<< nếu dùng hàm này
        p = p->getNext();
    }

    cout << "\n";
}

// ==================== UTILITY ====================

template <class DataType>
bool Stack<DataType>::contains(const DataType &data) const
{
    Node<DataType>* p = _pTop;

    while (p != nullptr)
    {
        if (p->getData() == data)
        {
            return true;
        }

        p = p->getNext();
    }

    return false;
}

template <class DataType>
std::vector<DataType> Stack<DataType>::toVector() const
{
    std::vector<DataType> vResult;

    Node<DataType>* p = _pTop;

    while (p != nullptr)
    {
        vResult.push_back(p->getData());
        p = p->getNext();
    }

    return vResult;
}
