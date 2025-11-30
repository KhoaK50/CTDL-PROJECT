#include "domain/Flight.h"
#include "core/Config.h"
#include "utils/Utils.h"
#include "data_structure/LinkedList.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>

using std::cout;
using std::endl;
using std::left;
using std::right;
using std::setw;
using std::string;

// ----------------------------------------------------------
// Ham ho tro: doi trang thai -> chuoi de in ra man hinh
// ----------------------------------------------------------
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

// ----------------------------------------------------------
// Chọn màu cho từng trạng thái chuyến bay
// ----------------------------------------------------------
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

Flight::Flight()
    : _strFlightId(""),
      _strPlaneId(""),
      _strDate(""),
      _strDestination(""),
      _eStatus(FLIGHT_AVAILABLE),
      _seatList(0),
      _listTickets(),
      _strDepartTime("")       
{
}

Flight::Flight(const string &strFlightId,
               const string &strPlaneId,
               const string &strDate,
               const string &strDestination,
               FlightStatus eStatus,
               int iTotalSeats)
    : _strFlightId(strFlightId),
      _strPlaneId(strPlaneId),
      _strDate(strDate),
      _strDestination(strDestination),
      _eStatus(eStatus),
      _seatList(iTotalSeats),
      _listTickets(),
      _strDepartTime("")      
{
}

Flight::Flight(const Flight &other)
    : _strFlightId(other._strFlightId),
      _strPlaneId(other._strPlaneId),
      _strDate(other._strDate),
      _strDestination(other._strDestination),
      _eStatus(other._eStatus),
      _seatList(other._seatList),
      _listTickets(other._listTickets),
      _strDepartTime(other._strDepartTime)  
{
}


Flight::~Flight()
{
    // _seatList va _listTickets tu dong giai phong
}

// ============================== Setter ==============================

void Flight::setFlightId(const string &strFlightId)
{
    _strFlightId = strFlightId;
}

void Flight::setPlaneId(const string &strPlaneId)
{
    _strPlaneId = strPlaneId;
}

void Flight::setDate(const string &strDate)
{
    _strDate = strDate;
}

void Flight::setDestination(const string &strDestination)
{
    _strDestination = strDestination;
}

void Flight::setStatus(FlightStatus eStatus)
{
    _eStatus = eStatus;
}

// ============================== Getter ==============================

string Flight::getFlightId() const
{
    return _strFlightId;
}

string Flight::getPlaneId() const
{
    return _strPlaneId;
}

string Flight::getDate() const
{
    return _strDate;
}

string Flight::getDestination() const
{
    return _strDestination;
}

FlightStatus Flight::getStatus() const
{
    return _eStatus;
}

string Flight::getStatusString() const
{
    return flightStatusToString(_eStatus);
}

std::string Flight::getDepartTime() const
{
    return _strDepartTime;
}

void Flight::setDepartTime(const std::string& time)
{
    _strDepartTime = time;
}

int Flight::getTotalSeatCount() const
{
    return _seatList.getTotalSeats();
}

int Flight::getFreeSeatCount() const
{
    return _seatList.countFreeSeats();
}

int Flight::getBookedSeatCount() const
{
    // Tong ghe - ghe trong = so ve da dat
    return getTotalSeatCount() - getFreeSeatCount();
}

// ===== Chuỗi danh sách ghế đã đặt (ví dụ: "2, 3, 5") =====
string Flight::getBookedSeatListString() const
{
    std::vector<Ticket> vTickets = _listTickets.toVector();
    if (vTickets.empty())
    {
        return "-";
    }

    std::sort(vTickets.begin(), vTickets.end(),
              [](const Ticket &a, const Ticket &b)
              {
                  return a.getSeatNumber() < b.getSeatNumber();
              });

    std::ostringstream oss;
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

// ============================ Trang thai ============================

bool Flight::isCancelled() const
{
    return (_eStatus == FLIGHT_CANCELLED);
}

bool Flight::isFull() const
{
    return (_eStatus == FLIGHT_FULL);
}

bool Flight::isAvailable() const
{
    return (_eStatus == FLIGHT_AVAILABLE);
}

// ========================= Nghiệp vụ vé =========================

bool Flight::bookTicket(const string &strCustomerId,
                        const string &strCustomerName,
                        int iSeatNumber)
{
    // 1. Kiem tra trang thai cho phep dat ve
    if (_eStatus == FLIGHT_CANCELLED ||
        _eStatus == FLIGHT_FULL ||
        _eStatus == FLIGHT_COMPLETED)
    {
        cout << ">> Loi: Trang thai chuyen bay khong cho phep dat ve!\n";
        return false;
    }

    // 2. Thu dat ghe trong SeatList
    if (!_seatList.bookSeat(iSeatNumber))
    {
        cout << ">> Loi: So ghe khong hop le hoac da duoc dat!\n";
        return false;
    }

    // 3. Tao ve va them vao danh sach ve
    Ticket newTicket(_strFlightId, strCustomerId, strCustomerName, iSeatNumber);
    newTicket.makeTicketId();

    _listTickets.addTail(newTicket);

    // 4. Cap nhat trang thai (neu khong con ghe trong -> FULL)
    if (_seatList.countFreeSeats() == 0)
    {
        _eStatus = FLIGHT_FULL;
    }
    else if (_eStatus != FLIGHT_AVAILABLE && _eStatus != FLIGHT_CANCELLED)
    {
        _eStatus = FLIGHT_AVAILABLE;
    }

    return true;
}

bool Flight::cancelTicketBySeat(int iSeatNumber)
{
    // 1. Thu huy ghe trong SeatList
    if (!_seatList.cancelSeat(iSeatNumber))
    {
        cout << ">> Loi: Khong the huy ghe nay (so ghe khong hop le hoac dang trong)!\n";
        return false;
    }

    // 2. Xoa ve trong danh sach ve
    Ticket key(_strFlightId, "", "", iSeatNumber);
    key.makeTicketId();
    _listTickets.remove(key);   // Neu khong tim thay thi ham remove() se khong lam gi

    // 3. Neu chuyen bay dang la FULL ma co ghe trong -> chuyen ve AVAILABLE
    if (_eStatus == FLIGHT_FULL && _seatList.countFreeSeats() > 0)
    {
        _eStatus = FLIGHT_AVAILABLE;
    }

    return true;
}

// =============================== Display ===============================

void Flight::print() const
{
    // ======= Chuẩn bị dữ liệu =======
    std::vector<Ticket> vTickets = _listTickets.toVector();
    std::sort(vTickets.begin(), vTickets.end(),
              [](const Ticket& a, const Ticket& b)
              {
                  return a.getSeatNumber() < b.getSeatNumber();
              });

    std::vector<int> seats;
    seats.reserve(vTickets.size());
    for (auto &t : vTickets)
        seats.push_back(t.getSeatNumber());

    const int SEATS_PER_LINE = 15;      // tối đa 15 ghế mỗi dòng
    size_t n = seats.size();
    size_t idx = 0;

    // ======= In dòng đầu (có đầy đủ cột) =======
    int color = getStatusColor(_eStatus);
    setTextColor(color);

    cout << left
         << setw(8)  << _strFlightId
         << setw(8)  << _strPlaneId
         << setw(12) << _strDate
         << setw(8)  << _strDepartTime
         << setw(20) << _strDestination
         << setw(15) << getStatusString();

    // In chunk đầu tiên của VeDaDat
    if (n == 0)
    {
        cout << "-\n";
        setTextColor(COLOR_DEFAULT);
        return;
    }

    // dòng đầu chỉ in 1 chunk
    for (size_t i = 0; i < n && i < SEATS_PER_LINE; ++i)
    {
        cout << seats[i];
        if (i + 1 < n && i + 1 < SEATS_PER_LINE)
            cout << ", ";
    }
    cout << "\n";
    idx += SEATS_PER_LINE;

    // ======= Các dòng tiếp theo (thụt cột) =======
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



void Flight::printDetail() const
{
    cout << "========================================\n";
    cout << " CHI TIET CHUYEN BAY: " << _strFlightId << "\n";
    cout << " May bay    : " << _strPlaneId       << "\n";
    cout << " Ngay di    : " << _strDate          << "\n";
    cout << " Gio di     : " << _strDepartTime    << "\n";   
    cout << " Diem den   : " << _strDestination   << "\n";
    cout << " Trang thai : " << getStatusString() << "\n";
    cout << " Tong ghe   : " << getTotalSeatCount()   << "\n";
    cout << " Ghe trong  : " << getFreeSeatCount()    << "\n";
    cout << " Da dat     : " << getBookedSeatCount()  << "\n";
    cout << "----------------------------------------\n";

    _seatList.printSeatMap();

    cout << "\n(Thong tin chi tiet hanh khach se duoc thong ke o chuc nang rieng.)\n";
    cout << "========================================\n";
}

