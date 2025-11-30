#include "domain/Ticket.h"

// ===============================================
// Constructor mac dinh
// ===============================================
Ticket::Ticket()
    : _strTicketId(""),
      _strFlightId(""),
      _strCustomerId(""),
      _strCustomerName(""),
      _iSeatNumber(0)
{
}

// ===============================================
// Constructor day du (TicketId se tu sinh)
// ===============================================
Ticket::Ticket(const std::string &strFlightId,
               const std::string &strCustomerId,
               const std::string &strCustomerName,
               int iSeatNumber)
    : _strTicketId(""),
      _strFlightId(strFlightId),
      _strCustomerId(strCustomerId),
      _strCustomerName(strCustomerName),
      _iSeatNumber(iSeatNumber)
{
    // Tu dong tao ma ve
    makeTicketId();
}

// ===============================================
// Copy constructor
// ===============================================
Ticket::Ticket(const Ticket &src)
    : _strTicketId(src._strTicketId),
      _strFlightId(src._strFlightId),
      _strCustomerId(src._strCustomerId),
      _strCustomerName(src._strCustomerName),
      _iSeatNumber(src._iSeatNumber)
{
}

// ===============================================
// Destructor
// ===============================================
Ticket::~Ticket()
{
}

// ===============================================
// Setter
// ===============================================
void Ticket::setFlightId(const std::string &strFlightId)
{
    _strFlightId = strFlightId;
}

void Ticket::setCustomerId(const std::string &strCustomerId)
{
    _strCustomerId = strCustomerId;
}

void Ticket::setCustomerName(const std::string &strCustomerName)
{
    _strCustomerName = strCustomerName;
}

void Ticket::setSeatNumber(int iSeatNumber)
{
    _iSeatNumber = iSeatNumber;
}

// ===============================================
// Getter
// ===============================================
std::string Ticket::getTicketId() const
{
    return _strTicketId;
}

std::string Ticket::getFlightId() const
{
    return _strFlightId;
}

std::string Ticket::getCustomerId() const
{
    return _strCustomerId;
}

std::string Ticket::getCustomerName() const
{
    return _strCustomerName;
}

int Ticket::getSeatNumber() const
{
    return _iSeatNumber;
}

// ===============================================
// Tu tao ma ve dua tren thong tin chuyen va ghe
// Vi du: FL001-15
// ===============================================
void Ticket::makeTicketId()
{
    _strTicketId = _strFlightId + "-" + std::to_string(_iSeatNumber);
}

// ===============================================
// Hien thi thong tin ve
// ===============================================
void Ticket::print() const
{
    std::cout << "Ma ve         : " << _strTicketId     << "\n";
    std::cout << "Ma CB         : " << _strFlightId     << "\n";
    std::cout << "CMND/CCCD     : " << _strCustomerId   << "\n";
    std::cout << "Ho ten        : " << _strCustomerName << "\n";
    std::cout << "So ghe        : " << _iSeatNumber     << "\n";
}

// ===============================================
// So sanh bang nhau (phuc vu CTDL template)
// ===============================================
bool Ticket::operator==(const Ticket &other) const
{
    // So sanh theo ma chuyen bay + so ghe
    // (xem day la khoa chinh de tim / xoa ve)
    return (_strFlightId == other._strFlightId &&
            _iSeatNumber == other._iSeatNumber);
}

// ===============================================
// So sanh nho hon (sap xep theo so ghe)
// Dung cho LinkedList<Ticket>::selectionSort()
// ===============================================
bool Ticket::operator<(const Ticket &other) const
{
    return _iSeatNumber < other._iSeatNumber;
}
