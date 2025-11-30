#pragma once
#include "Node.h"
#include <iostream>
#include <stdexcept>

// ==================== Constructor / Destructor ====================

template <class DataType>
Queue<DataType>::Queue()
    : _pHead(nullptr), _pTail(nullptr), _iSize(0)
{
}

template <class DataType>
Queue<DataType>::~Queue()
{
    clear();
}

// ==================== Core Operations ====================

template <class DataType>
void Queue<DataType>::enQueue(const DataType &data)
{
    Node<DataType>* pNew = new Node<DataType>(data);

    if (_pHead == nullptr)
    {
        _pHead = _pTail = pNew;
    }
    else
    {
        _pTail->setNext(pNew);
        _pTail = pNew;
    }

    _iSize++;
}

template <class DataType>
DataType Queue<DataType>::deQueue()
{
    if (_pHead == nullptr)
    {
        throw std::runtime_error("Queue empty");
    }

    Node<DataType>* pTemp = _pHead;
    DataType data = pTemp->getData();

    _pHead = pTemp->getNext();

    if (_pHead == nullptr)
    {
        _pTail = nullptr;
    }

    delete pTemp;
    _iSize--;

    return data;
}

template <class DataType>
const DataType& Queue<DataType>::front() const
{
    if (_pHead == nullptr)
    {
        throw std::runtime_error("Queue empty");
    }

    return _pHead->getData();
}

template <class DataType>
bool Queue<DataType>::isEmpty() const
{
    return (_pHead == nullptr);
}

template <class DataType>
int Queue<DataType>::getSize() const
{
    return _iSize;
}

template <class DataType>
void Queue<DataType>::clear()
{
    while (_pHead != nullptr)
    {
        Node<DataType>* pTemp = _pHead;
        _pHead = _pHead->getNext();
        delete pTemp;
    }

    _pTail = nullptr;
    _iSize = 0;
}

// ==================== Utility ====================

template <class DataType>
const DataType& Queue<DataType>::back() const
{
    if (_pTail == nullptr)
    {
        throw std::runtime_error("Queue empty");
    }

    return _pTail->getData();
}

template <class DataType>
bool Queue<DataType>::contains(const DataType &data) const
{
    Node<DataType>* p = _pHead;

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
std::vector<DataType> Queue<DataType>::toVector() const
{
    std::vector<DataType> result;

    Node<DataType>* p = _pHead;

    while (p != nullptr)
    {
        result.push_back(p->getData());
        p = p->getNext();
    }

    return result;
}
