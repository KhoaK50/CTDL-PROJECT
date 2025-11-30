#pragma once

#include <string>
#include <iostream>

class AdminAccount
{
private:
    std::string _strUserName;
    std::string _strPassword;

public:
    AdminAccount();
    AdminAccount(const std::string &strUserName,
                 const std::string &strPassword);
    AdminAccount(const AdminAccount &src);
    ~AdminAccount();

    void setUserName(const std::string &strUserName);
    void setPassword(const std::string &strPassword);

    std::string getUserName() const;
    std::string getPassword() const;

    void print() const;

    // So sánh để dùng cho Queue/Stack/LinkedList
    bool operator==(const AdminAccount &other) const;
};
