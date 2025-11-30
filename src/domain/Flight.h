#pragma once

#include <string>
#include "../core/Config.h"
#include "../core/SeatList.h"
#include "../data_structure/LinkedList.h"
#include "Ticket.h"

// Thông tin 1 chuyến bay: gắn với 1 máy bay, có danh sách ghế và danh sách vé
class Flight
{
private:
    std::string   _strFlightId;     // Ma chuyen bay
    std::string   _strPlaneId;      // Ma may bay (so hieu may bay)
    std::string   _strDate;         // Ngay gio khoi hanh (dang string)
    std::string   _strDestination;  // San bay den
    FlightStatus  _eStatus;         // Trang thai chuyen bay

    SeatList             _seatList;     // Danh sach ghe: trong / da dat
    LinkedList<Ticket>   _listTickets;  // Danh sach ve cua chuyen bay
    std::string _strDepartTime;   // HH:MM
public:
    Flight();
    Flight(const std::string &strFlightId,
           const std::string &strPlaneId,
           const std::string &strDate,
           const std::string &strDestination,
           FlightStatus eStatus,
           int iTotalSeats);
    Flight(const Flight &other);
    ~Flight();

    // ===== Setter =====
    void setFlightId(const std::string &strFlightId);
    void setPlaneId(const std::string &strPlaneId);
    void setDate(const std::string &strDate);
    void setDestination(const std::string &strDestination);
    void setStatus(FlightStatus eStatus);

    // ===== Getter =====
    std::string  getFlightId() const;
    std::string  getPlaneId() const;
    std::string  getDate() const;
    std::string  getDestination() const;
    FlightStatus getStatus() const;
    std::string  getStatusString() const;

    int  getTotalSeatCount() const;
    int  getFreeSeatCount() const;
    int  getBookedSeatCount() const;

    bool isCancelled() const;
    bool isFull() const;
    bool isAvailable() const;

    // Lấy chuỗi danh sách ghế đã đặt, ví dụ "2, 3, 5"
    std::string getBookedSeatListString() const;

    // ===== Nghiệp vụ vé =====
    // Dat ve cho 1 khach (neu ghe hop le va con trong)
    bool bookTicket(const std::string &strCustomerId,
                    const std::string &strCustomerName,
                    int iSeatNumber);

    // Huy ve theo so ghe
    bool cancelTicketBySeat(int iSeatNumber);

    // ===== Hien thi =====
    // In 1 dong thong tin chuyen bay (dung trong danh sach)
    void print() const;

    // In chi tiet chuyen bay + so ghe + thong tin tong quan
    void printDetail() const;

    std::string getDepartTime() const;
    void setDepartTime(const std::string& time);

};
