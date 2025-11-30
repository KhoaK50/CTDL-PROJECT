#include "menu/Menu.h"
#include "utils/Utils.h"
#include "core/Config.h"

#include <iostream>
#include <conio.h>      // _getch()

using std::cout;
using std::endl;
using std::string;

// ======================= Constructor =======================

Menu::Menu(Repository &repo)
    : _repo(repo)
{
}

// ============ HÀM HỖ TRỢ VẼ KHUNG MENU ============

// Căn giữa text trong độ rộng width
static std::string centerText(const std::string &text, int width)
{
    int len = static_cast<int>(text.length());
    if (len >= width) return text;

    int left  = (width - len) / 2;
    int right = width - len - left;
    return std::string(left, ' ') + text + std::string(right, ' ');
}

// In dòng full border: ************
static void printBoxBorder(int width, char border = '*')
{
    std::cout << std::string(width, border) << "\n";
}

// In 1 dòng khung chứa text căn giữa
static void printBoxedTitleLine(const std::string &text, int width, char border = '*')
{
    int innerWidth = width - 2;                 // trừ 2 dấu *
    std::string centered = centerText(text, innerWidth);
    std::cout << border << centered << border << "\n";
}

// In 1 dòng menu: * 1. ....                     *
static void printMenuItemLine(const std::string &text, int width)
{
    int innerWidth = width - 4; // "* " + text + " *"
    std::string s = text;
    if ((int)s.size() > innerWidth)
        s = s.substr(0, innerWidth);

    int padding = innerWidth - (int)s.size();

    setTextColor(COLOR_MENU_FRAME);
    std::cout << "* ";
    setTextColor(COLOR_MENU_TEXT);
    std::cout << s;
    std::cout << std::string(padding, ' ');
    setTextColor(COLOR_MENU_FRAME);
    std::cout << " *\n";
}

// In 1 dòng khung trống: *                      *
static void printEmptyMenuLine(int width)
{
    int innerWidth = width - 4;
    setTextColor(COLOR_MENU_FRAME);
    std::cout << "* " << std::string(innerWidth, ' ') << " *\n";
}

// ======================= VONG LAP CHINH =======================

void Menu::run()
{
    while (true)
    {
        showMainMenu();

        int iChoice = inputIntInRange("Nhap lua chon (0 de thoat): ", 0, 3);
        handleMainChoice(iChoice);

        if (iChoice == 0)
        {
            // Luu du lieu truoc khi thoat
            _repo.saveAllData();
            cout << "\nDu lieu da duoc luu. Thoat chuong trinh...\n";
            break;
        }
    }
}

// ======================= MENU CHINH =======================

void Menu::showMainMenu() const
{
    clearScreen();

    const int WIDTH = 60;

    // Khung + tiêu đề (cyan)
    setTextColor(COLOR_MENU_FRAME);
    printBoxBorder(WIDTH);
    printBoxedTitleLine("HE THONG QUAN LY CHUYEN BAY", WIDTH);
    printBoxBorder(WIDTH);

    // Nội dung menu (xanh lá)
    printEmptyMenuLine(WIDTH);
    printMenuItemLine("1. Xem danh sach chuyen bay", WIDTH);
    printMenuItemLine("2. Dat ve (khach ben ngoai he thong)", WIDTH);
    printMenuItemLine("3. Dang nhap quan ly", WIDTH);
    printMenuItemLine("0. Thoat va luu du lieu", WIDTH);
    printEmptyMenuLine(WIDTH);

    setTextColor(COLOR_MENU_FRAME);
    printBoxBorder(WIDTH);
    setTextColor(COLOR_DEFAULT);

    cout << "\n";
}

void Menu::handleMainChoice(int iChoice)
{
    switch (iChoice)
    {
    case 1:
        displayAllFlights();
        break;

    case 2:
        userBookTicket();
        break;

    case 3:
        loginAndManagementMenu();
        break;

    case 0:
    default:
        // Da xu ly trong run()
        break;
    }
}

// ======================= LOGIN + MENU QUAN LY =======================

void Menu::loginAndManagementMenu()
{
    const int iMaxAttempts = 3;
    int       iFailedCount = 0;

    while (true)
    {
        clearScreen();

        const int WIDTH = 50;

        // Khung đăng nhập
        setTextColor(COLOR_MENU_FRAME);
        printBoxBorder(WIDTH);
        printBoxedTitleLine("DANG NHAP HE THONG", WIDTH);
        printBoxBorder(WIDTH);
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

        // Kiem tra tai khoan
        if (_repo.checkAdminLogin(strUserName, strPassword))
        {
            setTextColor(COLOR_SUCCESS);
            cout << "\nDang nhap thanh cong!\n";
            setTextColor(COLOR_DEFAULT);
            pauseScreen();

            // Vao menu quan ly
            while (true)
            {
                showManagementMenu();
                int iChoice = inputIntInRange("Nhap lua chon (0 de quay ve menu chinh): ", 0, 4);
                handleManagementChoice(iChoice);

                if (iChoice == 0)
                {
                    return; // quay ve menu chinh
                }
            }
        }

        // Sai tai khoan / mat khau
        iFailedCount++;

        setTextColor(COLOR_ERROR);
        cout << "\nSai ten dang nhap hoac mat khau! (" << iFailedCount << "/" << iMaxAttempts << ")\n";
        setTextColor(COLOR_DEFAULT);

        if (iFailedCount >= iMaxAttempts)
        {
            cout << "Ban da nhap sai " << iMaxAttempts << " lan.\n";
            cout << "Nhan ESC de quay ve menu chinh, hoac phim bat ky de thu lai...\n";

            int cKey = _getch();
            if (cKey == 27)       // 27 = ESC
            {
                return;
            }

            iFailedCount = 0;     // thu lai tu dau
        }

        pauseScreen();
    }
}

// ======================= MENU QUAN LY =======================

void Menu::showManagementMenu() const
{
    clearScreen();

    const int WIDTH = 60;

    setTextColor(COLOR_MENU_FRAME);
    printBoxBorder(WIDTH);
    printBoxedTitleLine("MENU QUAN LY", WIDTH);
    printBoxBorder(WIDTH);

    printEmptyMenuLine(WIDTH);
    printMenuItemLine("1. Xu ly dat ve", WIDTH);
    printMenuItemLine("2. Xu ly tra / huy ve", WIDTH);
    printMenuItemLine("3. Thong ke", WIDTH);
    printMenuItemLine("4. UNDO lan dat ve gan nhat", WIDTH);
    printMenuItemLine("0. Quay ve menu chinh", WIDTH);
    printEmptyMenuLine(WIDTH);

    printBoxBorder(WIDTH);
    setTextColor(COLOR_DEFAULT);

    cout << "\n";
}

void Menu::handleManagementChoice(int iChoice)
{
    switch (iChoice)
    {
    case 1:
        adminProcessBooking();
        break;

    case 2:
        adminCancelTicket();
        break;

    case 3:
    {
        while (true)
        {
            showStatisticMenu();
            int iSub = inputIntInRange("Nhap lua chon (0 de quay lai): ", 0, 3);
            handleStatisticChoice(iSub);
            if (iSub == 0)
            {
                break;
            }
        }
        break;
    }

    case 4:
        undoLastBooking();
        break;

    case 0:
    default:
        break;
    }
}

// ======================= MENU THONG KE =======================

void Menu::showStatisticMenu() const
{
    clearScreen();

    const int WIDTH = 60;

    setTextColor(COLOR_MENU_FRAME);
    printBoxBorder(WIDTH);
    printBoxedTitleLine("MENU THONG KE", WIDTH);
    printBoxBorder(WIDTH);

    printEmptyMenuLine(WIDTH);
    printMenuItemLine("1. Thong ke khach cua 1 chuyen bay", WIDTH);
    printMenuItemLine("2. Thong ke ghe con trong cua 1 chuyen", WIDTH);
    printMenuItemLine("3. Thong ke so lan 1 may bay thuc hien chuyen bay", WIDTH);
    printMenuItemLine("0. Quay ve menu quan ly", WIDTH);
    printEmptyMenuLine(WIDTH);

    printBoxBorder(WIDTH);
    setTextColor(COLOR_DEFAULT);

    cout << "\n";
}

void Menu::handleStatisticChoice(int iChoice)
{
    switch (iChoice)
    {
    case 1:
        statisticsByFlight();
        break;

    case 2:
        statisticsFreeSeats();
        break;

    case 3:
        statisticsPlaneUsage();
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

void Menu::displayAllFlights()
{
    clearScreen();
    _repo.printAllFlights();
    cout << "\n";
    pauseScreen();
}

// ----- 2. Khach ben ngoai dat ve (vao Queue) -----

void Menu::userBookTicket()
{
    clearScreen();
    setTextColor(COLOR_TITLE);
    cout << "===== DAT VE CHO KHACH HANG =====\n";
    setTextColor(COLOR_DEFAULT);

    // B1. Nhap thong tin co ban
    string strFlightId  = inputValidFlightId("Nhap ma chuyen bay: ");
    string strIdNumber  = inputValidIDNumber("Nhap CMND/CCCD (9 hoac 12 so): ");
    string strFullName  = inputValidFullName("Nhap ho ten khach hang: ");

    // B2. Tim chuyen bay tu Repository de hien so do ghe
    const Flight* pFlight = _repo.getFlightConst(strFlightId);
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

    int totalSeats = pFlight->getTotalSeatCount();
    cout << "\n(Chon so ghe trong khoang 1.." << totalSeats << ")\n\n";

    int iSeatNumber = inputIntStrict("Nhap so ghe muon dat: ", 1, totalSeats);

    bool bOk = _repo.userRegisterBooking(strFlightId,
                                         strIdNumber,
                                         strFullName,
                                         iSeatNumber);

    if (bOk)
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

void Menu::adminProcessBooking()
{
    while (true)
    {
        clearScreen();
        setTextColor(COLOR_TITLE);
        cout << "===== XU LY DAT VE (ADMIN) =====\n\n";
        setTextColor(COLOR_DEFAULT);

        _repo.printWaitingBookings();

        cout << "\nY: Duyet yeu cau ve dat"
             << "\nN: Huy / bo qua yeu cau ve dat"
             << "\n0: Quay ve menu quan ly\n\n";

        std::string s = inputNonEmptyString("Nhap lua chon (Y/N/0): ");
        char c = s.empty() ? '0' : s[0];

        if (c == '0')
        {
            return;
        }
        else if (c == 'y' || c == 'Y')
        {
            bool bOk = _repo.adminProcessNextBooking();

            if (!bOk)
            {
                setTextColor(COLOR_ERROR);
                cout << "\nKhong xu ly duoc yeu cau dat ve.\n";
                setTextColor(COLOR_DEFAULT);
            }

            pauseScreen();
        }
        else if (c == 'n' || c == 'N')
        {
            bool bOk = _repo.rejectNextBooking();
            (void)bOk;

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

void Menu::adminCancelTicket()
{
    clearScreen();
    setTextColor(COLOR_TITLE);
    cout << "===== HUY VE (ADMIN) =====\n";
    setTextColor(COLOR_DEFAULT);

    string strFlightId = inputValidFlightId("Nhap ma chuyen bay: ");
    int    iSeatNumber = inputIntStrict("Nhap so ghe can huy (1..300): ", 1, 300);

    bool bOk = _repo.adminCancelTicket(strFlightId, iSeatNumber);

    if (bOk)
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

void Menu::statisticsByFlight()
{
    string strFlightId = inputValidFlightId("Nhap ma chuyen bay: ");
    _repo.statisticsPassengersOfFlight(strFlightId);
}

// ----- 6. Thong ke ghe trong cua 1 chuyen -----

void Menu::statisticsFreeSeats()
{
    string strFlightId = inputValidFlightId("Nhap ma chuyen bay: ");
    _repo.statisticsFreeSeats(strFlightId);
}

// ----- 7. Thong ke so lan 1 may bay thuc hien chuyen -----

void Menu::statisticsPlaneUsage()
{
    string strPlaneId = inputNonEmptyString("Nhap so hieu may bay: ");
    strPlaneId = toUpper(strPlaneId);
    _repo.statisticsPlaneUsage(strPlaneId);
}

// ----- 8. UNDO booking gan nhat -----

void Menu::undoLastBooking()
{
    clearScreen();
    setTextColor(COLOR_TITLE);
    cout << "===== UNDO LAN DAT VE GAN NHAT =====\n";
    setTextColor(COLOR_DEFAULT);

    bool bOk = _repo.undoLastBooking();

    if (!bOk)
    {
        setTextColor(COLOR_ERROR);
        cout << "\nKhong the UNDO.\n";
        setTextColor(COLOR_DEFAULT);
    }

    pauseScreen();
}
