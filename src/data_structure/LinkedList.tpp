#pragma once
#include "LinkedList.h" 
#include <utility> // std::swap
#include <iostream>
using std::cout;
using std::endl;
// Không khai báo lại class ở đây, chỉ triển khai hàm

template <class DataType>
LinkedList<DataType>::LinkedList(): _pHead(nullptr), _pTail(nullptr), _iSize(0){}

template <class DataType>
LinkedList<DataType>::~LinkedList(){
    clear();
}

/*********************************************************
* @Description Xuất toàn bộ dữ liệu của LinkedList
*********************************************************/
template <class DataType>
void LinkedList<DataType>::display() const
{
    Node<DataType>* pWalker = _pHead;

    while (pWalker != nullptr)
    {
        std::cout << pWalker->getData() << " ";
        pWalker = pWalker->getNext();
    }
}

/*********************************************************
* @Description Thêm một phần tử vào đầu LinkedList
* @parameter   Giá trị của phần tử cần thêm
*********************************************************/
template <class DataType>
void LinkedList<DataType>::addHead(DataType data){
    Node<DataType>* pAdd = new Node<DataType>(data);

    if (_pHead == nullptr){
        _pHead = _pTail = pAdd;
    }
    else{
        pAdd->_pNext = _pHead;
        _pHead = pAdd;
    }

    _iSize++;
}

/*********************************************************
* @Description Thêm một phần tử vào cuối LinkedList
* @parameter   Giá trị của phần tử cần thêm
*********************************************************/
template <class DataType>
void LinkedList<DataType>::addTail(DataType data){
    Node<DataType>* pAdd = new Node<DataType>(data);

    if (_pHead == nullptr){
        _pHead = _pTail = pAdd;
    }
    else{
        _pTail->_pNext = pAdd;
        _pTail = pAdd;
    }

    _iSize++;
}

/*********************************************************
* @Description Thêm một phần tử vào sau phần tử node trong LinkedList
* @parameter   Phần tử node và giá trị của phần tử cần thêm
*********************************************************/
template <class DataType>
void LinkedList<DataType>::addAfter(Node<DataType>* node, DataType data){
    // Nếu node là nullptr -> hiểu là thêm vào đầu
    if (node == nullptr){
        addHead(data);
        return;
    }

    Node<DataType>* pAdd = new Node<DataType>(data);

    pAdd->_pNext = node->_pNext;
    node->_pNext = pAdd;

    if (node == _pTail){
        _pTail = pAdd;
    }

    _iSize++;
}

/*********************************************************
* @Description Tìm một phần tử trong LinkedList khi biết khóa
* @parameter   Giá trị khóa của phần tử cần tìm
* @return      Trả về phần tử được tìm thấy, nullptr nếu không tìm thấy
*********************************************************/
template <class DataType>
Node<DataType>* LinkedList<DataType>::search(const DataType &data){
    Node<DataType>* p = _pHead;

    while (p != nullptr && p->_data != data){
        p = p->_pNext;
    }

    return p;
}

/*********************************************************
* @Description Tìm một phần tử đứng trước một phần tử trong LinkedList
* @parameter   node: phần tử cần tìm node đứng trước
* @return      Trả về phần tử được tìm thấy, nullptr nếu không tìm thấy
*********************************************************/
template <class DataType>
Node<DataType>* LinkedList<DataType>::searchPre(Node<DataType>* node){
    if (_pHead == node){
        return nullptr;
    }

    Node<DataType>* p = _pHead;

    while (p != nullptr && p->_pNext != node){
        p = p->_pNext;
    }

    return p;
}

/*********************************************************
* @Description Xóa phần tử ở đầu LinkedList
*********************************************************/
template <class DataType>
void LinkedList<DataType>::removeHead(){
    if (_pHead != nullptr){
        Node<DataType>* pTemp = _pHead;
        _pHead = _pHead->_pNext;
        delete pTemp;

        if (_pHead == nullptr){
            _pTail = nullptr;
        }

        _iSize--;
    }
}

/*********************************************************
* @Description Xóa phần tử ở cuối LinkedList
*********************************************************/
template <class DataType>
void LinkedList<DataType>::removeTail(){
    if (_pHead == nullptr){
        return;
    }

    Node<DataType>* pTemp = _pTail;
    _pTail = searchPre(_pTail);
    delete pTemp;

    if (_pTail != nullptr){
        _pTail->_pNext = nullptr;
    }
    else{
        _pHead = nullptr;
    }

    _iSize--;
}

/*********************************************************
* @Description Xóa phần tử ở sau phần tử khác trong LinkedList
*********************************************************/
template <class DataType>
void LinkedList<DataType>::removeAfter(Node<DataType>* node){
    if (node == nullptr){
        removeHead();
        return;
    }

    Node<DataType>* pTemp = node->_pNext;

    if (pTemp != nullptr){
        if (pTemp == _pTail)
        {
            _pTail = node;
        }

        node->_pNext = pTemp->_pNext;
        delete pTemp;
        _iSize--;
    }
}

/*********************************************************
* @Description Xóa phần tử có khóa là data trong LinkedList
*********************************************************/
template <class DataType>
void LinkedList<DataType>::remove(const DataType &data){
    Node<DataType>* pWalker = _pHead;
    Node<DataType>* pTemp   = nullptr;

    while (pWalker != nullptr && !(pWalker->_data == data)){
        pTemp   = pWalker;
        pWalker = pWalker->_pNext;
    }

    if (pWalker == nullptr){
        return;
    }

    if (pTemp == nullptr){
        removeHead();
        return;
    }

    if (pWalker == _pTail){
        _pTail = pTemp;
        _pTail->_pNext = nullptr;
    }
    else{
        pTemp->_pNext = pWalker->_pNext;
    }

    delete pWalker;
    _iSize--;
}

/*********************************************************
* @Description Xóa toàn bộ LinkedList
*********************************************************/
template <class DataType>
void LinkedList<DataType>::clear(){
    while (_pHead != nullptr){
        removeHead();
    }
}

/*********************************************************
* @Description Sắp xếp danh sách theo thuật toán Selection Sort
*********************************************************/
template <class DataType>
void LinkedList<DataType>::selectionSort(){
    Node<DataType>* p;
    Node<DataType>* q;
    Node<DataType>* pMin;

    for (p = _pHead; p != nullptr && p->_pNext != nullptr; p = p->_pNext){
        pMin = p;

        for (q = p->_pNext; q != nullptr; q = q->_pNext){
            if (q->_data < pMin->_data){
                pMin = q;
            }
        }

        if (pMin != p){
            std::swap(pMin->_data, p->_data);
        }
    }
}

/*********************************************************
* @Description Lấy số lượng phần tử có trong LinkedList
* @return      Trả về số phần tử có trong LinkedList
*********************************************************/
template <class DataType>
int LinkedList<DataType>::getSize() const{
    return _iSize;
}

/*********************************************************
* @Description Chuyển LinkedList sang vector
*********************************************************/
template <class DataType>
std::vector<DataType> LinkedList<DataType>::toVector() const
{
    std::vector<DataType> v;
    Node<DataType>* p = _pHead;
    while (p != nullptr)
    {
        v.push_back(p->_data);
        p = p->_pNext;
    }
    return v;
}
