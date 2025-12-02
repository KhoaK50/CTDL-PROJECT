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

/************
* @Description Struct lưu trữ yêu cầu đặt vé của người dùng
* Dùng cho hàng đợi (Queue)
*************/
struct BookingRequest
{
    Customer    CustomerInfo;   // Khách hàng (CMND + họ tên + index)
    std::string FlightId;       // Mã chuyến bay (đã chuẩn hóa)
    int         SeatNumber;     // Số ghế muốn đặt
    std::string RequestTime;    // Thời gian gửi yêu cầu (chuỗi)
};

/************
* @Description Lớp Kho chứa dữ liệu (Repository)
* Quản lý toàn bộ dữ liệu, file I/O và logic nghiệp vụ chính
*************/
class Repository
{
private:
    // ========================= Data Storage =========================
    std::vector<Airplane>     _vAirplanes;
    std::vector<Flight>       _vFlights;
    std::vector<Customer>     _vCustomers;
    std::vector<AdminAccount> _vAdmins;

    // ========================= Data Structures =========================
    Queue<BookingRequest> _qWaitingBookings;  // Hàng đợi yêu cầu đặt vé
    Stack<BookingRequest> _stUndoRequests;    // Stack các booking vừa được duyệt (để UNDO)

    // ========================= File I/O Helpers =========================

    /************
    * @Description Load danh sách máy bay từ file
    * @return true nếu thành công
    *************/
    bool loadAirplanes();

    /************
    * @Description Lưu danh sách máy bay xuống file
    * @return true nếu thành công
    *************/
    bool saveAirplanes() const;

    /************
    * @Description Load danh sách chuyến bay từ file
    * @return true nếu thành công
    *************/
    bool loadFlights();

    /************
    * @Description Lưu danh sách chuyến bay xuống file
    * @return true nếu thành công
    *************/
    bool saveFlights() const;

    /************
    * @Description Load danh sách khách hàng từ file
    * @return true nếu thành công
    *************/
    bool loadCustomers();

    /************
    * @Description Lưu danh sách khách hàng xuống file
    * @return true nếu thành công
    *************/
    bool saveCustomers() const;

    /************
    * @Description Load danh sách admin từ file
    * @return true nếu thành công
    *************/
    bool loadAdmins();

    /************
    * @Description Lưu danh sách admin xuống file
    * @return true nếu thành công
    *************/
    bool saveAdmins() const;

    /************
    * @Description Quét thư mục tickets/ để khôi phục vé cho các chuyến bay
    * @return true nếu thành công
    *************/
    bool loadTickets();

    // ========================= Queue Persistence =========================

    /************
    * @Description Load lại hàng đợi booking từ file VeDangCho.txt khi khởi động
    * @return true nếu thành công
    *************/
    bool loadWaitingBookings();

    /************
    * @Description Ghi nối tiếp 1 booking mới vào file VeDangCho.txt
    * @param bookingRequest Yêu cầu cần lưu
    * @return true nếu thành công
    *************/
    bool appendWaitingBookingToFile(const BookingRequest& bookingRequest) const;

    /************
    * @Description Xóa dòng đầu tiên trong VeDangCho.txt (sau khi Dequeue)
    * @return true nếu thành công
    *************/
    bool popWaitingBookingFromFile() const;

    // ========================= Ticket Logging =========================

    /************
    * @Description Ghi log vé đặt thành công vào VeThanhCong.txt
    * @param ticket Vé đã tạo
    * @param strRequestTime Thời gian đặt vé
    * @return true nếu thành công
    *************/
    bool appendSuccessfulTicketToFile(const Ticket& ticket,
                                      const std::string& strRequestTime) const;

    /************
    * @Description Ghi file chi tiết vé (.txt) vào thư mục tickets/
    * @param ticket Vé cần ghi
    * @return true nếu thành công
    *************/
    bool writeTicketFile(const Ticket& ticket) const;

    /************
    * @Description Xóa file vé trong thư mục tickets/
    * @param strFlightId Mã chuyến bay
    * @param iSeatNumber Số ghế
    * @return true nếu xóa thành công
    *************/
    bool deleteTicketFile(const std::string& strFlightId,
                          int iSeatNumber) const;

    // ========================= Internal Logic Helpers =========================

    /************
    * @Description Tìm chuyến bay theo ID (Non-const version)
    * @param strFlightId Mã chuyến bay
    * @return Con trỏ đến Flight hoặc nullptr
    *************/
    Flight* findFlightById(const std::string& strFlightId);

    /************
    * @Description Tìm chuyến bay theo ID (Const version)
    * @param strFlightId Mã chuyến bay
    * @return Con trỏ const đến Flight hoặc nullptr
    *************/
    const Flight* findFlightById(const std::string& strFlightId) const;

    /************
    * @Description Kiểm tra thời gian đặt vé có hợp lệ không (1..90 ngày trước bay)
    * @param flight Chuyến bay cần kiểm tra
    * @return true nếu trong khoảng thời gian cho phép
    *************/
    bool isBookingTimeAllowed(const Flight& flight) const;

public:
    /************
    * @Description Constructor mặc định
    *************/
    Repository();

    // ========================= Public I/O =========================

    /************
    * @Description Load toàn bộ dữ liệu hệ thống
    * @return true nếu tất cả thành công
    *************/
    bool loadAllData();

    /************
    * @Description Lưu toàn bộ dữ liệu hệ thống
    * @return true nếu tất cả thành công
    *************/
    bool saveAllData() const;

    // ========================= Authentication =========================

    /************
    * @Description Kiểm tra đăng nhập Admin
    * @param strUserName Tên đăng nhập
    * @param strPassword Mật khẩu
    * @return true nếu đúng
    *************/
    bool checkAdminLogin(const std::string& strUserName,
                         const std::string& strPassword) const;

    // ========================= Display =========================

    /************
    * @Description In danh sách tất cả chuyến bay
    *************/
    void printAllFlights() const;

    // ========================= Business Logic (Queue/Stack) =========================

    /************
    * @Description Người dùng đăng ký đặt vé (đưa vào Queue)
    * @param strFlightId Mã chuyến bay
    * @param strIdNumber CMND/CCCD
    * @param strFullName Họ tên
    * @param iSeatNumber Số ghế
    * @return true nếu thêm vào hàng đợi thành công
    *************/
    bool userRegisterBooking(const std::string& strFlightId,
                             const std::string& strIdNumber,
                             const std::string& strFullName,
                             int iSeatNumber);

    /************
    * @Description Admin xử lý (duyệt) yêu cầu đặt vé ở đầu hàng đợi
    * @return true nếu duyệt thành công
    *************/
    bool adminProcessNextBooking();

    /************
    * @Description Hiển thị danh sách các yêu cầu đang chờ trong Queue
    *************/
    void printWaitingBookings();

    /************
    * @Description Từ chối (hủy) yêu cầu đặt vé ở đầu hàng đợi
    * @return true nếu từ chối thành công
    *************/
    bool rejectNextBooking();

    /************
    * @Description Admin hủy vé trực tiếp (không qua Queue)
    * @param strFlightId Mã chuyến bay
    * @param iSeatNumber Số ghế cần hủy
    * @return true nếu hủy thành công
    *************/
    bool adminCancelTicket(const std::string& strFlightId,
                           int iSeatNumber);

    /************
    * @Description Hoàn tác (Undo) lệnh đặt vé gần nhất (dùng Stack)
    * @return true nếu Undo thành công
    *************/
    bool undoLastBooking();

    // ========================= Statistics =========================

    /************
    * @Description Thống kê hành khách của một chuyến bay
    * @param strFlightId Mã chuyến bay
    *************/
    void statisticsPassengersOfFlight(const std::string& strFlightId) const;

    /************
    * @Description Thống kê ghế trống của một chuyến bay
    * @param strFlightId Mã chuyến bay
    *************/
    void statisticsFreeSeats(const std::string& strFlightId) const;

    /************
    * @Description Thống kê tần suất hoạt động của máy bay
    * @param strPlaneId Mã máy bay
    *************/
    void statisticsPlaneUsage(const std::string& strPlaneId) const;

    // ========================= Accessors =========================

    /************
    * @Description Lấy thông tin chuyến bay (Read-only)
    * @param strFlightId Mã chuyến bay
    * @return Con trỏ const đến Flight hoặc nullptr
    *************/
    const Flight* getFlightConst(const std::string& strFlightId) const;
};