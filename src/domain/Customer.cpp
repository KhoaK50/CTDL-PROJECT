#include "domain/Customer.h"
#include <iostream>

// ========================= Constructors =========================

Customer::Customer()
    : _iIndex(0), _strIdNumber(""), _strFullName("")
{
}

Customer::Customer(int index, const std::string &idNumber,
                   const std::string &fullName)
    : _iIndex(index), _strIdNumber(idNumber), _strFullName(fullName)
{
}

// ============================= Setter =============================

void Customer::setIndex(int index)
{
    _iIndex = index;
}

void Customer::setIdNumber(const std::string &idNumber)
{
    _strIdNumber = idNumber;
}

void Customer::setFullName(const std::string &fullName)
{
    _strFullName = fullName;
}

// ============================= Getter =============================

int Customer::getIndex() const
{
    return _iIndex;
}

std::string Customer::getIdNumber() const
{
    return _strIdNumber;
}

std::string Customer::getFullName() const
{
    return _strFullName;
}

// =============================== Print ===============================

void Customer::print() const
{
    std::cout << "\n===== THONG TIN KHACH HANG =====\n";
    std::cout << "STT          : " << _iIndex << "\n";
    std::cout << "CMND/CCCD    : " << _strIdNumber << "\n";
    std::cout << "Ho ten       : " << _strFullName << "\n";
    std::cout << "=====================================\n";
}

// ============================= Comparison =============================

bool Customer::operator==(const Customer &other) const
{
    return (_iIndex     == other._iIndex &&
            _strIdNumber == other._strIdNumber &&
            _strFullName == other._strFullName);
}
