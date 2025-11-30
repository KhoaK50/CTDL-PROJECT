#pragma once

#include "Node.h"
#include <vector>     // thêm

// Danh sách liên kết đơn tổng quát (template) dùng Node<DataType>.
// Giả sử Node<DataType> có các thành phần:
//   - DataType _data;
//   - Node<DataType>* _pNext;
//   - Hàm display() để in ra 1 node.

template <class DataType>
class LinkedList
{
private:
    Node<DataType>* _pHead;
    Node<DataType>* _pTail;
    int _iSize;

public:
    LinkedList();
    ~LinkedList();

    // Thêm phần tử vào đầu danh sách
    void addHead(DataType data);

    // Thêm phần tử vào cuối danh sách
    void addTail(DataType data);

    // Thêm phần tử vào sau 1 node cho trước
    // Nếu node == nullptr thì sẽ thêm vào đầu
    void addAfter(Node<DataType>* node, DataType data);

    // Tìm node có _data == data
    // Trả về con trỏ đến node nếu tìm thấy, nullptr nếu không
    Node<DataType>* search(const DataType &data);

    // Tìm node đứng trước "node" trong danh sách
    // Trả về nullptr nếu node là head hoặc không tìm thấy
    Node<DataType>* searchPre(Node<DataType>* node);

    // Xóa node đầu
    void removeHead();

    // Xóa node cuối
    void removeTail();

    // Xóa node nằm sau "node"
    // Nếu node == nullptr thì xóa head
    void removeAfter(Node<DataType>* node);

    // Xóa node đầu tiên có _data == data
    void remove(const DataType &data);

    // Sắp xếp danh sách tăng dần theo _data bằng Selection Sort
    void selectionSort();

    // Lấy số phần tử hiện có trong danh sách
    int getSize() const;

    // In toàn bộ danh sách bằng cách gọi display() trên từng node
    void display() const;

    // Xóa toàn bộ danh sách, giải phóng bộ nhớ
    void clear();

    // Chuyển toàn bộ danh sách sang vector (hỗ trợ thống kê)
    std::vector<DataType> toVector() const;
};

// Phần triển khai template
#include "LinkedList.tpp"
