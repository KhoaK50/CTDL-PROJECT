#pragma once

#include <string>
#include <vector>

#include "../domain/Airplane.h"
#include "../domain/Flight.h"
#include "../domain/Customer.h"
#include "../domain/Ticket.h"
#include "../domain/AdminAccount.h"
#include "../data_structure/Queue.h"
#include "../data_structure/Stack.h"

// Yeu cau dat ve cua nguoi dung ngoai he thong
struct BookingRequest
{
    Customer    CustomerInfo;   // Khach hang (CMND + ho ten + index)
    std::string FlightId;       // Ma chuyen bay (da chuan hoa)
    int         SeatNumber;     // So ghe muon dat
    std::string RequestTime;    // Thoi gian gui yeu cau (chuoi)
};

class Repository
{
private:
    // Du lieu chinh
    std::vector<Airplane>     _vAirplanes;
    std::vector<Flight>       _vFlights;
    std::vector<Customer>     _vCustomers;
    std::vector<AdminAccount> _vAdmins;

    // CTDL dung cho nghiep vu
    Queue<BookingRequest> _qWaitingBookings;  // Hang doi yeu cau dat ve
    Stack<BookingRequest> _stUndoRequests;    // Stack cac booking vua duoc duyet (de UNDO)

    // --- Ham ho tro load/save tung file ---
    bool loadAirplanes();
    bool saveAirplanes() const;

    bool loadFlights();
    bool saveFlights() const;

    bool loadCustomers();
    bool saveCustomers() const;

    bool loadAdmins();
    bool saveAdmins() const;

    bool loadTickets();   // Duyet thu muc tickets/ de khoi phuc trang thai

    // ========== Luu / load danh sach booking dang cho & log thanh cong ==========

    // Load lai hang doi booking tu file VeDangCho.txt
    bool loadWaitingBookings();

    // Append 1 booking moi vao file VeDangCho.txt
    bool appendWaitingBookingToFile(const BookingRequest& bookingRequest) const;

    // Sau khi xu ly 1 booking (deQueue) thi xoa dong dau trong VeDangCho.txt
    bool popWaitingBookingFromFile() const;

    // Ghi 1 ve dat thanh cong vao VeThanhCong.txt (log)
    bool appendSuccessfulTicketToFile(const Ticket& ticket,
                                      const std::string& strRequestTime) const;

    // Ghi / xoa mot file ve .txt
    bool writeTicketFile(const Ticket& ticket) const;
    bool deleteTicketFile(const std::string& strFlightId,
                          int iSeatNumber) const;

    // Tim chuyen bay theo ma
    Flight*       findFlightById(const std::string& strFlightId);
    const Flight* findFlightById(const std::string& strFlightId) const;

    // Kiem tra thoi gian con cho phep dat ve (1..90 ngay truoc ngay bay)
    bool isBookingTimeAllowed(const Flight& flight) const;

public:
    Repository();

    // Load / save tat ca cac file du lieu
    bool loadAllData();
    bool saveAllData() const;

    // Kiem tra dang nhap admin
    bool checkAdminLogin(const std::string& strUserName,
                         const std::string& strPassword) const;

    // In danh sach chuyen bay
    void printAllFlights() const;

    // --- Nghiep vu dat / huy / undo ---
    // Khach ben ngoai dang ky dat ve -> dua vao Queue
    bool userRegisterBooking(const std::string& strFlightId,
                             const std::string& strIdNumber,
                             const std::string& strFullName,
                             int iSeatNumber);

    // Admin xu ly yeu cau dat ve dau hang doi
    bool adminProcessNextBooking();

    // Hien danh sach cac yeu cau dat ve dang cho (Queue)
    void printWaitingBookings();

    // Tu choi (loai bo) booking dau hang doi ma khong dat ve
    bool rejectNextBooking();

    // Admin huy ve truc tiep theo ma chuyen + so ghe
    bool adminCancelTicket(const std::string& strFlightId,
                           int iSeatNumber);

    // Huy giao dich dat ve gan nhat (hoan tac: dua booking ve hang doi)
    bool undoLastBooking();

    // --- Thong ke ---
    void statisticsPassengersOfFlight(const std::string& strFlightId) const;
    void statisticsFreeSeats(const std::string& strFlightId) const;
    void statisticsPlaneUsage(const std::string& strPlaneId) const;

    // Lay con tro const toi chuyen bay (tim khong phan biet hoa thuong)
    const Flight* getFlightConst(const std::string& strFlightId) const;
};
