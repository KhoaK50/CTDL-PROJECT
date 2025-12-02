#include "menu/Menu.h"
#include "utils/Utils.h"
#include "core/Config.h"

#include <iostream>
#include <conio.h>      // _getch()

// Rule 12: Sử dụng namespace std trong file .cpp
using namespace std;

// ======================= Constructor =======================

/************
* @Description Constructor khởi tạo Menu
*************/
Menu::Menu(Repository &repo)
    : _repo(repo)
{
}

// ============ HÀM HỖ TRỢ VẼ KHUNG MENU (STATIC) ============

/************
* @Description Căn giữa text trong độ rộng cho trước
* @param strText Chuỗi văn bản cần căn giữa
* @param iWidth Độ rộng khung
* @return Chuỗi đã được thêm khoảng trắng
*************/
static string centerText(const string &strText, int iWidth)
{
    int iLen = static_cast<int>(strText.length());
    if (iLen >= iWidth) return strText;

    int iLeft  = (iWidth - iLen) / 2;
    int iRight = iWidth - iLen - iLeft;
    return string(iLeft, ' ') + strText + string(iRight, ' ');
}

/************
* @Description In dòng viền đầy đủ (Ví dụ: ************)
* @param iWidth Độ rộng
* @param cBorder Ký tự viền
*************/
static void printBoxBorder(int iWidth, char cBorder = '*')
{
    cout << string(iWidth, cBorder) << "\n";
}

/************
* @Description In dòng tiêu đề có viền và căn giữa
* @param strText Nội dung tiêu đề
* @param iWidth Độ rộng
* @param cBorder Ký tự viền
*************/
static void printBoxedTitleLine(const string &strText, int iWidth, char cBorder = '*')
{
    int iInnerWidth = iWidth - 2;                 // trừ 2 dấu *
    string strCentered = centerText(strText, iInnerWidth);
    cout << cBorder << strCentered << cBorder << "\n";
}

/************
* @Description In một dòng menu (Ví dụ: * 1. Chuc nang...      *)
* @param strText Nội dung menu
* @param iWidth Độ rộng
*************/
static void printMenuItemLine(const string &strText, int iWidth)
{
    int iInnerWidth = iWidth - 4; // "* " + text + " *"
    string strDisplay = strText;
    
    // Cắt chuỗi nếu quá dài
    if ((int)strDisplay.size() > iInnerWidth)
        strDisplay = strDisplay.substr(0, iInnerWidth);

    int iPadding = iInnerWidth - (int)strDisplay.size();

    setTextColor(COLOR_MENU_FRAME);
    cout << "* ";
    setTextColor(COLOR_MENU_TEXT);
    cout << strDisplay;
    cout << string(iPadding, ' ');
    setTextColor(COLOR_MENU_FRAME);
    cout << " *\n";
}

/************
* @Description In dòng khung trống (padding)
* @param iWidth Độ rộng
*************/
static void printEmptyMenuLine(int iWidth)
{
    int iInnerWidth = iWidth - 4;
    setTextColor(COLOR_MENU_FRAME);
    cout << "* " << string(iInnerWidth, ' ') << " *\n";
}

// ======================= VONG LAP CHINH =======================

/************
* @Description Vòng lặp chính điều khiển chương trình
*************/
void Menu::run()
{
    while (true)
    {
        this->showMainMenu();

        int iChoice = inputIntInRange("Nhap lua chon (0 de thoat): ", 0, 3);
        this->handleMainChoice(iChoice);

        if (iChoice == 0)
        {
            // Luu du lieu truoc khi thoat
            this->_repo.saveAllData();
            cout << "\nDu lieu da duoc luu. Thoat chuong trinh...\n";
            break;
        }
    }
}

// ======================= MENU CHINH =======================

/************
* @Description Hiển thị Menu chính
*************/
void Menu::showMainMenu() const
{
    clearScreen();

    const int MENU_WIDTH = 60;

    // Khung + tiêu đề (cyan)
    setTextColor(COLOR_MENU_FRAME);
    printBoxBorder(MENU_WIDTH);
    printBoxedTitleLine("HE THONG QUAN LY CHUYEN BAY", MENU_WIDTH);
    printBoxBorder(MENU_WIDTH);

    // Nội dung menu (xanh lá)
    printEmptyMenuLine(MENU_WIDTH);
    printMenuItemLine("1. Xem danh sach chuyen bay", MENU_WIDTH);
    printMenuItemLine("2. Dat ve (khach ben ngoai he thong)", MENU_WIDTH);
    printMenuItemLine("3. Dang nhap quan ly", MENU_WIDTH);
    printMenuItemLine("0. Thoat va luu du lieu", MENU_WIDTH);
    printEmptyMenuLine(MENU_WIDTH);

    setTextColor(COLOR_MENU_FRAME);
    printBoxBorder(MENU_WIDTH);
    setTextColor(COLOR_DEFAULT);

    cout << "\n";
}

/************
* @Description Xử lý lựa chọn Menu chính
*************/
void Menu::handleMainChoice(int iChoice)
{
    switch (iChoice)
    {
    case 1:
        this->displayAllFlights();
        break;

    case 2:
        this->userBookTicket();
        break;

    case 3:
        this->loginAndManagementMenu();
        break;

    case 0:
    default:
        // Da xu ly trong run()
        break;
    }
}

// ======================= LOGIN + MENU QUAN LY =======================

/************
* @Description Quy trình đăng nhập và chuyển hướng sang Menu quản lý
*************/
void Menu::loginAndManagementMenu()
{
    const int MAX_ATTEMPTS = 3;
    const int KEY_ESC = 27;
    int iFailedCount = 0;

    while (true)
    {
        clearScreen();

        const int FORM_WIDTH = 50;

        // Khung đăng nhập
        setTextColor(COLOR_MENU_FRAME);
        printBoxBorder(FORM_WIDTH);
        printBoxedTitleLine("DANG NHAP HE THONG", FORM_WIDTH);
        printBoxBorder(FORM_WIDTH);
        setTextColor(COLOR_DEFAULT);

        cout << "\n";

        // Nhap User
        setTextColor(COLOR_MENU_TEXT);
        cout << "User: ";
        setTextColor(COLOR_DEFAULT);
        string strUserName = inputNonEmptyString("");

        // Nhap Password (co che bang '*')
        setTextColor(COLOR_MENU_TEXT);
        cout << "Password: ";
        setTextColor(COLOR_DEFAULT);
        string strPassword = inputPassword("");

        // Kiem tra tai khoan (Rule 27: dùng this->)
        if (this->_repo.checkAdminLogin(strUserName, strPassword))
        {
            setTextColor(COLOR_SUCCESS);
            cout << "\nDang nhap thanh cong!\n";
            setTextColor(COLOR_DEFAULT);
            pauseScreen();

            // Vao menu quan ly
            while (true)
            {
                this->showManagementMenu();
                int iChoice = inputIntInRange("Nhap lua chon (0 de quay ve menu chinh): ", 0, 4);
                this->handleManagementChoice(iChoice);

                if (iChoice == 0)
                {
                    return; // quay ve menu chinh
                }
            }
        }

        // Sai tai khoan / mat khau
        iFailedCount++;

        setTextColor(COLOR_ERROR);
        cout << "\nSai ten dang nhap hoac mat khau! (" << iFailedCount << "/" << MAX_ATTEMPTS << ")\n";
        setTextColor(COLOR_DEFAULT);

        if (iFailedCount >= MAX_ATTEMPTS)
        {
            cout << "Ban da nhap sai " << MAX_ATTEMPTS << " lan.\n";
            cout << "Nhan ESC de quay ve menu chinh, hoac phim bat ky de thu lai...\n";

            int cKey = _getch();
            if (cKey == KEY_ESC) 
            {
                return;
            }

            iFailedCount = 0;     // thu lai tu dau
        }

        pauseScreen();
    }
}

// ======================= MENU QUAN LY =======================

/************
* @Description Hiển thị Menu quản lý
*************/
void Menu::showManagementMenu() const
{
    clearScreen();

    const int MENU_WIDTH = 60;

    setTextColor(COLOR_MENU_FRAME);
    printBoxBorder(MENU_WIDTH);
    printBoxedTitleLine("MENU QUAN LY", MENU_WIDTH);
    printBoxBorder(MENU_WIDTH);

    printEmptyMenuLine(MENU_WIDTH);
    printMenuItemLine("1. Xu ly dat ve", MENU_WIDTH);
    printMenuItemLine("2. Xu ly tra / huy ve", MENU_WIDTH);
    printMenuItemLine("3. Thong ke", MENU_WIDTH);
    printMenuItemLine("4. UNDO lan dat ve gan nhat", MENU_WIDTH);
    printMenuItemLine("0. Quay ve menu chinh", MENU_WIDTH);
    printEmptyMenuLine(MENU_WIDTH);

    printBoxBorder(MENU_WIDTH);
    setTextColor(COLOR_DEFAULT);

    cout << "\n";
}

/************
* @Description Xử lý lựa chọn Menu quản lý
*************/
void Menu::handleManagementChoice(int iChoice)
{
    switch (iChoice)
    {
    case 1:
        this->adminProcessBooking();
        break;

    case 2:
        this->adminCancelTicket();
        break;

    case 3:
    {
        while (true)
        {
            this->showStatisticMenu();
            int iSub = inputIntInRange("Nhap lua chon (0 de quay lai): ", 0, 3);
            this->handleStatisticChoice(iSub);
            if (iSub == 0)
            {
                break;
            }
        }
        break;
    }

    case 4:
        this->undoLastBooking();
        break;

    case 0:
    default:
        break;
    }
}

// ======================= MENU THONG KE =======================

/************
* @Description Hiển thị Menu thống kê
*************/
void Menu::showStatisticMenu() const
{
    clearScreen();

    const int MENU_WIDTH = 60;

    setTextColor(COLOR_MENU_FRAME);
    printBoxBorder(MENU_WIDTH);
    printBoxedTitleLine("MENU THONG KE", MENU_WIDTH);
    printBoxBorder(MENU_WIDTH);

    printEmptyMenuLine(MENU_WIDTH);
    printMenuItemLine("1. Thong ke khach cua 1 chuyen bay", MENU_WIDTH);
    printMenuItemLine("2. Thong ke ghe con trong cua 1 chuyen", MENU_WIDTH);
    printMenuItemLine("3. Thong ke so lan 1 may bay thuc hien chuyen bay", MENU_WIDTH);
    printMenuItemLine("0. Quay ve menu quan ly", MENU_WIDTH);
    printEmptyMenuLine(MENU_WIDTH);

    printBoxBorder(MENU_WIDTH);
    setTextColor(COLOR_DEFAULT);

    cout << "\n";
}

/************
* @Description Xử lý lựa chọn Menu thống kê
*************/
void Menu::handleStatisticChoice(int iChoice)
{
    switch (iChoice)
    {
    case 1:
        this->statisticsByFlight();
        break;

    case 2:
        this->statisticsFreeSeats();
        break;

    case 3:
        this->statisticsPlaneUsage();
        break;

    case 0:
    default:
        break;
    }

    if (iChoice != 0)
    {
        pauseScreen();
    }
}

// ======================= CHUC NANG NGHIEP VU =======================

// ----- 1. Hien thi danh sach chuyen bay -----

/************
* @Description Hiển thị tất cả chuyến bay
*************/
void Menu::displayAllFlights()
{
    clearScreen();
    // Rule 27: Dùng this->
    this->_repo.printAllFlights();
    cout << "\n";
    pauseScreen();
}

// ----- 2. Khach ben ngoai dat ve (vao Queue) -----

/************
* @Description Chức năng đặt vé dành cho khách hàng
*************/
void Menu::userBookTicket()
{
    clearScreen();
    setTextColor(COLOR_TITLE);
    cout << "===== DAT VE CHO KHACH HANG =====\n";
    setTextColor(COLOR_DEFAULT);

    // B1. Nhap thong tin co ban (biến local dùng Hungarian)
    string strFlightId  = inputValidFlightId("Nhap ma chuyen bay: ");
    string strIdNumber  = inputValidIDNumber("Nhap CMND/CCCD (9 hoac 12 so): ");
    string strFullName  = inputValidFullName("Nhap ho ten khach hang: ");

    // B2. Tim chuyen bay tu Repository de hien so do ghe
    const Flight* pFlight = this->_repo.getFlightConst(strFlightId);
    if (!pFlight)
    {
        setTextColor(COLOR_ERROR);
        cout << "\nLoi: Khong tim thay chuyen bay trong he thong.\n";
        setTextColor(COLOR_DEFAULT);
        pauseScreen();
        return;
    }

    clearScreen();
    setTextColor(COLOR_TITLE);
    cout << "===== THONG TIN CHUYEN BAY & SO DO GHE =====\n";
    setTextColor(COLOR_DEFAULT);

    pFlight->printDetail();

    int iTotalSeats = pFlight->getTotalSeatCount();
    cout << "\n(Chon so ghe trong khoang 1.." << iTotalSeats << ")\n\n";

    int iSeatNumber = inputIntStrict("Nhap so ghe muon dat: ", 1, iTotalSeats);

    bool bIsSuccess = this->_repo.userRegisterBooking(strFlightId,
                                                      strIdNumber,
                                                      strFullName,
                                                      iSeatNumber);

    if (bIsSuccess)
    {
        setTextColor(COLOR_SUCCESS);
        cout << "\nDa them yeu cau dat ve vao hang doi.\n";
    }
    else
    {
        setTextColor(COLOR_ERROR);
        cout << "\nKhong the ghi nhan yeu cau dat ve.\n";
    }
    setTextColor(COLOR_DEFAULT);

    pauseScreen();
}

// ----- 3. Admin xu ly dat ve trong Queue -----

/************
* @Description Admin duyệt hoặc từ chối các yêu cầu đặt vé
*************/
void Menu::adminProcessBooking()
{
    while (true)
    {
        clearScreen();
        setTextColor(COLOR_TITLE);
        cout << "===== XU LY DAT VE (ADMIN) =====\n\n";
        setTextColor(COLOR_DEFAULT);

        this->_repo.printWaitingBookings();

        cout << "\nY: Duyet yeu cau ve dat"
             << "\nN: Huy / bo qua yeu cau ve dat"
             << "\n0: Quay ve menu quan ly\n\n";

        string strInput = inputNonEmptyString("Nhap lua chon (Y/N/0): ");
        char cChoice = strInput.empty() ? '0' : strInput[0];

        if (cChoice == '0')
        {
            return;
        }
        else if (cChoice == 'y' || cChoice == 'Y')
        {
            bool bIsSuccess = this->_repo.adminProcessNextBooking();

            if (!bIsSuccess)
            {
                setTextColor(COLOR_ERROR);
                cout << "\nKhong xu ly duoc yeu cau dat ve.\n";
                setTextColor(COLOR_DEFAULT);
            }

            pauseScreen();
        }
        else if (cChoice == 'n' || cChoice == 'N')
        {
            bool bIsSuccess = this->_repo.rejectNextBooking();
            (void)bIsSuccess; // Suppress unused variable warning

            pauseScreen();
        }
        else
        {
            setTextColor(COLOR_ERROR);
            cout << "\nLua chon khong hop le. Vui long nhap lai.\n";
            setTextColor(COLOR_DEFAULT);
            pauseScreen();
        }
    }
}

// ----- 4. Admin huy ve truc tiep -----

/************
* @Description Admin hủy vé đã đặt
*************/
void Menu::adminCancelTicket()
{
    clearScreen();
    setTextColor(COLOR_TITLE);
    cout << "===== HUY VE (ADMIN) =====\n";
    setTextColor(COLOR_DEFAULT);

    string strFlightId = inputValidFlightId("Nhap ma chuyen bay: ");
    int    iSeatNumber = inputIntStrict("Nhap so ghe can huy (1..300): ", 1, 300);

    bool bIsSuccess = this->_repo.adminCancelTicket(strFlightId, iSeatNumber);

    if (bIsSuccess)
    {
        setTextColor(COLOR_SUCCESS);
        cout << "\nDa huy ve thanh cong.\n";
    }
    else
    {
        setTextColor(COLOR_ERROR);
        cout << "\nKhong the huy ve.\n";
    }
    setTextColor(COLOR_DEFAULT);

    pauseScreen();
}

// ----- 5. Thong ke khach cua 1 chuyen -----

/************
* @Description Thống kê hành khách theo chuyến bay
*************/
void Menu::statisticsByFlight()
{
    string strFlightId = inputValidFlightId("Nhap ma chuyen bay: ");
    this->_repo.statisticsPassengersOfFlight(strFlightId);
}

// ----- 6. Thong ke ghe trong cua 1 chuyen -----

/************
* @Description Thống kê ghế trống
*************/
void Menu::statisticsFreeSeats()
{
    string strFlightId = inputValidFlightId("Nhap ma chuyen bay: ");
    this->_repo.statisticsFreeSeats(strFlightId);
}

// ----- 7. Thong ke so lan 1 may bay thuc hien chuyen -----

/************
* @Description Thống kê tần suất bay của máy bay
*************/
void Menu::statisticsPlaneUsage()
{
    string strPlaneId = inputNonEmptyString("Nhap so hieu may bay: ");
    strPlaneId = toUpper(strPlaneId);
    this->_repo.statisticsPlaneUsage(strPlaneId);
}

// ----- 8. UNDO booking gan nhat -----

/************
* @Description Hoàn tác lệnh đặt vé gần nhất
*************/
void Menu::undoLastBooking()
{
    clearScreen();
    setTextColor(COLOR_TITLE);
    cout << "===== UNDO LAN DAT VE GAN NHAT =====\n";
    setTextColor(COLOR_DEFAULT);

    bool bIsSuccess = this->_repo.undoLastBooking();

    if (!bIsSuccess)
    {
        setTextColor(COLOR_ERROR);
        cout << "\nKhong the UNDO.\n";
        setTextColor(COLOR_DEFAULT);
    }

    pauseScreen();
}