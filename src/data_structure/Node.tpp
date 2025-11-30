#pragma once
#include "Node.h"

template <class DataType>
Node<DataType>::Node()
    : _data(), _pNext(nullptr)
{
}

template <class DataType>
Node<DataType>::Node(const DataType &data)
    : _data(data), _pNext(nullptr)
{
}

template <class DataType>
DataType& Node<DataType>::getData()
{
    return _data;
}

template <class DataType>
const DataType& Node<DataType>::getData() const
{
    return _data;
}

template <class DataType>
void Node<DataType>::setData(const DataType &data)
{
    _data = data;
}

template <class DataType>
Node<DataType>* Node<DataType>::getNext()
{
    return _pNext;
}

template <class DataType>
const Node<DataType>* Node<DataType>::getNext() const
{
    return _pNext;
}

template <class DataType>
void Node<DataType>::setNext(Node<DataType>* pNext)
{
    _pNext = pNext;
}
