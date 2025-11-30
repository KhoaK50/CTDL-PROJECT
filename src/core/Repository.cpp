#include "core/Repository.h"
#include "core/Config.h"
#include "utils/Utils.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <cstdio>
#include <iomanip>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace fs = std::filesystem;

// ================== HAM HO TRO CUC BO ==================

// 0,1,2,3 -> enum FlightStatus
static FlightStatus intToFlightStatus(int iStatus)
{
    switch (iStatus)
    {
    case 0:
        return FLIGHT_CANCELLED;
    case 1:
        return FLIGHT_AVAILABLE;
    case 2:
        return FLIGHT_FULL;
    case 3:
        return FLIGHT_COMPLETED;
    default:
        return FLIGHT_AVAILABLE;
    }
}

// enum -> so nguyen de luu file
static int flightStatusToInt(FlightStatus flightStatus)
{
    switch (flightStatus)
    {
    case FLIGHT_CANCELLED:
        return 0;
    case FLIGHT_AVAILABLE:
        return 1;
    case FLIGHT_FULL:
        return 2;
    case FLIGHT_COMPLETED:
        return 3;
    default:
        return 1;
    }
}

// Ma ve = <MaChuyenBay>-<SoGhe>
static string buildTicketId(const string& strFlightId,
                            int iSeatNumber)
{
    return strFlightId + "-" + std::to_string(iSeatNumber);
}

// Duong dan day du toi 1 file ve, tu MaChuyenBay + SoGhe
static string buildTicketFilePath(const string& strFlightId,
                                  int iSeatNumber)
{
    string strTicketId = buildTicketId(strFlightId, iSeatNumber);
    return string(PATH_TICKET_DIR) + strTicketId + ".txt";
}

// Sap xep danh sach chuyen bay chi de hien thi cho gon,
// KHONG he tu dong thay doi trang thai.
// - Nhom 1: CHUA hoan tat (CON VE / HET VE / HUY) -> nam tren
// - Nhom 2: HOAN TAT -> nam cuoi
// - Trong tung nhom: sort theo ngay (chuoi dd/mm/yyyy) roi den gio
static void sortFlightsForDisplay(std::vector<Flight>& vFlights)
{
    std::sort(
        vFlights.begin(),
        vFlights.end(),
        [](const Flight& flightA, const Flight& flightB)
        {
            bool bACompleted = (flightA.getStatus() == FLIGHT_COMPLETED);
            bool bBCompleted = (flightB.getStatus() == FLIGHT_COMPLETED);

            if (bACompleted != bBCompleted)
            {
                // Chua hoan tat nam tren, hoan tat nam duoi
                return (!bACompleted && bBCompleted);
            }

            if (flightA.getDate() != flightB.getDate())
            {
                return (flightA.getDate() < flightB.getDate());
            }

            if (flightA.getDepartTime() != flightB.getDepartTime())
            {
                return (flightA.getDepartTime() < flightB.getDepartTime());
            }

            return (flightA.getFlightId() < flightB.getFlightId());
        });
}

// ======================= CONSTRUCTOR =======================

Repository::Repository()
{
    loadAllData();
}

// ======================= LOAD / SAVE TONG =======================

bool Repository::loadAllData()
{
    bool bOk = true;

    if (!loadAirplanes())
    {
        cout << ">> Canh bao: Khong load duoc MayBay.txt\n";
        bOk = false;
    }

    if (!loadFlights())
    {
        cout << ">> Canh bao: Khong load duoc ChuyenBay.txt\n";
        bOk = false;
    }

    if (!loadCustomers())
    {
        cout << ">> Canh bao: Khong load duoc KhachHang.txt\n";
        bOk = false;
    }

    if (!loadAdmins())
    {
        cout << ">> Canh bao: Khong load duoc Admin.txt\n";
        bOk = false;
    }

    if (!loadTickets())
    {
        cout << ">> Thong tin: Khong tim thay hoac khong load duoc cac file ve trong thu muc tickets.\n";
    }

    if (!loadWaitingBookings())
    {
        cout << ">> Thong tin: Hien chua co danh sach khach dang cho trong VeDangCho.txt.\n";
    }

    // CHI sap xep de hien thi, KHONG tu dong doi trang thai
    if (!_vFlights.empty())
    {
        sortFlightsForDisplay(_vFlights);
    }

    return bOk;
}

bool Repository::saveAllData() const
{
    bool bOk = true;

    if (!saveAirplanes())
    {
        cout << ">> Canh bao: Khong luu duoc MayBay.txt\n";
        bOk = false;
    }

    if (!saveFlights())
    {
        cout << ">> Canh bao: Khong luu duoc ChuyenBay.txt\n";
        bOk = false;
    }

    if (!saveCustomers())
    {
        cout << ">> Canh bao: Khong luu duoc KhachHang.txt\n";
        bOk = false;
    }

    if (!saveAdmins())
    {
        cout << ">> Canh bao: Khong luu duoc Admin.txt\n";
        bOk = false;
    }

    // Ve: duoc ghi / xoa ngay tai thoi diem dat / huy / UNDO
    return bOk;
}

// ======================= LOAD MAYBAY =======================

bool Repository::loadAirplanes()
{
    _vAirplanes.clear();

    std::ifstream fin(PATH_MAYBAY);
    if (!fin.is_open())
    {
        return false;
    }

    string strPlaneId;
    int    iSeatCount = 0;

    while (fin >> strPlaneId >> iSeatCount)
    {
        Airplane airplane(strPlaneId, iSeatCount);
        _vAirplanes.push_back(airplane);
    }

    fin.close();
    return true;
}

bool Repository::saveAirplanes() const
{
    std::ofstream fout(PATH_MAYBAY);
    if (!fout.is_open())
    {
        return false;
    }

    for (const Airplane& airplane : _vAirplanes)
    {
        fout << airplane.getPlaneId()   << " "
             << airplane.getSeatCount() << "\n";
    }

    fout.close();
    return true;
}

// ======================= LOAD CHUYENBAY =======================

bool Repository::loadFlights()
{
    _vFlights.clear();

    std::ifstream fin(PATH_CHUYENBAY);
    if (!fin.is_open())
    {
        return false;
    }

    string strLine;

    while (std::getline(fin, strLine))
    {
        if (strLine.empty())
        {
            continue;
        }

        std::stringstream stringStream(strLine);

        string strFlightId;
        string strPlaneId;
        string strDate;
        string strTime;         // Gio bay
        string strDestination;
        int    iStatusInt  = 0;
        int    iTotalSeats = 0;

        stringStream >> strFlightId
                     >> strPlaneId
                     >> strDate
                     >> strTime
                     >> strDestination
                     >> iStatusInt
                     >> iTotalSeats;

        FlightStatus flightStatus = intToFlightStatus(iStatusInt);

        Flight flight(
            strFlightId,
            strPlaneId,
            strDate,
            strDestination,
            flightStatus,
            iTotalSeats
        );

        flight.setDepartTime(strTime);    // Gan gio

        _vFlights.push_back(flight);
    }

    fin.close();
    return true;
}

bool Repository::saveFlights() const
{
    std::ofstream fout(PATH_CHUYENBAY);
    if (!fout.is_open())
    {
        return false;
    }

    for (const Flight& flight : _vFlights)
    {
        int iStatusInt = flightStatusToInt(flight.getStatus());

        fout << flight.getFlightId()       << " "
             << flight.getPlaneId()        << " "
             << flight.getDate()           << " "
             << flight.getDepartTime()     << " "
             << flight.getDestination()    << " "
             << iStatusInt                 << " "
             << flight.getTotalSeatCount() << "\n";
    }

    fout.close();
    return true;
}

// ======================= LOAD KHACHHANG =======================

bool Repository::loadCustomers()
{
    _vCustomers.clear();

    std::ifstream fin(PATH_KHACHHANG);
    if (!fin.is_open())
    {
        return false;
    }

    int    iIndex      = 0;
    string strIdNumber;
    string strNameRest;

    while (true)
    {
        if (!(fin >> iIndex >> strIdNumber))
        {
            break;
        }

        std::getline(fin, strNameRest);

        if (!strNameRest.empty() && strNameRest[0] == ' ')
        {
            strNameRest.erase(0, 1);
        }

        Customer customer(iIndex, strIdNumber, strNameRest);
        _vCustomers.push_back(customer);
    }

    fin.close();
    return true;
}

bool Repository::saveCustomers() const
{
    std::ofstream fout(PATH_KHACHHANG);
    if (!fout.is_open())
    {
        return false;
    }

    for (const Customer& customer : _vCustomers)
    {
        fout << customer.getIndex()    << " "
             << customer.getIdNumber() << " "
             << customer.getFullName() << "\n";
    }

    fout.close();
    return true;
}

// ======================= LOAD ADMIN =======================

bool Repository::loadAdmins()
{
    _vAdmins.clear();

    std::ifstream fin(PATH_ADMIN);
    if (!fin.is_open())
    {
        return false;
    }

    string strUserName;
    string strPassword;

    while (fin >> strUserName >> strPassword)
    {
        AdminAccount adminAccount(strUserName, strPassword);
        _vAdmins.push_back(adminAccount);
    }

    fin.close();
    return true;
}

bool Repository::saveAdmins() const
{
    std::ofstream fout(PATH_ADMIN);
    if (!fout.is_open())
    {
        return false;
    }

    for (const AdminAccount& adminAccount : _vAdmins)
    {
        fout << adminAccount.getUserName() << " "
             << adminAccount.getPassword() << "\n";
    }

    fout.close();
    return true;
}

// ======================= LOAD TICKETS =======================
//
// Moi file ve (PATH_TICKET_DIR + MaCB-SoGhe.txt):
//   Dong 1: MaChuyenBay
//   Dong 2: CMND/CCCD
//   Dong 3: Ho ten
//   Dong 4: So ghe
// =========================================================

bool Repository::loadTickets()
{
    if (!fs::exists(PATH_TICKET_DIR))
    {
        return false;
    }

    for (const auto& directoryEntry : fs::directory_iterator(PATH_TICKET_DIR))
    {
        if (!directoryEntry.is_regular_file())
        {
            continue;
        }

        std::ifstream fin(directoryEntry.path().string());
        if (!fin.is_open())
        {
            continue;
        }

        string strFlightId;
        string strCustomerId;
        string strCustomerName;
        string strSeatLine;

        if (!std::getline(fin, strFlightId))
        {
            continue;
        }
        if (!std::getline(fin, strCustomerId))
        {
            continue;
        }
        if (!std::getline(fin, strCustomerName))
        {
            continue;
        }
        if (!std::getline(fin, strSeatLine))
        {
            continue;
        }

        int iSeatNumber = std::stoi(strSeatLine);

        Flight* pFlight = findFlightById(strFlightId);
        if (pFlight == nullptr)
        {
            continue;
        }

        // Su dung bookTicket de khoi phuc seatList + danh sach ve
        pFlight->bookTicket(strCustomerId, strCustomerName, iSeatNumber);

        fin.close();
    }

    return true;
}

// ================== LOAD DANH SACH BOOKING DANG CHO ==================
//
// VeDangCho.txt
//   Moi dong: MaCB|CMND/CCCD|HoTen|SoGhe|ThoiGianYeuCau
// --------------------------------------------------------------------

bool Repository::loadWaitingBookings()
{
    // Clear queue: dung deQueue lien tuc den khi rong
    while (!_qWaitingBookings.isEmpty())
    {
        _qWaitingBookings.deQueue();
    }

    std::ifstream fin(PATH_VE_DANG_CHO);
    if (!fin.is_open())
    {
        // Chua co file -> coi nhu khong co booking nao
        return false;
    }

    string strLine;
    while (std::getline(fin, strLine))
    {
        if (strLine.empty())
        {
            continue;
        }

        std::stringstream stringStream(strLine);
        std::vector<string> vTokens;
        string strField;

        while (std::getline(stringStream, strField, '|'))
        {
            vTokens.push_back(strField);
        }

        if (vTokens.size() < 5)
        {
            // Sai dinh dang -> bo qua
            continue;
        }

        string strFlightId    = vTokens[0];
        string strIdNumber    = vTokens[1];
        string strFullName    = vTokens[2];
        int    iSeatNumber    = std::stoi(vTokens[3]);
        string strRequestTime = vTokens[4];

        // Tim / tao customer tu idNumber
        Customer* pExistingCustomer = nullptr;
        for (auto& customer : _vCustomers)
        {
            if (customer.getIdNumber() == strIdNumber)
            {
                pExistingCustomer = &customer;
                break;
            }
        }

        if (pExistingCustomer == nullptr)
        {
            int iNewIndex = static_cast<int>(_vCustomers.size()) + 1;
            Customer newCustomer(iNewIndex, strIdNumber, strFullName);
            _vCustomers.push_back(newCustomer);
            pExistingCustomer = &_vCustomers.back();
        }
        else if (pExistingCustomer->getFullName() != strFullName)
        {
            pExistingCustomer->setFullName(strFullName);
        }

        BookingRequest bookingRequest;
        bookingRequest.CustomerInfo = *pExistingCustomer;
        bookingRequest.FlightId     = strFlightId;
        bookingRequest.SeatNumber   = iSeatNumber;
        bookingRequest.RequestTime  = strRequestTime;

        _qWaitingBookings.enQueue(bookingRequest);
    }

    return true;
}

// Append 1 booking vao VeDangCho.txt
bool Repository::appendWaitingBookingToFile(const BookingRequest& bookingRequest) const
{
    std::ofstream fout(PATH_VE_DANG_CHO, std::ios::app);
    if (!fout.is_open())
    {
        return false;
    }

    fout << bookingRequest.FlightId                      << "|"
         << bookingRequest.CustomerInfo.getIdNumber()    << "|"
         << bookingRequest.CustomerInfo.getFullName()    << "|"
         << bookingRequest.SeatNumber                    << "|"
         << bookingRequest.RequestTime                   << "\n";

    return true;
}

// Xoa dong dau tien trong VeDangCho.txt (sau khi da deQueue o trong RAM)
bool Repository::popWaitingBookingFromFile() const
{
    std::ifstream fin(PATH_VE_DANG_CHO);
    if (!fin.is_open())
    {
        return false;
    }

    std::vector<string> vLines;
    string strLine;

    while (std::getline(fin, strLine))
    {
        if (!strLine.empty())
        {
            vLines.push_back(strLine);
        }
    }

    fin.close();

    if (vLines.empty())
    {
        return true;
    }

    // Xoa dong dau (booking cu nhat)
    vLines.erase(vLines.begin());

    std::ofstream fout(PATH_VE_DANG_CHO);
    if (!fout.is_open())
    {
        return false;
    }

    for (const auto& strContentLine : vLines)
    {
        fout << strContentLine << "\n";
    }

    return true;
}

// Log ve dat thanh cong vao VeThanhCong.txt
bool Repository::appendSuccessfulTicketToFile(const Ticket& ticket,
                                              const std::string& strRequestTime) const
{
    std::ofstream fout(PATH_VE_THANH_CONG, std::ios::app);
    if (!fout.is_open())
    {
        return false;
    }

    fout << ticket.getTicketId()     << "|"
         << ticket.getFlightId()     << "|"
         << ticket.getCustomerId()   << "|"
         << ticket.getCustomerName() << "|"
         << ticket.getSeatNumber()   << "|"
         << strRequestTime           << "|"
         << getCurrentTimeString()   << "\n";  // Thoi gian xu ly that su

    return true;
}

// ================== GHI / XOA 1 FILE VE RIENG LE ==================

bool Repository::writeTicketFile(const Ticket& ticket) const
{
    // Dam bao thu muc ton tai
    fs::create_directories(PATH_TICKET_DIR);

    string strPath = buildTicketFilePath(
        ticket.getFlightId(),
        ticket.getSeatNumber()
    );

    std::ofstream fout(strPath);
    if (!fout.is_open())
    {
        return false;
    }

    // 4 dong noi dung file ve
    fout << ticket.getFlightId()     << "\n";
    fout << ticket.getCustomerId()   << "\n";
    fout << ticket.getCustomerName() << "\n";
    fout << ticket.getSeatNumber()   << "\n";

    fout.close();
    return true;
}

bool Repository::deleteTicketFile(const string& strFlightId,
                                  int iSeatNumber) const
{
    string strPath = buildTicketFilePath(strFlightId, iSeatNumber);
    int iResult    = std::remove(strPath.c_str());
    return (iResult == 0);
}

// ======================= LOGIN ADMIN =======================

bool Repository::checkAdminLogin(const string& strUserName,
                                 const string& strPassword) const
{
    for (const AdminAccount& adminAccount : _vAdmins)
    {
        if (adminAccount.getUserName() == strUserName &&
            adminAccount.getPassword() == strPassword)
        {
            return true;
        }
    }

    return false;
}

// ======================= IN DANH SACH CHUYEN BAY =======================

void Repository::printAllFlights() const
{
    setTextColor(COLOR_TITLE);
    cout << "\n===== DANH SACH CHUYEN BAY =====\n\n";
    setTextColor(COLOR_HEADER);

    // Do rong cot: nho dung giong trong Flight::print()
    cout << std::left
         << std::setw(8)  << "MaCB"
         << std::setw(8)  << "MayBay"
         << std::setw(12) << "NgayBay"
         << std::setw(8)  << "GioBay"
         << std::setw(20) << "DiemDen"
         << std::setw(15) << "TrangThai"
         << std::setw(20) << "VeDaDat"
         << "\n";

    cout << std::string(130, '-') << "\n";

    setTextColor(COLOR_DEFAULT);

    for (const Flight& flight : _vFlights)
    {
        flight.print();
    }
}

// ======================= HANG DOI BOOKING =======================

void Repository::printWaitingBookings()
{
    setTextColor(COLOR_TITLE);
    cout << "\n===== DANH SACH YEU CAU DAT VE DANG CHO =====\n\n";
    setTextColor(COLOR_DEFAULT);

    if (_qWaitingBookings.isEmpty())
    {
        cout << "(Hien khong co yeu cau nao trong hang doi.)\n";
        return;
    }

    // Header
    setTextColor(COLOR_HEADER);
    cout << std::left
         << std::setw(5)  << "STT"
         << std::setw(12) << "MaCB"
         << std::setw(15) << "CMND/CCCD"
         << std::setw(30) << "Ho ten"
         << std::setw(8)  << "Ghe"
         << std::setw(20) << "Thoi gian gui"
         << "\n";

    cout << std::string(90, '-') << "\n";
    setTextColor(COLOR_DEFAULT);

    // Duyet thuc su tren _qWaitingBookings nhung luu tam roi khoi phuc
    Queue<BookingRequest> qBackup;
    int iIndex = 1;

    while (!_qWaitingBookings.isEmpty())
    {
        BookingRequest bookingRequest = _qWaitingBookings.deQueue();

        cout << std::left
             << std::setw(5)  << iIndex++
             << std::setw(12) << bookingRequest.FlightId
             << std::setw(15) << bookingRequest.CustomerInfo.getIdNumber()
             << std::setw(30) << bookingRequest.CustomerInfo.getFullName()
             << std::setw(8)  << bookingRequest.SeatNumber
             << std::setw(20) << bookingRequest.RequestTime
             << "\n";

        qBackup.enQueue(bookingRequest);
    }

    // Khoi phuc nguyen trang queue
    while (!qBackup.isEmpty())
    {
        _qWaitingBookings.enQueue(qBackup.deQueue());
    }
}

// ======================= TIM CHUYEN BAY =======================

Flight* Repository::findFlightById(const std::string& strFlightId)
{
    std::string strIdUpper = toUpper(strFlightId);

    for (Flight& flight : _vFlights)
    {
        if (toUpper(flight.getFlightId()) == strIdUpper)
        {
            return &flight;
        }
    }

    return nullptr;
}

const Flight* Repository::findFlightById(const std::string& strFlightId) const
{
    std::string strIdUpper = toUpper(strFlightId);

    for (const Flight& flight : _vFlights)
    {
        if (toUpper(flight.getFlightId()) == strIdUpper)
        {
            return &flight;
        }
    }

    return nullptr;
}

// ================== KIEM TRA THOI GIAN DAT VE ==================

bool Repository::isBookingTimeAllowed(const Flight& flight) const
{
    std::string strDate = flight.getDate();

    if (!isValidDate(strDate))
    {
        return false;
    }

    int iDiffDays = getDaysFromTodayToDate(strDate);

    // CHI cho phep dat khi con tu 30 ngay tro len truoc ngay khoi hanh
    if (iDiffDays < 30)
    {
        // Chua du 1 thang
        return false;
    }

    return true;
}

// ============ NGUOI DUNG NGOAI HE THONG: DANG KY DAT VE ============

bool Repository::userRegisterBooking(const std::string& strFlightId,
                                     const std::string& strIdNumber,
                                     const std::string& strFullName,
                                     int iSeatNumber)
{
    // 1. Tim chuyen bay (khong phan biet hoa thuong)
    Flight*      pFlight          = nullptr;
    std::string  strFlightIdUpper = toUpper(strFlightId);

    for (auto& flight : _vFlights)
    {
        if (toUpper(flight.getFlightId()) == strFlightIdUpper)
        {
            pFlight = &flight;
            break;
        }
    }

    if (pFlight == nullptr)
    {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: Khong tim thay chuyen bay co ma: " << strFlightId << "\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    // 2. Kiem tra trang thai chuyen bay
    if (pFlight->isCancelled() ||
        pFlight->isFull() ||
        pFlight->getStatus() == FLIGHT_COMPLETED)
    {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: Trang thai chuyen bay khong cho phep dat ve.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    // 3. Kiem tra rang buoc thoi gian (>= 30 ngay)
    if (!isBookingTimeAllowed(*pFlight))
    {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: Chi duoc dat ve truoc ngay khoi hanh it nhat 1 thang.\n";
        cout << "   Ngay khoi hanh: " << pFlight->getDate() << "\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    // 4. Kiem tra so ghe thuoc pham vi chuyen bay
    int iTotalSeats = pFlight->getTotalSeatCount();
    if (iSeatNumber < 1 || iSeatNumber > iTotalSeats)
    {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: So ghe khong hop le cho chuyen bay nay (1.." << iTotalSeats << ").\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    // 5. Kiem tra CMND/CCCD da ton tai chua
    Customer* pExistingCustomer = nullptr;
    for (auto& customer : _vCustomers)
    {
        if (customer.getIdNumber() == strIdNumber)
        {
            pExistingCustomer = &customer;
            break;
        }
    }

    if (pExistingCustomer != nullptr)
    {
        // Khong cho dung lai cung CMND/CCCD
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: CMND/CCCD nay da ton tai trong he thong!\n";
        cout << "   Moi nhap thong tin khach di cung ma co CMND/CCCD rieng.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    // 6. Them Customer moi vao danh sach khach hang (trong RAM)
    int iNewIndex = static_cast<int>(_vCustomers.size()) + 1;
    Customer newCustomer(iNewIndex, strIdNumber, strFullName);
    _vCustomers.push_back(newCustomer);
    pExistingCustomer = &_vCustomers.back();

    // 7. Dong goi BookingRequest va dua vao Queue
    BookingRequest bookingRequest;
    bookingRequest.CustomerInfo = *pExistingCustomer;
    bookingRequest.FlightId     = pFlight->getFlightId();  // Ma chuan trong he thong
    bookingRequest.SeatNumber   = iSeatNumber;
    bookingRequest.RequestTime  = getCurrentTimeString();

    _qWaitingBookings.enQueue(bookingRequest);

    // Ghi vao VeDangCho.txt de luu vet
    appendWaitingBookingToFile(bookingRequest);

    setTextColor(COLOR_SUCCESS);
    cout << ">> Da ghi nhan yeu cau dat ve vao hang doi.\n";
    setTextColor(COLOR_DEFAULT);

    return true;
}

// ====================== ADMIN XU LY DAT VE ======================

bool Repository::adminProcessNextBooking()
{
    if (_qWaitingBookings.isEmpty())
    {
        setTextColor(COLOR_WARNING);
        cout << ">> Hang doi dat ve dang rong.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    // Lay booking dau tien trong hang doi
    BookingRequest bookingRequest = _qWaitingBookings.deQueue();
    popWaitingBookingFromFile();   // Xoa dong dau trong file VeDangCho.txt

    // LUU VAO STACK UNDO (booking vua duoc duyet)
    _stUndoRequests.push(bookingRequest);

    // 1. Tim chuyen bay
    Flight* pFlight = findFlightById(bookingRequest.FlightId);
    if (pFlight == nullptr)
    {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: Khong tim thay chuyen bay "
             << bookingRequest.FlightId << ". Yeu cau bi huy.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    // 2. Kiem tra thoi gian (>= 30 ngay)
    if (!isBookingTimeAllowed(*pFlight))
    {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: Yeu cau dat ve cho chuyen "
             << pFlight->getFlightId()
             << " khong con hop le ve thoi gian (chi nhan ve it nhat 1 thang truoc khoi hanh).\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    // 3. Dat ghe
    bool bOkBooking = pFlight->bookTicket(
        bookingRequest.CustomerInfo.getIdNumber(),
        bookingRequest.CustomerInfo.getFullName(),
        bookingRequest.SeatNumber
    );

    if (!bOkBooking)
    {
        setTextColor(COLOR_ERROR);
        cout << ">> Khong the dat ve (ghe khong hop le / da bi dat / trang thai khong cho phep).\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    // 4. Tao ticket
    Ticket ticket(
        pFlight->getFlightId(),
        bookingRequest.CustomerInfo.getIdNumber(),
        bookingRequest.CustomerInfo.getFullName(),
        bookingRequest.SeatNumber
    );

    ticket.makeTicketId();

    // 5. Ghi file ve (MaCB-Seat.txt)
    if (!writeTicketFile(ticket))
    {
        setTextColor(COLOR_WARNING);
        cout << ">> Canh bao: Khong the ghi file ve cho luot dat nay.\n";
        setTextColor(COLOR_DEFAULT);
    }

    // 6. Ghi log ve thanh cong
    appendSuccessfulTicketToFile(ticket, bookingRequest.RequestTime);

    // 6.5. Luu lai toan bo danh sach khach ngay sau khi duyet ve
    saveCustomers();

    // 7. Thong bao
    setTextColor(COLOR_SUCCESS);
    cout << ">> DAT VE THANH CONG!\n"
         << "   Khach: " << bookingRequest.CustomerInfo.getFullName() << "\n"
         << "   CMND/CCCD: " << bookingRequest.CustomerInfo.getIdNumber() << "\n"
         << "   Chuyen bay: " << pFlight->getFlightId() << "\n"
         << "   So ghe: " << bookingRequest.SeatNumber << "\n";
    setTextColor(COLOR_DEFAULT);

    return true;
}

// ====================== TU CHOI BOOKING DAU HANG DOI ======================

bool Repository::rejectNextBooking()
{
    if (_qWaitingBookings.isEmpty())
    {
        setTextColor(COLOR_WARNING);
        cout << ">> Hang doi dat ve dang rong.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    // Lay booking dau va xoa khoi file VeDangCho.txt luon
    BookingRequest bookingRequest = _qWaitingBookings.deQueue();
    popWaitingBookingFromFile();

    setTextColor(COLOR_WARNING);
    cout << ">> Da BO QUA yeu cau dat ve cua khach: "
         << bookingRequest.CustomerInfo.getFullName()
         << " (Chuyen " << bookingRequest.FlightId
         << ", ghe " << bookingRequest.SeatNumber << ").\n";
    setTextColor(COLOR_DEFAULT);

    return true;
}

// ====================== ADMIN HUY VE TRUC TIEP ======================

bool Repository::adminCancelTicket(const string& strFlightId,
                                   int iSeatNumber)
{
    Flight* pFlight = findFlightById(strFlightId);
    if (pFlight == nullptr)
    {
        cout << ">> Loi: Khong tim thay chuyen bay.\n";
        return false;
    }

    bool bOkCancel = pFlight->cancelTicketBySeat(iSeatNumber);
    if (!bOkCancel)
    {
        cout << ">> Khong the huy ve (kiem tra so ghe / trang thai).\n";
        return false;
    }

    deleteTicketFile(strFlightId, iSeatNumber);

    cout << ">> Da huy ve ghe " << iSeatNumber
         << " tren chuyen " << strFlightId << ".\n";

    return true;
}

// ====================== UNDO LAN BOOKING GAN NHAT ======================

bool Repository::undoLastBooking()
{
    if (_stUndoRequests.isEmpty())
    {
        cout << ">> Khong co giao dich dat ve nao de UNDO.\n";
        return false;
    }

    // Lay booking cuoi cung da duoc duyet
    BookingRequest lastRequest = _stUndoRequests.pop();

    // Tim chuyen bay
    Flight* pFlight = findFlightById(lastRequest.FlightId);
    if (pFlight == nullptr)
    {
        cout << ">> Loi: Khong tim thay chuyen bay khi UNDO.\n";
        return false;
    }

    // Huy ghe da dat
    bool bOkCancel = pFlight->cancelTicketBySeat(lastRequest.SeatNumber);
    if (!bOkCancel)
    {
        cout << ">> UNDO that bai (khong the huy ve).\n";
        return false;
    }

    // Xoa file ve tuong ung
    deleteTicketFile(lastRequest.FlightId, lastRequest.SeatNumber);

    // Dua booking tro lai hang doi
    _qWaitingBookings.enQueue(lastRequest);

    // Ghi lai vao VeDangCho.txt
    appendWaitingBookingToFile(lastRequest);

    cout << ">> UNDO thanh cong! Da dua lai yeu cau ve hang doi.\n";

    return true;
}

// ====================== THONG KE ======================

void Repository::statisticsPassengersOfFlight(const string& strFlightId) const
{
    const Flight* pFlight = findFlightById(strFlightId);
    if (pFlight == nullptr)
    {
        cout << ">> Khong tim thay chuyen bay.\n";
        return;
    }

    if (!fs::exists(PATH_TICKET_DIR))
    {
        cout << ">> Chua co thu muc tickets hoac chua co ve nao.\n";
        return;
    }

    struct Row
    {
        int    Index;
        string IdNumber;
        string FullName;
        int    Seat;
    };

    std::vector<Row> vRows;

    for (const auto& directoryEntry : fs::directory_iterator(PATH_TICKET_DIR))
    {
        if (!directoryEntry.is_regular_file())
        {
            continue;
        }

        std::ifstream fin(directoryEntry.path().string());
        if (!fin.is_open())
        {
            continue;
        }

        string strFileFlightId;
        string strIdNumber;
        string strFullName;
        string strSeatLine;

        if (!std::getline(fin, strFileFlightId))
        {
            continue;
        }
        if (!std::getline(fin, strIdNumber))
        {
            continue;
        }
        if (!std::getline(fin, strFullName))
        {
            continue;
        }
        if (!std::getline(fin, strSeatLine))
        {
            continue;
        }

        if (strFileFlightId != strFlightId)
        {
            continue;
        }

        int iSeat = std::stoi(strSeatLine);

        Row rowData;
        rowData.Index    = static_cast<int>(vRows.size()) + 1;
        rowData.IdNumber = strIdNumber;
        rowData.FullName = strFullName;
        rowData.Seat     = iSeat;

        vRows.push_back(rowData);
    }

    setTextColor(COLOR_TITLE);
    cout << "\n===== DANH SACH KHACH CUA CHUYEN " << strFlightId << " =====\n";
    setTextColor(COLOR_DEFAULT);

    cout << "Tong ghe   : " << pFlight->getTotalSeatCount()  << "\n";
    cout << "Da dat     : " << pFlight->getBookedSeatCount() << "\n";
    cout << "Con trong  : " << pFlight->getFreeSeatCount()   << "\n\n";

    if (vRows.empty())
    {
        cout << "Chua co khach nao tren chuyen bay nay.\n";
        return;
    }

    setTextColor(COLOR_HEADER);
    cout << std::left
         << std::setw(6)  << "STT"
         << std::setw(15) << "CMND/CCCD"
         << std::setw(30) << "Ho ten"
         << std::setw(8)  << "Ghe"
         << "\n";

    cout << std::string(60, '-') << "\n";
    setTextColor(COLOR_DEFAULT);

    for (const Row& rowData : vRows)
    {
        cout << std::left
             << std::setw(6)  << rowData.Index
             << std::setw(15) << rowData.IdNumber
             << std::setw(30) << rowData.FullName
             << std::setw(8)  << rowData.Seat
             << "\n";
    }
}

void Repository::statisticsFreeSeats(const string& strFlightId) const
{
    const Flight* pFlight = findFlightById(strFlightId);
    if (pFlight == nullptr)
    {
        cout << ">> Khong tim thay chuyen bay.\n";
        return;
    }

    setTextColor(COLOR_TITLE);
    cout << "\n===== SO DO GHE - CHUYEN " << strFlightId << " =====\n";
    setTextColor(COLOR_DEFAULT);

    pFlight->printDetail();
}

void Repository::statisticsPlaneUsage(const string& strPlaneId) const
{
    int iCount = 0;

    for (const Flight& flight : _vFlights)
    {
        if (flight.getPlaneId() == strPlaneId &&
            flight.getStatus() == FLIGHT_COMPLETED)
        {
            iCount++;
        }
    }

    setTextColor(COLOR_TITLE);
    cout << "\n===== THONG KE MAY BAY " << strPlaneId << " =====\n";
    setTextColor(COLOR_DEFAULT);

    cout << "May bay " << strPlaneId
         << " da thuc hien " << iCount
         << " chuyen bay (HOAN TAT).\n";
}

const Flight* Repository::getFlightConst(const std::string& strFlightId) const
{
    std::string strIdUpper = toUpper(strFlightId);

    for (const Flight& flight : _vFlights)
    {
        if (toUpper(flight.getFlightId()) == strIdUpper)
        {
            return &flight;
        }
    }

    return nullptr;
}
