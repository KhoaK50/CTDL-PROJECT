#pragma once

#include <string>

class Customer
{
private:
    int         _iIndex;        // Số thứ tự khách hàng
    std::string _strIdNumber;   // CMND/CCCD
    std::string _strFullName;   // Họ tên

public:
    // Constructor
    Customer();
    Customer(int index, const std::string &idNumber,
             const std::string &fullName);

    // Setter
    void setIndex(int index);
    void setIdNumber(const std::string &idNumber);
    void setFullName(const std::string &fullName);

    // Getter
    int getIndex() const;
    std::string getIdNumber() const;
    std::string getFullName() const;

    // Print thông tin khách hàng
    void print() const;

    // So sánh object – cực quan trọng cho CTDL (LinkedList/Stack/Queue)
    bool operator==(const Customer &other) const;
};
