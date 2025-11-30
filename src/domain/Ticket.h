#pragma once

#include <string>
#include <iostream>

class Ticket
{
private:
    std::string _strTicketId;      // Ma ve
    std::string _strFlightId;      // Ma chuyen bay
    std::string _strCustomerId;    // Ma khach hang
    std::string _strCustomerName;  // Ten khach hang
    int         _iSeatNumber;      // So ghe

public:
    Ticket();
    Ticket(const std::string &strFlightId,
           const std::string &strCustomerId,
           const std::string &strCustomerName,
           int iSeatNumber);
    Ticket(const Ticket &src);
    ~Ticket();

    // Setter
    void setFlightId(const std::string &strFlightId);
    void setCustomerId(const std::string &strCustomerId);
    void setCustomerName(const std::string &strCustomerName);
    void setSeatNumber(int iSeatNumber);

    // Getter
    std::string getTicketId() const;
    std::string getFlightId() const;
    std::string getCustomerId() const;
    std::string getCustomerName() const;
    int         getSeatNumber() const;

    // Tu tao ma ve dua tren ma chuyen bay + so ghe
    void makeTicketId();

    // In thong tin ve
    void print() const;

    // So sanh bang nhau (can cho LinkedList/Queue/Stack)
    bool operator==(const Ticket &other) const;

    // So sanh nho hon (de sort theo so ghe trong LinkedList<Ticket>)
    bool operator<(const Ticket &other) const;
};
