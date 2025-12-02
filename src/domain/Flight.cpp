#include "domain/Flight.h"
#include "core/Config.h"
#include "utils/Utils.h"
#include "data_structure/LinkedList.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// ----------------------------------------------------------
// Helper Functions (Internal linkage)
// ----------------------------------------------------------

/************
* @Description Chuyển đổi trạng thái enum sang chuỗi hiển thị
* @param eStatus Trạng thái chuyến bay
* @return Chuỗi mô tả trạng thái
*************/
static string flightStatusToString(FlightStatus eStatus)
{
    switch (eStatus)
    {
    case FLIGHT_CANCELLED:
        return "HUY CHUYEN";
    case FLIGHT_AVAILABLE:
        return "CON VE";
    case FLIGHT_FULL:
        return "HET VE";
    case FLIGHT_COMPLETED:
        return "HOAN TAT";
    default:
        return "UNKNOWN";
    }
}

/************
* @Description Lấy mã màu tương ứng với trạng thái
* @param eStatus Trạng thái chuyến bay
* @return Mã màu (int) defined trong Config.h
*************/
static int getStatusColor(FlightStatus eStatus)
{
    switch (eStatus)
    {
    case FLIGHT_CANCELLED:  // Hủy chuyến -> đỏ
        return COLOR_ERROR;
    case FLIGHT_AVAILABLE:  // Còn vé -> vàng
        return COLOR_WARNING;
    case FLIGHT_FULL:       // Hết vé -> xanh dương (highlight)
        return COLOR_HIGHLIGHT;
    case FLIGHT_COMPLETED:  // Hoàn tất -> xanh lá
        return COLOR_SUCCESS;
    default:
        return COLOR_DEFAULT;
    }
}

// ========================= Constructors =========================

/************
* @Description Constructor mặc định
*************/
Flight::Flight()
    : _strFlightId(""),
      _strPlaneId(""),
      _strDate(""),
      _strDepartTime(""),
      _strDestination(""),
      _eStatus(FLIGHT_AVAILABLE),
      _SeatList(0),
      _ListTickets()
{
}

/************
* @Description Constructor đầy đủ tham số
*************/
Flight::Flight(const string &strFlightId,
               const string &strPlaneId,
               const string &strDate,
               const string &strDestination,
               FlightStatus eStatus,
               int iTotalSeats)
    : _strFlightId(strFlightId),
      _strPlaneId(strPlaneId),
      _strDate(strDate),
      _strDepartTime(""),
      _strDestination(strDestination),
      _eStatus(eStatus),
      _SeatList(iTotalSeats),
      _ListTickets()
{
}

/************
* @Description Copy Constructor
*************/
Flight::Flight(const Flight &other)
    : _strFlightId(other._strFlightId),
      _strPlaneId(other._strPlaneId),
      _strDate(other._strDate),
      _strDepartTime(other._strDepartTime),
      _strDestination(other._strDestination),
      _eStatus(other._eStatus),
      _SeatList(other._SeatList),
      _ListTickets(other._ListTickets)
{
}

/************
* @Description Destructor
*************/
Flight::~Flight()
{
}

// ============================== Setter ==============================

/************
* @Description Thiết lập mã chuyến bay
*************/
void Flight::setFlightId(const string &strFlightId)
{
    this->_strFlightId = strFlightId;
}

/************
* @Description Thiết lập mã máy bay
*************/
void Flight::setPlaneId(const string &strPlaneId)
{
    this->_strPlaneId = strPlaneId;
}

/************
* @Description Thiết lập ngày khởi hành
*************/
void Flight::setDate(const string &strDate)
{
    this->_strDate = strDate;
}

/************
* @Description Thiết lập giờ khởi hành
*************/
void Flight::setDepartTime(const string &strDepartTime)
{
    this->_strDepartTime = strDepartTime;
}

/************
* @Description Thiết lập điểm đến
*************/
void Flight::setDestination(const string &strDestination)
{
    this->_strDestination = strDestination;
}

/************
* @Description Thiết lập trạng thái
*************/
void Flight::setStatus(FlightStatus eStatus)
{
    this->_eStatus = eStatus;
}

// ============================== Getter ==============================

/************
* @Description Lấy mã chuyến bay
*************/
string Flight::getFlightId() const
{
    return this->_strFlightId;
}

/************
* @Description Lấy mã máy bay
*************/
string Flight::getPlaneId() const
{
    return this->_strPlaneId;
}

/************
* @Description Lấy ngày khởi hành
*************/
string Flight::getDate() const
{
    return this->_strDate;
}

/************
* @Description Lấy giờ khởi hành
*************/
string Flight::getDepartTime() const
{
    return this->_strDepartTime;
}

/************
* @Description Lấy điểm đến
*************/
string Flight::getDestination() const
{
    return this->_strDestination;
}

/************
* @Description Lấy trạng thái (enum)
*************/
FlightStatus Flight::getStatus() const
{
    return this->_eStatus;
}

/************
* @Description Lấy chuỗi mô tả trạng thái
*************/
string Flight::getStatusString() const
{
    return flightStatusToString(this->_eStatus);
}

/************
* @Description Lấy tổng số ghế
*************/
int Flight::getTotalSeatCount() const
{
    return this->_SeatList.getTotalSeats();
}

/************
* @Description Lấy số ghế trống
*************/
int Flight::getFreeSeatCount() const
{
    return this->_SeatList.countFreeSeats();
}

/************
* @Description Lấy số ghế đã đặt
*************/
int Flight::getBookedSeatCount() const
{
    return this->getTotalSeatCount() - this->getFreeSeatCount();
}

/************
* @Description Lấy chuỗi danh sách ghế đã đặt (ví dụ: "2, 3, 5")
*************/
string Flight::getBookedSeatListString() const
{
    // Rule 27: Dùng this-> truy cập _ListTickets
    vector<Ticket> vTickets = this->_ListTickets.toVector();
    
    if (vTickets.empty())
    {
        return "-";
    }

    // Sort vé theo số ghế tăng dần để hiển thị đẹp
    sort(vTickets.begin(), vTickets.end(),
         [](const Ticket &a, const Ticket &b)
         {
             return a.getSeatNumber() < b.getSeatNumber();
         });

    ostringstream oss;
    for (size_t i = 0; i < vTickets.size(); ++i)
    {
        if (i > 0)
        {
            oss << ", ";
        }
        oss << vTickets[i].getSeatNumber();
    }
    return oss.str();
}

// ============================ Status Checks ============================

/************
* @Description Kiểm tra đã hủy
*************/
bool Flight::isCancelled() const
{
    return (this->_eStatus == FLIGHT_CANCELLED);
}

/************
* @Description Kiểm tra đã đầy vé
*************/
bool Flight::isFull() const
{
    return (this->_eStatus == FLIGHT_FULL);
}

/************
* @Description Kiểm tra còn khả dụng
*************/
bool Flight::isAvailable() const
{
    return (this->_eStatus == FLIGHT_AVAILABLE);
}

// ========================= Business Logic =========================

/************
* @Description Đặt vé cho khách hàng
*************/
bool Flight::bookTicket(const string &strCustomerId,
                        const string &strCustomerName,
                        int iSeatNumber)
{
    // 1. Kiem tra trang thai cho phep dat ve
    if (this->_eStatus == FLIGHT_CANCELLED ||
        this->_eStatus == FLIGHT_FULL ||
        this->_eStatus == FLIGHT_COMPLETED)
    {
        cout << ">> Loi: Trang thai chuyen bay khong cho phep dat ve!\n";
        return false;
    }

    // 2. Thu dat ghe trong SeatList (dùng this->)
    if (!this->_SeatList.bookSeat(iSeatNumber))
    {
        cout << ">> Loi: So ghe khong hop le hoac da duoc dat!\n";
        return false;
    }

    // 3. Tao ve va them vao danh sach ve
    Ticket newTicket(this->_strFlightId, strCustomerId, strCustomerName, iSeatNumber);
    // ID vé được tạo tự động trong constructor của Ticket hoặc gọi hàm makeTicketId
    // newTicket.makeTicketId(); // Nếu constructor đã gọi rồi thì dòng này thừa

    this->_ListTickets.addTail(newTicket);

    // 4. Cap nhat trang thai (neu khong con ghe trong -> FULL)
    if (this->_SeatList.countFreeSeats() == 0)
    {
        this->_eStatus = FLIGHT_FULL;
    }
    else if (this->_eStatus != FLIGHT_AVAILABLE && this->_eStatus != FLIGHT_CANCELLED)
    {
        this->_eStatus = FLIGHT_AVAILABLE;
    }

    return true;
}

/************
* @Description Hủy vé theo số ghế
*************/
bool Flight::cancelTicketBySeat(int iSeatNumber)
{
    // 1. Thu huy ghe trong SeatList
    if (!this->_SeatList.cancelSeat(iSeatNumber))
    {
        cout << ">> Loi: Khong the huy ghe nay (so ghe khong hop le hoac dang trong)!\n";
        return false;
    }

    // 2. Xoa ve trong danh sach ve

    Ticket key(this->_strFlightId, "", "", iSeatNumber);
   
    
    this->_ListTickets.remove(key); 

    // 3. Neu chuyen bay dang la FULL ma co ghe trong -> chuyen ve AVAILABLE
    if (this->_eStatus == FLIGHT_FULL && this->_SeatList.countFreeSeats() > 0)
    {
        this->_eStatus = FLIGHT_AVAILABLE;
    }

    return true;
}

// =============================== Display ===============================

/************
* @Description In thông tin tóm tắt (1 dòng)
*************/
void Flight::print() const
{
    // ======= Chuẩn bị dữ liệu =======
    vector<Ticket> vTickets = this->_ListTickets.toVector();
    sort(vTickets.begin(), vTickets.end(),
         [](const Ticket& a, const Ticket& b)
         {
             return a.getSeatNumber() < b.getSeatNumber();
         });

    vector<int> seats;
    seats.reserve(vTickets.size());
    for (auto &t : vTickets)
        seats.push_back(t.getSeatNumber());

    const int SEATS_PER_LINE = 15;      // Magic number chấp nhận được vì là const local
    size_t n = seats.size();
    size_t idx = 0;

    // ======= In dòng đầu (có đầy đủ cột) =======
    int color = getStatusColor(this->_eStatus);
    setTextColor(color);

    cout << left
         << setw(8)  << this->_strFlightId
         << setw(8)  << this->_strPlaneId
         << setw(12) << this->_strDate
         << setw(8)  << this->_strDepartTime
         << setw(20) << this->_strDestination
         << setw(15) << this->getStatusString();

    // In chunk đầu tiên của VeDaDat
    if (n == 0)
    {
        cout << "-\n";
        setTextColor(COLOR_DEFAULT);
        return;
    }

    // Dòng đầu chỉ in 1 chunk
    for (size_t i = 0; i < n && i < SEATS_PER_LINE; ++i)
    {
        cout << seats[i];
        if (i + 1 < n && i + 1 < SEATS_PER_LINE)
            cout << ", ";
    }
    cout << "\n";
    idx += SEATS_PER_LINE;

    // ======= Các dòng tiếp theo (thụt cột để hiển thị danh sách vé dài) =======
    while (idx < n)
    {
        cout << left
             << setw(8)  << ""
             << setw(8)  << ""
             << setw(12) << ""
             << setw(8)  << ""
             << setw(20) << ""
             << setw(15) << "";

        for (size_t i = idx; i < n && i < idx + SEATS_PER_LINE; ++i)
        {
            cout << seats[i];
            if (i + 1 < n && i + 1 < idx + SEATS_PER_LINE)
                cout << ", ";
        }
        cout << "\n";
        idx += SEATS_PER_LINE;
    }

    setTextColor(COLOR_DEFAULT);
}

/************
* @Description In chi tiết chuyến bay
*************/
void Flight::printDetail() const
{
    cout << "========================================\n";
    cout << " CHI TIET CHUYEN BAY: " << this->_strFlightId << "\n";
    cout << " May bay    : " << this->_strPlaneId       << "\n";
    cout << " Ngay di    : " << this->_strDate          << "\n";
    cout << " Gio di     : " << this->_strDepartTime    << "\n";   
    cout << " Diem den   : " << this->_strDestination   << "\n";
    cout << " Trang thai : " << this->getStatusString() << "\n";
    cout << " Tong ghe   : " << this->getTotalSeatCount()   << "\n";
    cout << " Ghe trong  : " << this->getFreeSeatCount()    << "\n";
    cout << " Da dat     : " << this->getBookedSeatCount()  << "\n";
    cout << "----------------------------------------\n";

    this->_SeatList.printSeatMap();

    cout << "\n(Thong tin chi tiet hanh khach se duoc thong ke o chuc nang rieng.)\n";
    cout << "========================================\n";
}