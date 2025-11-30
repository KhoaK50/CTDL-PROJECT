#include "domain/AdminAccount.h"

// =====================================================
// Constructor mặc định
// =====================================================
AdminAccount::AdminAccount()
    : _strUserName(""), _strPassword("")
{
}

// =====================================================
// Constructor đầy đủ
// =====================================================
AdminAccount::AdminAccount(const std::string &strUserName,
                           const std::string &strPassword)
    : _strUserName(strUserName),
      _strPassword(strPassword)
{
}

// =====================================================
// Copy constructor
// =====================================================
AdminAccount::AdminAccount(const AdminAccount &src)
    : _strUserName(src._strUserName),
      _strPassword(src._strPassword)
{
}

// =====================================================
// Destructor
// =====================================================
AdminAccount::~AdminAccount()
{
}

// =====================================================
// Setter
// =====================================================
void AdminAccount::setUserName(const std::string &strUserName)
{
    _strUserName = strUserName;
}

void AdminAccount::setPassword(const std::string &strPassword)
{
    _strPassword = strPassword;
}

// =====================================================
// Getter
// =====================================================
std::string AdminAccount::getUserName() const
{
    return _strUserName;
}

std::string AdminAccount::getPassword() const
{
    return _strPassword;
}

// =====================================================
// In thông tin admin (dùng debug)
// =====================================================
void AdminAccount::print() const
{
    std::cout << "[Admin] " << _strUserName << "\n";
}

// =====================================================
// So sánh object – cần cho LinkedList/Queue/Stack
// =====================================================
bool AdminAccount::operator==(const AdminAccount &other) const
{
    return (_strUserName == other._strUserName &&
            _strPassword == other._strPassword);
}
