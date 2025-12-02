#include "domain/Customer.h"
#include <iostream>

using namespace std; 

// ========================= Constructors =========================

/************
* @Description Constructor mặc định
*************/
Customer::Customer()
{
    this->_iIndex = 0;
    this->_strIdNumber = "";
    this->_strFullName = "";
}

/************
* @Description Constructor đầy đủ tham số
* @param iIndex Số thứ tự khách hàng
* @param strIdNumber Số CMND/CCCD
* @param strFullName Họ và tên
*************/
Customer::Customer(int iIndex, const string &strIdNumber, const string &strFullName)
{
    this->_iIndex = iIndex;
    this->_strIdNumber = strIdNumber;
    this->_strFullName = strFullName;
}

// ============================= Setter =============================

/************
* @Description Thiết lập số thứ tự
* @param iIndex Số thứ tự cần gán
*************/
void Customer::setIndex(int iIndex)
{
    this->_iIndex = iIndex;
}

/************
* @Description Thiết lập số CMND/CCCD
* @param strIdNumber Chuỗi CMND/CCCD mới
*************/
void Customer::setIdNumber(const string &strIdNumber)
{
    this->_strIdNumber = strIdNumber;
}

/************
* @Description Thiết lập họ tên
* @param strFullName Họ tên mới
*************/
void Customer::setFullName(const string &strFullName)
{
    this->_strFullName = strFullName;
}

// ============================= Getter =============================

/************
* @Description Lấy số thứ tự khách hàng
* @return Số thứ tự (int)
*************/
int Customer::getIndex() const
{
    return this->_iIndex;
}

/************
* @Description Lấy số CMND/CCCD
* @return Chuỗi CMND/CCCD
*************/
string Customer::getIdNumber() const
{
    return this->_strIdNumber;
}

/************
* @Description Lấy họ tên khách hàng
* @return Chuỗi họ tên
*************/
string Customer::getFullName() const
{
    return this->_strFullName;
}

// =============================== Print ===============================

/************
* @Description Hiển thị thông tin khách hàng
*************/
void Customer::print() const
{
    cout << "\n===== THONG TIN KHACH HANG =====\n";
    cout << "STT          : " << this->_iIndex << "\n";
    cout << "CMND/CCCD    : " << this->_strIdNumber << "\n";
    cout << "Ho ten       : " << this->_strFullName << "\n";
    cout << "=====================================\n";
}

// ============================= Comparison =============================

/************
* @Description So sánh hai khách hàng
* @param other Đối tượng khách hàng cần so sánh
* @return true nếu tất cả thông tin trùng khớp
*************/
bool Customer::operator==(const Customer &other) const
{
    return (this->_iIndex      == other._iIndex &&
            this->_strIdNumber == other._strIdNumber &&
            this->_strFullName == other._strFullName);
}