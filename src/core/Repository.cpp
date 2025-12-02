#include "core/Repository.h"
#include "core/Config.h"
#include "utils/Utils.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <iomanip>
#include <algorithm>
#include <sys/stat.h> // Dùng để kiểm tra file/folder tồn tại
#include <dirent.h>   // Dùng để duyệt thư mục (thay cho filesystem)
#include <direct.h>   // Dùng cho _mkdir (trên Windows)

using std::cout;
using std::endl;
using std::string;
using std::vector;

// ================== HÀM HỖ TRỢ FILE SYSTEM (THỦ CÔNG) ==================

/************
* @Description Kiểm tra file hoặc folder có tồn tại không
*************/
static bool fs_exists(const std::string& path) {
    struct stat info;
    return (stat(path.c_str(), &info) == 0);
}

/************
* @Description Kiểm tra đây có phải là file thường không
*************/
static bool fs_is_regular_file(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) return false;
    return (info.st_mode & S_IFREG);
}

/************
* @Description Tạo thư mục nếu chưa tồn tại
*************/
static void fs_create_directories(const std::string& path) {
    if (fs_exists(path)) return;
    _mkdir(path.c_str()); // Hàm của Windows (direct.h)
}

// ================== HÀM HỖ TRỢ CỤC BỘ ==================

/************
* @Description Chuyển đổi int sang enum FlightStatus
*************/
static FlightStatus intToFlightStatus(int iStatus)
{
    switch (iStatus)
    {
    case 0: return FLIGHT_CANCELLED;
    case 1: return FLIGHT_AVAILABLE;
    case 2: return FLIGHT_FULL;
    case 3: return FLIGHT_COMPLETED;
    default: return FLIGHT_AVAILABLE;
    }
}

/************
* @Description Chuyển đổi enum FlightStatus sang int để lưu file
*************/
static int flightStatusToInt(FlightStatus flightStatus)
{
    switch (flightStatus)
    {
    case FLIGHT_CANCELLED: return 0;
    case FLIGHT_AVAILABLE: return 1;
    case FLIGHT_FULL:      return 2;
    case FLIGHT_COMPLETED: return 3;
    default:               return 1;
    }
}

/************
* @Description Tạo mã vé từ Mã chuyến + Số ghế
*************/
static string buildTicketId(const string& strFlightId, int iSeatNumber)
{
    return strFlightId + "-" + std::to_string(iSeatNumber);
}

/************
* @Description Tạo đường dẫn file vé
*************/
static string buildTicketFilePath(const string& strFlightId, int iSeatNumber)
{
    string strTicketId = buildTicketId(strFlightId, iSeatNumber);
    return string(PATH_TICKET_DIR) + strTicketId + ".txt";
}

/************
* @Description Sắp xếp danh sách chuyến bay để hiển thị
*************/
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

/************
* @Description Constructor mặc định
*************/
Repository::Repository()
{
    this->loadAllData();
}

// ======================= LOAD / SAVE TỔNG =======================

/************
* @Description Load toàn bộ dữ liệu
*************/
bool Repository::loadAllData()
{
    bool bOk = true;

    if (!this->loadAirplanes()) {
        cout << ">> Canh bao: Khong load duoc MayBay.txt\n";
        bOk = false;
    }

    if (!this->loadFlights()) {
        cout << ">> Canh bao: Khong load duoc ChuyenBay.txt\n";
        bOk = false;
    }

    if (!this->loadCustomers()) {
        cout << ">> Canh bao: Khong load duoc KhachHang.txt\n";
        bOk = false;
    }

    if (!this->loadAdmins()) {
        cout << ">> Canh bao: Khong load duoc Admin.txt\n";
        bOk = false;
    }

    if (!this->loadTickets()) {
        cout << ">> Thong tin: Khong tim thay hoac khong load duoc cac file ve trong thu muc tickets.\n";
    }

    if (!this->loadWaitingBookings()) {
        cout << ">> Thong tin: Hien chua co danh sach khach dang cho trong VeDangCho.txt.\n";
    }

    // CHI sap xep de hien thi, KHONG tu dong doi trang thai
    if (!this->_vFlights.empty()) {
        sortFlightsForDisplay(this->_vFlights);
    }

    return bOk;
}

/************
* @Description Lưu toàn bộ dữ liệu
*************/
bool Repository::saveAllData() const
{
    bool bOk = true;

    if (!this->saveAirplanes()) {
        cout << ">> Canh bao: Khong luu duoc MayBay.txt\n";
        bOk = false;
    }

    if (!this->saveFlights()) {
        cout << ">> Canh bao: Khong luu duoc ChuyenBay.txt\n";
        bOk = false;
    }

    if (!this->saveCustomers()) {
        cout << ">> Canh bao: Khong luu duoc KhachHang.txt\n";
        bOk = false;
    }

    if (!this->saveAdmins()) {
        cout << ">> Canh bao: Khong luu duoc Admin.txt\n";
        bOk = false;
    }

    return bOk;
}

// ======================= LOAD MAYBAY =======================

/************
* @Description Load danh sách máy bay
*************/
bool Repository::loadAirplanes()
{
    this->_vAirplanes.clear();
    std::ifstream fin(PATH_MAYBAY);
    if (!fin.is_open()) return false;

    string strPlaneId;
    int iSeatCount = 0;

    while (fin >> strPlaneId >> iSeatCount) {
        Airplane airplane(strPlaneId, iSeatCount);
        this->_vAirplanes.push_back(airplane);
    }
    fin.close();
    return true;
}

/************
* @Description Lưu danh sách máy bay
*************/
bool Repository::saveAirplanes() const
{
    std::ofstream fout(PATH_MAYBAY);
    if (!fout.is_open()) return false;

    for (const Airplane& airplane : this->_vAirplanes) {
        fout << airplane.getPlaneId()   << " "
             << airplane.getSeatCount() << "\n";
    }
    fout.close();
    return true;
}

// ======================= LOAD CHUYENBAY =======================

/************
* @Description Load danh sách chuyến bay
*************/
bool Repository::loadFlights()
{
    this->_vFlights.clear();
    std::ifstream fin(PATH_CHUYENBAY);
    if (!fin.is_open()) return false;

    string strLine;
    while (std::getline(fin, strLine)) {
        if (strLine.empty()) continue;

        std::stringstream stringStream(strLine);
        string strFlightId, strPlaneId, strDate, strTime, strDestination;
        int iStatusInt = 0, iTotalSeats = 0;

        stringStream >> strFlightId >> strPlaneId >> strDate >> strTime
                     >> strDestination >> iStatusInt >> iTotalSeats;

        FlightStatus flightStatus = intToFlightStatus(iStatusInt);
        Flight flight(strFlightId, strPlaneId, strDate, strDestination, flightStatus, iTotalSeats);
        flight.setDepartTime(strTime);

        this->_vFlights.push_back(flight);
    }
    fin.close();
    return true;
}

/************
* @Description Lưu danh sách chuyến bay
*************/
bool Repository::saveFlights() const
{
    std::ofstream fout(PATH_CHUYENBAY);
    if (!fout.is_open()) return false;

    for (const Flight& flight : this->_vFlights) {
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

/************
* @Description Load danh sách khách hàng
*************/
bool Repository::loadCustomers()
{
    this->_vCustomers.clear();
    std::ifstream fin(PATH_KHACHHANG);
    if (!fin.is_open()) return false;

    int iIndex = 0;
    string strIdNumber, strNameRest;

    while (true) {
        if (!(fin >> iIndex >> strIdNumber)) break;
        std::getline(fin, strNameRest);
        if (!strNameRest.empty() && strNameRest[0] == ' ') strNameRest.erase(0, 1);

        Customer customer(iIndex, strIdNumber, strNameRest);
        this->_vCustomers.push_back(customer);
    }
    fin.close();
    return true;
}

/************
* @Description Lưu danh sách khách hàng
*************/
bool Repository::saveCustomers() const
{
    std::ofstream fout(PATH_KHACHHANG);
    if (!fout.is_open()) return false;

    for (const Customer& customer : this->_vCustomers) {
        fout << customer.getIndex()    << " "
             << customer.getIdNumber() << " "
             << customer.getFullName() << "\n";
    }
    fout.close();
    return true;
}

// ======================= LOAD ADMIN =======================

/************
* @Description Load danh sách Admin
*************/
bool Repository::loadAdmins()
{
    this->_vAdmins.clear();
    std::ifstream fin(PATH_ADMIN);
    if (!fin.is_open()) return false;

    string strUserName, strPassword;
    while (fin >> strUserName >> strPassword) {
        AdminAccount adminAccount(strUserName, strPassword);
        this->_vAdmins.push_back(adminAccount);
    }
    fin.close();
    return true;
}

/************
* @Description Lưu danh sách Admin
*************/
bool Repository::saveAdmins() const
{
    std::ofstream fout(PATH_ADMIN);
    if (!fout.is_open()) return false;

    for (const AdminAccount& adminAccount : this->_vAdmins) {
        fout << adminAccount.getUserName() << " "
             << adminAccount.getPassword() << "\n";
    }
    fout.close();
    return true;
}

// ======================= LOAD TICKETS (SỬ DỤNG DIRENT.H THAY FILESYSTEM) =======================

/************
* @Description Load toàn bộ vé từ thư mục tickets/
*************/
bool Repository::loadTickets()
{
    if (!fs_exists(PATH_TICKET_DIR)) return false;

    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(PATH_TICKET_DIR)) != NULL) {
        // Duyệt từng file trong thư mục
        while ((ent = readdir(dir)) != NULL) {
            string fileName = ent->d_name;
            if (fileName == "." || fileName == "..") continue;

            string fullPath = string(PATH_TICKET_DIR) + fileName;

            std::ifstream fin(fullPath);
            if (!fin.is_open()) continue;

            string strFlightId, strCustomerId, strCustomerName, strSeatLine;
            if (!std::getline(fin, strFlightId)) continue;
            if (!std::getline(fin, strCustomerId)) continue;
            if (!std::getline(fin, strCustomerName)) continue;
            if (!std::getline(fin, strSeatLine)) continue;

            int iSeatNumber = std::stoi(strSeatLine);

            Flight* pFlight = this->findFlightById(strFlightId);
            if (pFlight == nullptr) continue;

            pFlight->bookTicket(strCustomerId, strCustomerName, iSeatNumber);
            fin.close();
        }
        closedir(dir);
    } else {
        return false;
    }
    return true;
}

// ================== LOAD DANH SACH BOOKING DANG CHO ==================

/************
* @Description Load danh sách Booking đang chờ xử lý
*************/
bool Repository::loadWaitingBookings()
{
    while (!this->_qWaitingBookings.isEmpty()) {
        this->_qWaitingBookings.deQueue();
    }

    std::ifstream fin(PATH_VE_DANG_CHO);
    if (!fin.is_open()) return false;

    string strLine;
    while (std::getline(fin, strLine)) {
        if (strLine.empty()) continue;

        std::stringstream stringStream(strLine);
        std::vector<string> vTokens;
        string strField;

        while (std::getline(stringStream, strField, '|')) {
            vTokens.push_back(strField);
        }

        if (vTokens.size() < 5) continue;

        string strFlightId    = vTokens[0];
        string strIdNumber    = vTokens[1];
        string strFullName    = vTokens[2];
        int    iSeatNumber    = std::stoi(vTokens[3]);
        string strRequestTime = vTokens[4];

        Customer* pExistingCustomer = nullptr;
        for (auto& customer : this->_vCustomers) {
            if (customer.getIdNumber() == strIdNumber) {
                pExistingCustomer = &customer;
                break;
            }
        }

        if (pExistingCustomer == nullptr) {
            int iNewIndex = static_cast<int>(this->_vCustomers.size()) + 1;
            Customer newCustomer(iNewIndex, strIdNumber, strFullName);
            this->_vCustomers.push_back(newCustomer);
            pExistingCustomer = &this->_vCustomers.back();
        } else if (pExistingCustomer->getFullName() != strFullName) {
            pExistingCustomer->setFullName(strFullName);
        }

        BookingRequest bookingRequest;
        bookingRequest.CustomerInfo = *pExistingCustomer;
        bookingRequest.FlightId     = strFlightId;
        bookingRequest.SeatNumber   = iSeatNumber;
        bookingRequest.RequestTime  = strRequestTime;

        this->_qWaitingBookings.enQueue(bookingRequest);
    }
    return true;
}

/************
* @Description Ghi Booking mới vào file VeDangCho.txt
*************/
bool Repository::appendWaitingBookingToFile(const BookingRequest& bookingRequest) const
{
    std::ofstream fout(PATH_VE_DANG_CHO, std::ios::app);
    if (!fout.is_open()) return false;

    fout << bookingRequest.FlightId                      << "|"
         << bookingRequest.CustomerInfo.getIdNumber()    << "|"
         << bookingRequest.CustomerInfo.getFullName()    << "|"
         << bookingRequest.SeatNumber                    << "|"
         << bookingRequest.RequestTime                   << "\n";
    return true;
}

/************
* @Description Xóa dòng đầu tiên của file VeDangCho.txt
*************/
bool Repository::popWaitingBookingFromFile() const
{
    std::ifstream fin(PATH_VE_DANG_CHO);
    if (!fin.is_open()) return false;

    std::vector<string> vLines;
    string strLine;
    while (std::getline(fin, strLine)) {
        if (!strLine.empty()) vLines.push_back(strLine);
    }
    fin.close();

    if (vLines.empty()) return true;
    vLines.erase(vLines.begin());

    std::ofstream fout(PATH_VE_DANG_CHO);
    if (!fout.is_open()) return false;
    for (const auto& strContentLine : vLines) {
        fout << strContentLine << "\n";
    }
    return true;
}

/************
* @Description Ghi log vé thành công vào file
*************/
bool Repository::appendSuccessfulTicketToFile(const Ticket& ticket, const std::string& strRequestTime) const
{
    std::ofstream fout(PATH_VE_THANH_CONG, std::ios::app);
    if (!fout.is_open()) return false;

    fout << ticket.getTicketId()     << "|"
         << ticket.getFlightId()     << "|"
         << ticket.getCustomerId()   << "|"
         << ticket.getCustomerName() << "|"
         << ticket.getSeatNumber()   << "|"
         << strRequestTime           << "|"
         << getCurrentTimeString()   << "\n";
    return true;
}

// ================== GHI / XOA 1 FILE VE RIENG LE ==================

/************
* @Description Tạo file vé riêng lẻ
*************/
bool Repository::writeTicketFile(const Ticket& ticket) const
{
    fs_create_directories(PATH_TICKET_DIR);

    string strPath = buildTicketFilePath(ticket.getFlightId(), ticket.getSeatNumber());
    std::ofstream fout(strPath);
    if (!fout.is_open()) return false;

    fout << ticket.getFlightId()     << "\n";
    fout << ticket.getCustomerId()   << "\n";
    fout << ticket.getCustomerName() << "\n";
    fout << ticket.getSeatNumber()   << "\n";
    fout.close();
    return true;
}

/************
* @Description Xóa file vé riêng lẻ
*************/
bool Repository::deleteTicketFile(const string& strFlightId, int iSeatNumber) const
{
    string strPath = buildTicketFilePath(strFlightId, iSeatNumber);
    int iResult    = std::remove(strPath.c_str());
    return (iResult == 0);
}

// ======================= LOGIN ADMIN =======================

/************
* @Description Kiểm tra đăng nhập Admin
*************/
bool Repository::checkAdminLogin(const string& strUserName, const string& strPassword) const
{
    for (const AdminAccount& adminAccount : this->_vAdmins) {
        if (adminAccount.getUserName() == strUserName &&
            adminAccount.getPassword() == strPassword) {
            return true;
        }
    }
    return false;
}

// ======================= IN DANH SACH CHUYEN BAY =======================

/************
* @Description In danh sách chuyến bay
*************/
void Repository::printAllFlights() const
{
    setTextColor(COLOR_TITLE);
    cout << "\n===== DANH SACH CHUYEN BAY =====\n\n";
    setTextColor(COLOR_HEADER);

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

    for (const Flight& flight : this->_vFlights) {
        flight.print();
    }
}

// ======================= HANG DOI BOOKING =======================

/************
* @Description In danh sách Booking đang chờ
*************/
void Repository::printWaitingBookings()
{
    setTextColor(COLOR_TITLE);
    cout << "\n===== DANH SACH YEU CAU DAT VE DANG CHO =====\n\n";
    setTextColor(COLOR_DEFAULT);

    if (this->_qWaitingBookings.isEmpty()) {
        cout << "(Hien khong co yeu cau nao trong hang doi.)\n";
        return;
    }

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

    Queue<BookingRequest> qBackup;
    int iIndex = 1;

    while (!this->_qWaitingBookings.isEmpty()) {
        BookingRequest bookingRequest = this->_qWaitingBookings.deQueue();

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

    while (!qBackup.isEmpty()) {
        this->_qWaitingBookings.enQueue(qBackup.deQueue());
    }
}

// ======================= TIM CHUYEN BAY =======================

/************
* @Description Tìm chuyến bay theo ID (non-const)
*************/
Flight* Repository::findFlightById(const std::string& strFlightId)
{
    std::string strIdUpper = toUpper(strFlightId);
    for (Flight& flight : this->_vFlights) {
        if (toUpper(flight.getFlightId()) == strIdUpper) return &flight;
    }
    return nullptr;
}

/************
* @Description Tìm chuyến bay theo ID (const)
*************/
const Flight* Repository::findFlightById(const std::string& strFlightId) const
{
    std::string strIdUpper = toUpper(strFlightId);
    for (const Flight& flight : this->_vFlights) {
        if (toUpper(flight.getFlightId()) == strIdUpper) return &flight;
    }
    return nullptr;
}

// ================== KIEM TRA THOI GIAN DAT VE ==================

/************
* @Description Kiểm tra thời gian đặt vé hợp lệ
*************/
bool Repository::isBookingTimeAllowed(const Flight& flight) const
{
    std::string strDate = flight.getDate();
    if (!isValidDate(strDate)) return false;

    int iDiffDays = getDaysFromTodayToDate(strDate);
    if (iDiffDays < 30) return false;
    return true;
}

// ============ NGUOI DUNG NGOAI HE THONG: DANG KY DAT VE ============

/************
* @Description Người dùng đăng ký đặt vé
*************/
bool Repository::userRegisterBooking(const std::string& strFlightId, const std::string& strIdNumber,
                                     const std::string& strFullName, int iSeatNumber)
{
    Flight* pFlight = this->findFlightById(strFlightId);

    if (pFlight == nullptr) {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: Khong tim thay chuyen bay co ma: " << strFlightId << "\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    if (pFlight->isCancelled() || pFlight->isFull() || pFlight->getStatus() == FLIGHT_COMPLETED) {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: Trang thai chuyen bay khong cho phep dat ve.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    if (!this->isBookingTimeAllowed(*pFlight)) {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: Chi duoc dat ve truoc ngay khoi hanh it nhat 1 thang.\n";
        cout << "   Ngay khoi hanh: " << pFlight->getDate() << "\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    int iTotalSeats = pFlight->getTotalSeatCount();
    if (iSeatNumber < 1 || iSeatNumber > iTotalSeats) {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: So ghe khong hop le cho chuyen bay nay (1.." << iTotalSeats << ").\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    Customer* pExistingCustomer = nullptr;
    for (auto& customer : this->_vCustomers) {
        if (customer.getIdNumber() == strIdNumber) {
            pExistingCustomer = &customer;
            break;
        }
    }

    if (pExistingCustomer != nullptr) {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: CMND/CCCD nay da ton tai trong he thong!\n";
        cout << "   Moi nhap thong tin khach di cung ma co CMND/CCCD rieng.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    int iNewIndex = static_cast<int>(this->_vCustomers.size()) + 1;
    Customer newCustomer(iNewIndex, strIdNumber, strFullName);
    this->_vCustomers.push_back(newCustomer);
    pExistingCustomer = &this->_vCustomers.back();

    BookingRequest bookingRequest;
    bookingRequest.CustomerInfo = *pExistingCustomer;
    bookingRequest.FlightId     = pFlight->getFlightId();
    bookingRequest.SeatNumber   = iSeatNumber;
    bookingRequest.RequestTime  = getCurrentTimeString();

    this->_qWaitingBookings.enQueue(bookingRequest);
    this->appendWaitingBookingToFile(bookingRequest);

    setTextColor(COLOR_SUCCESS);
    cout << ">> Da ghi nhan yeu cau dat ve vao hang doi.\n";
    setTextColor(COLOR_DEFAULT);

    return true;
}

// ====================== ADMIN XU LY DAT VE ======================

/************
* @Description Admin duyệt yêu cầu đặt vé
*************/
bool Repository::adminProcessNextBooking()
{
    if (this->_qWaitingBookings.isEmpty()) {
        setTextColor(COLOR_WARNING);
        cout << ">> Hang doi dat ve dang rong.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    BookingRequest bookingRequest = this->_qWaitingBookings.deQueue();
    this->popWaitingBookingFromFile();
    this->_stUndoRequests.push(bookingRequest);

    Flight* pFlight = this->findFlightById(bookingRequest.FlightId);
    if (pFlight == nullptr) {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: Khong tim thay chuyen bay " << bookingRequest.FlightId << ". Yeu cau bi huy.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    if (!this->isBookingTimeAllowed(*pFlight)) {
        setTextColor(COLOR_ERROR);
        cout << ">> Loi: Yeu cau dat ve cho chuyen " << pFlight->getFlightId()
             << " khong con hop le ve thoi gian.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    bool bOkBooking = pFlight->bookTicket(bookingRequest.CustomerInfo.getIdNumber(),
                                          bookingRequest.CustomerInfo.getFullName(),
                                          bookingRequest.SeatNumber);

    if (!bOkBooking) {
        setTextColor(COLOR_ERROR);
        cout << ">> Khong the dat ve (ghe khong hop le / da bi dat / trang thai khong cho phep).\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    Ticket ticket(pFlight->getFlightId(), bookingRequest.CustomerInfo.getIdNumber(),
                  bookingRequest.CustomerInfo.getFullName(), bookingRequest.SeatNumber);
    ticket.makeTicketId();

    if (!this->writeTicketFile(ticket)) {
        setTextColor(COLOR_WARNING);
        cout << ">> Canh bao: Khong the ghi file ve cho luot dat nay.\n";
        setTextColor(COLOR_DEFAULT);
    }

    this->appendSuccessfulTicketToFile(ticket, bookingRequest.RequestTime);
    this->saveCustomers();

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

/************
* @Description Từ chối yêu cầu đặt vé đầu tiên
*************/
bool Repository::rejectNextBooking()
{
    if (this->_qWaitingBookings.isEmpty()) {
        setTextColor(COLOR_WARNING);
        cout << ">> Hang doi dat ve dang rong.\n";
        setTextColor(COLOR_DEFAULT);
        return false;
    }

    BookingRequest bookingRequest = this->_qWaitingBookings.deQueue();
    this->popWaitingBookingFromFile();

    setTextColor(COLOR_WARNING);
    cout << ">> Da BO QUA yeu cau dat ve cua khach: " << bookingRequest.CustomerInfo.getFullName()
         << " (Chuyen " << bookingRequest.FlightId << ", ghe " << bookingRequest.SeatNumber << ").\n";
    setTextColor(COLOR_DEFAULT);

    return true;
}

// ====================== ADMIN HUY VE TRUC TIEP ======================

/************
* @Description Admin hủy vé trực tiếp
*************/
bool Repository::adminCancelTicket(const string& strFlightId, int iSeatNumber)
{
    Flight* pFlight = this->findFlightById(strFlightId);
    if (pFlight == nullptr) {
        cout << ">> Loi: Khong tim thay chuyen bay.\n";
        return false;
    }

    bool bOkCancel = pFlight->cancelTicketBySeat(iSeatNumber);
    if (!bOkCancel) {
        cout << ">> Khong the huy ve (kiem tra so ghe / trang thai).\n";
        return false;
    }

    this->deleteTicketFile(strFlightId, iSeatNumber);
    cout << ">> Da huy ve ghe " << iSeatNumber << " tren chuyen " << strFlightId << ".\n";
    return true;
}

// ====================== UNDO LAN BOOKING GAN NHAT ======================

/************
* @Description Hoàn tác booking gần nhất
*************/
bool Repository::undoLastBooking()
{
    if (this->_stUndoRequests.isEmpty()) {
        cout << ">> Khong co giao dich dat ve nao de UNDO.\n";
        return false;
    }

    BookingRequest lastRequest = this->_stUndoRequests.pop();
    Flight* pFlight = this->findFlightById(lastRequest.FlightId);

    if (pFlight == nullptr) {
        cout << ">> Loi: Khong tim thay chuyen bay khi UNDO.\n";
        return false;
    }

    bool bOkCancel = pFlight->cancelTicketBySeat(lastRequest.SeatNumber);
    if (!bOkCancel) {
        cout << ">> UNDO that bai (khong the huy ve).\n";
        return false;
    }

    this->deleteTicketFile(lastRequest.FlightId, lastRequest.SeatNumber);
    this->_qWaitingBookings.enQueue(lastRequest);
    this->appendWaitingBookingToFile(lastRequest);

    cout << ">> UNDO thanh cong! Da dua lai yeu cau ve hang doi.\n";
    return true;
}

// ====================== THONG KE ======================

/************
* @Description Thống kê hành khách của chuyến bay
*************/
void Repository::statisticsPassengersOfFlight(const string& strFlightId) const
{
    const Flight* pFlight = this->findFlightById(strFlightId);
    if (pFlight == nullptr) {
        cout << ">> Khong tim thay chuyen bay.\n";
        return;
    }

    if (!fs_exists(PATH_TICKET_DIR)) {
        cout << ">> Chua co thu muc tickets hoac chua co ve nao.\n";
        return;
    }

    struct Row {
        int    Index;
        string IdNumber;
        string FullName;
        int    Seat;
    };
    std::vector<Row> vRows;

    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(PATH_TICKET_DIR)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            string fileName = ent->d_name;
            if (fileName == "." || fileName == "..") continue;

            string fullPath = string(PATH_TICKET_DIR) + fileName;
            std::ifstream fin(fullPath);
            if (!fin.is_open()) continue;

            string strFileFlightId, strIdNumber, strFullName, strSeatLine;
            if (!std::getline(fin, strFileFlightId) || strFileFlightId != strFlightId) continue;
            if (!std::getline(fin, strIdNumber)) continue;
            if (!std::getline(fin, strFullName)) continue;
            if (!std::getline(fin, strSeatLine)) continue;

            Row rowData;
            rowData.Index    = static_cast<int>(vRows.size()) + 1;
            rowData.IdNumber = strIdNumber;
            rowData.FullName = strFullName;
            rowData.Seat     = std::stoi(strSeatLine);
            vRows.push_back(rowData);
        }
        closedir(dir);
    }

    setTextColor(COLOR_TITLE);
    cout << "\n===== DANH SACH KHACH CUA CHUYEN " << strFlightId << " =====\n";
    setTextColor(COLOR_DEFAULT);

    cout << "Tong ghe   : " << pFlight->getTotalSeatCount()  << "\n";
    cout << "Da dat     : " << pFlight->getBookedSeatCount() << "\n";
    cout << "Con trong  : " << pFlight->getFreeSeatCount()   << "\n\n";

    if (vRows.empty()) {
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

    for (const Row& rowData : vRows) {
        cout << std::left
             << std::setw(6)  << rowData.Index
             << std::setw(15) << rowData.IdNumber
             << std::setw(30) << rowData.FullName
             << std::setw(8)  << rowData.Seat
             << "\n";
    }
}

/************
* @Description Thống kê ghế trống
*************/
void Repository::statisticsFreeSeats(const string& strFlightId) const
{
    const Flight* pFlight = this->findFlightById(strFlightId);
    if (pFlight == nullptr) {
        cout << ">> Khong tim thay chuyen bay.\n";
        return;
    }

    setTextColor(COLOR_TITLE);
    cout << "\n===== SO DO GHE - CHUYEN " << strFlightId << " =====\n";
    setTextColor(COLOR_DEFAULT);

    pFlight->printDetail();
}

/************
* @Description Thống kê tần suất máy bay
*************/
void Repository::statisticsPlaneUsage(const string& strPlaneId) const
{
    int iCount = 0;
    for (const Flight& flight : this->_vFlights) {
        if (flight.getPlaneId() == strPlaneId && flight.getStatus() == FLIGHT_COMPLETED) {
            iCount++;
        }
    }

    setTextColor(COLOR_TITLE);
    cout << "\n===== THONG KE MAY BAY " << strPlaneId << " =====\n";
    setTextColor(COLOR_DEFAULT);
    cout << "May bay " << strPlaneId << " da thuc hien " << iCount << " chuyen bay (HOAN TAT).\n";
}

/************
* @Description Lấy thông tin chuyến bay (read-only)
*************/
const Flight* Repository::getFlightConst(const std::string& strFlightId) const
{
    std::string strIdUpper = toUpper(strFlightId);
    for (const Flight& flight : this->_vFlights) {
        if (toUpper(flight.getFlightId()) == strIdUpper) return &flight;
    }
    return nullptr;
}