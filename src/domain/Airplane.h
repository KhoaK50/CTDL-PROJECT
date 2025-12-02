#pragma once

#include <string>
#include <iostream>

class Airplane
{
private:
    std::string _strPlaneId;
    int _iSeatCount;

public:
    /************
    * @Description Constructor mặc định
    *************/
    Airplane();

    /************
    * @Description Constructor đầy đủ tham số
    * @param strPlaneId Số hiệu máy bay
    * @param iSeatCount Số lượng ghế
    *************/
    Airplane(const std::string& strPlaneId, const int iSeatCount);

    /************
    * @Description Thiết lập số hiệu máy bay
    * @param strPlaneId Số hiệu cần gán
    *************/
    void setPlaneId(const std::string& strPlaneId);

    /************
    * @Description Thiết lập số ghế
    * @param iSeatCount Số ghế cần gán
    *************/
    void setSeatCount(const int iSeatCount);

    /************
    * @Description Lấy số hiệu máy bay
    * @return Chuỗi số hiệu
    *************/
    std::string getPlaneId() const;

    /************
    * @Description Lấy số lượng ghế
    * @return Số lượng ghế (int)
    *************/
    int getSeatCount() const;

    /************
    * @Description Hiển thị thông tin máy bay
    *************/
    void print() const;

    /************
    * @Description So sánh hai máy bay (dựa trên ID)
    * @param other Đối tượng máy bay cần so sánh
    * @return true nếu trùng ID, ngược lại false
    *************/
    bool operator==(const Airplane& other) const;
};