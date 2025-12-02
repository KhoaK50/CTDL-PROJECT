#include "domain/Ticket.h"
#include <iostream>
#include <string>

using namespace std;

// ========================= Constructors =========================

/************
* @Description Constructor mặc định
*************/
Ticket::Ticket()
{
    this->_strTicketId = "";
    this->_strFlightId = "";
    this->_strCustomerId = "";
    this->_strCustomerName = "";
    this->_iSeatNumber = 0;
}

/************
* @Description Constructor đầy đủ tham số
* @param strFlightId Mã chuyến bay
* @param strCustomerId Mã khách hàng
* @param strCustomerName Tên khách hàng
* @param iSeatNumber Số ghế
*************/
Ticket::Ticket(const string &strFlightId,
               const string &strCustomerId,
               const string &strCustomerName,
               int iSeatNumber)
{
    this->_strTicketId = ""; 
    this->_strFlightId = strFlightId;
    this->_strCustomerId = strCustomerId;
    this->_strCustomerName = strCustomerName;
    this->_iSeatNumber = iSeatNumber;

    this->makeTicketId();
}

/************
* @Description Copy Constructor
*************/
Ticket::Ticket(const Ticket &src)
{
    this->_strTicketId = src._strTicketId;
    this->_strFlightId = src._strFlightId;
    this->_strCustomerId = src._strCustomerId;
    this->_strCustomerName = src._strCustomerName;
    this->_iSeatNumber = src._iSeatNumber;
}

/************
* @Description Destructor
*************/
Ticket::~Ticket()
{
}

// ========================= Setters =========================

/************
* @Description Thiết lập mã chuyến bay
*************/
void Ticket::setFlightId(const string &strFlightId)
{
    this->_strFlightId = strFlightId;
    this->makeTicketId();
}

/************
* @Description Thiết lập mã khách hàng
*************/
void Ticket::setCustomerId(const string &strCustomerId)
{
    this->_strCustomerId = strCustomerId;
}

/************
* @Description Thiết lập tên khách hàng
*************/
void Ticket::setCustomerName(const string &strCustomerName)
{
    this->_strCustomerName = strCustomerName;
}

/************
* @Description Thiết lập số ghế
*************/
void Ticket::setSeatNumber(int iSeatNumber)
{
    this->_iSeatNumber = iSeatNumber;
    this->makeTicketId();
}

// ========================= Getters =========================

/************
* @Description Lấy mã vé
*************/
string Ticket::getTicketId() const
{
    return this->_strTicketId;
}

/************
* @Description Lấy mã chuyến bay
*************/
string Ticket::getFlightId() const
{
    return this->_strFlightId;
}

/************
* @Description Lấy mã khách hàng
*************/
string Ticket::getCustomerId() const
{
    return this->_strCustomerId;
}

/************
* @Description Lấy tên khách hàng
*************/
string Ticket::getCustomerName() const
{
    return this->_strCustomerName;
}

/************
* @Description Lấy số ghế
*************/
int Ticket::getSeatNumber() const
{
    return this->_iSeatNumber;
}

// ========================= Business & Utils =========================

/************
* @Description Tự động tạo mã vé dựa trên thông tin chuyến và ghế
* Format: [FlightId]-[SeatNumber] (Ví dụ: FL001-15)
*************/
void Ticket::makeTicketId()
{
    this->_strTicketId = this->_strFlightId + "-" + to_string(this->_iSeatNumber);
}

/************
* @Description Hiển thị thông tin vé
*************/
void Ticket::print() const
{
    cout << "Ma ve         : " << this->_strTicketId     << "\n";
    cout << "Ma CB         : " << this->_strFlightId     << "\n";
    cout << "CMND/CCCD     : " << this->_strCustomerId   << "\n";
    cout << "Ho ten        : " << this->_strCustomerName << "\n";
    cout << "So ghe        : " << this->_iSeatNumber     << "\n";
}

// ========================= Operators =========================

/************
* @Description So sánh bằng nhau (phục vụ CTDL template)
*************/
bool Ticket::operator==(const Ticket &other) const
{
    // So sánh theo mã chuyến bay + số ghế (khóa chính logic)
    return (this->_strFlightId == other._strFlightId &&
            this->_iSeatNumber == other._iSeatNumber);
}

/************
* @Description So sánh nhỏ hơn (sắp xếp theo số ghế)
*************/
bool Ticket::operator<(const Ticket &other) const
{
    return this->_iSeatNumber < other._iSeatNumber;
}