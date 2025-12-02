#include "domain/Airplane.h"
#include <iostream>

using namespace std;

/************
* @Description Constructor mặc định
*************/
Airplane::Airplane()
{
    this->_strPlaneId = "";
    this->_iSeatCount = 0;
}

/************
* @Description Constructor đầy đủ tham số
* @param strPlaneId Số hiệu máy bay
* @param iSeatCount Số lượng ghế
*************/
Airplane::Airplane(const string& strPlaneId, const int iSeatCount)
{
    this->_strPlaneId = strPlaneId;
    this->_iSeatCount = iSeatCount;
}

/************
* @Description Thiết lập số hiệu máy bay
* @param strPlaneId Số hiệu cần gán
*************/
void Airplane::setPlaneId(const string& strPlaneId)
{
    this->_strPlaneId = strPlaneId;
}

/************
* @Description Thiết lập số ghế
* @param iSeatCount Số ghế cần gán
*************/
void Airplane::setSeatCount(const int iSeatCount)
{
    this->_iSeatCount = iSeatCount;
}


/************
* @Description Lấy số hiệu máy bay
* @return Chuỗi số hiệu
*************/
string Airplane::getPlaneId() const
{
    return this->_strPlaneId;
}

/************
* @Description Lấy số lượng ghế
* @return Số lượng ghế (int)
*************/
int Airplane::getSeatCount() const
{
    return this->_iSeatCount;
}

/************
* @Description Hiển thị thông tin máy bay
*************/
void Airplane::print() const
{
    cout << "\n===== THONG TIN MAY BAY =====\n";
    cout << "So hieu may bay : " << this->_strPlaneId << "\n";
    cout << "So cho ngoi     : " << this->_iSeatCount << "\n";
    cout << "===================================\n";
}


/************
* @Description So sánh hai máy bay
* @param other Đối tượng máy bay cần so sánh
* @return true nếu thông tin giống nhau
*************/
bool Airplane::operator==(const Airplane& other) const
{
    return (this->_strPlaneId == other._strPlaneId &&
            this->_iSeatCount == other._iSeatCount);
}