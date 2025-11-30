#pragma once

// Forward declaration cho các lớp dùng Node làm friend
template <class DataType> class Stack;
template <class DataType> class Queue;
template <class DataType> class LinkedList;

template <class DataType>
class Node
{
    // Cho phép Stack, Queue, LinkedList truy cập trực tiếp private
    friend class Stack<DataType>;
    friend class Queue<DataType>;
    friend class LinkedList<DataType>;

private:
    DataType        _data;
    Node<DataType>* _pNext;

public:
    Node();
    explicit Node(const DataType &data);

    // Lấy / gán dữ liệu
    DataType&       getData();
    const DataType& getData() const;
    void            setData(const DataType &data);

    // Lấy / gán node kế tiếp
    Node<DataType>*       getNext();
    const Node<DataType>* getNext() const;
    void                  setNext(Node<DataType>* pNext);
};

// Phần triển khai template
#include "Node.tpp"
