#include "domain/AdminAccount.h"
#include <iostream> 

using namespace std;

// ========================= Constructors =========================

/************
* @Description Constructor mặc định
*************/
AdminAccount::AdminAccount()
{
    this->_strUserName = "";
    this->_strPassword = "";
}

/************
* @Description Constructor đầy đủ tham số
* @param strUserName Tên đăng nhập
* @param strPassword Mật khẩu
*************/
AdminAccount::AdminAccount(const string &strUserName, const string &strPassword)
{
    this->_strUserName = strUserName;
    this->_strPassword = strPassword;
}

/************
* @Description Copy constructor
* @param src Đối tượng nguồn
*************/
AdminAccount::AdminAccount(const AdminAccount &src)
{
    this->_strUserName = src._strUserName;
    this->_strPassword = src._strPassword;
}

/************
* @Description Destructor
*************/
AdminAccount::~AdminAccount()
{
}

// ========================= Setter =========================

/************
* @Description Thiết lập tên đăng nhập
* @param strUserName Tên đăng nhập mới
*************/
void AdminAccount::setUserName(const string &strUserName)
{
    this->_strUserName = strUserName;
}

/************
* @Description Thiết lập mật khẩu
* @param strPassword Mật khẩu mới
*************/
void AdminAccount::setPassword(const string &strPassword)
{
    this->_strPassword = strPassword;
}

// ========================= Getter =========================

/************
* @Description Lấy tên đăng nhập
* @return Tên đăng nhập hiện tại
*************/
string AdminAccount::getUserName() const
{
    return this->_strUserName;
}

/************
* @Description Lấy mật khẩu
* @return Mật khẩu hiện tại
*************/
string AdminAccount::getPassword() const
{
    return this->_strPassword;
}

// ========================= Print =========================

/************
* @Description In thông tin admin (dùng debug)
*************/
void AdminAccount::print() const
{
    cout << "[Admin] " << this->_strUserName << "\n";
}

// ========================= Operator =========================

/************
* @Description So sánh hai tài khoản
* @param other Tài khoản cần so sánh
* @return true nếu giống nhau
*************/
bool AdminAccount::operator==(const AdminAccount &other) const
{
    return (this->_strUserName == other._strUserName &&
            this->_strPassword == other._strPassword);
}