#pragma once

#include <string>

class Customer
{
private:
    int         _iIndex;        
    std::string _strIdNumber;   
    std::string _strFullName;   

public:
    /************
    * @Description Constructor mặc định
    *************/
    Customer();

    /************
    * @Description Constructor đầy đủ tham số
    * @param iIndex Số thứ tự khách hàng
    * @param strIdNumber Số CMND/CCCD
    * @param strFullName Họ và tên
    *************/
    Customer(int iIndex, const std::string &strIdNumber, const std::string &strFullName);

    // ========================= Setters =========================

    /************
    * @Description Thiết lập số thứ tự
    * @param iIndex Số thứ tự cần gán
    *************/
    void setIndex(int iIndex);

    /************
    * @Description Thiết lập số CMND/CCCD
    * @param strIdNumber Chuỗi CMND/CCCD mới
    *************/
    void setIdNumber(const std::string &strIdNumber);

    /************
    * @Description Thiết lập họ tên
    * @param strFullName Họ tên mới
    *************/
    void setFullName(const std::string &strFullName);

    // ========================= Getters =========================

    /************
    * @Description Lấy số thứ tự khách hàng
    * @return Số thứ tự (int)
    *************/
    int getIndex() const;

    /************
    * @Description Lấy số CMND/CCCD
    * @return Chuỗi CMND/CCCD
    *************/
    std::string getIdNumber() const;

    /************
    * @Description Lấy họ tên khách hàng
    * @return Chuỗi họ tên
    *************/
    std::string getFullName() const;

    // ========================= Utilities =========================

    /************
    * @Description Hiển thị thông tin khách hàng
    *************/
    void print() const;

    /************
    * @Description So sánh hai khách hàng (dựa trên CMND/CCCD)
    * @param other Đối tượng khách hàng cần so sánh
    * @return true nếu trùng CMND/CCCD
    *************/
    bool operator==(const Customer &other) const;
};