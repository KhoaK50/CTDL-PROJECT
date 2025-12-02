#pragma once

#include <string>


class AdminAccount
{
private:
    std::string _strUserName;
    std::string _strPassword;

public:
    /************
    * @Description Constructor mặc định
    *************/
    AdminAccount();

    /************
    * @Description Constructor đầy đủ tham số
    * @param strUserName Tên đăng nhập
    * @param strPassword Mật khẩu
    *************/
    AdminAccount(const std::string &strUserName, const std::string &strPassword);

    /************
    * @Description Copy Constructor
    * @param src Đối tượng nguồn để sao chép
    *************/
    AdminAccount(const AdminAccount &src);

    /************
    * @Description Destructor (Hủy)
    *************/
    ~AdminAccount();

    /************
    * @Description Thiết lập tên đăng nhập
    * @param strUserName Tên đăng nhập mới
    *************/
    void setUserName(const std::string &strUserName);

    /************
    * @Description Thiết lập mật khẩu
    * @param strPassword Mật khẩu mới
    *************/
    void setPassword(const std::string &strPassword);

    /************
    * @Description Lấy tên đăng nhập hiện tại
    * @return Tên đăng nhập (string)
    *************/
    std::string getUserName() const;

    /************
    * @Description Lấy mật khẩu hiện tại
    * @return Mật khẩu (string)
    *************/
    std::string getPassword() const;

    /************
    * @Description Hiển thị thông tin tài khoản ra màn hình
    *************/
    void print() const;

    /************
    * @Description So sánh hai tài khoản (dùng cho Queue/Stack/LinkedList)
    * @param other Tài khoản cần so sánh
    * @return true nếu giống nhau (cả user và pass)
    *************/
    bool operator==(const AdminAccount &other) const;
};