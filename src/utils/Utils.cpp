#include "utils/Utils.h"
#include "core/Config.h"

#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>
#include <conio.h>      // _getch()

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>    // SetConsoleTextAttribute, GetStdHandle, Sleep

#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;


// ================== HÀM XỬ LÝ CONSOLE ==================

/************
* @Description Xóa màn hình console
*************/
void clearScreen()
{
    system("cls");
}

/************
* @Description Tạm dừng chương trình chờ người dùng nhấn Enter
*************/
void pauseScreen()
{
    cout << "Nhan Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/************
* @Description Đổi màu chữ console
* @param iColorCode Mã màu
*************/
void setTextColor(int iColorCode)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)iColorCode);
}


// ================== HÀM NHẬP LIỆU AN TOÀN ==================

/************
* @Description Nhập số nguyên trong khoảng
* @param strPrompt Câu nhắc
* @param iMin Giá trị nhỏ nhất
* @param iMax Giá trị lớn nhất
* @return Số nguyên hợp lệ
*************/
int inputIntInRange(const string &strPrompt, int iMin, int iMax)
{
    int iValue = 0;

    while (true)
    {
        cout << strPrompt;
        cin >> iValue;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            setTextColor(COLOR_ERROR);
            cout << "Loi: Vui long nhap so nguyen!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (iValue < iMin || iValue > iMax)
        {
            setTextColor(COLOR_WARNING);
            cout << "Gia tri phai nam trong [" << iMin << ", " << iMax << "]. Nhap lai.\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        return iValue;
    }
}

/************
* @Description Nhập chuỗi không rỗng
* @param strPrompt Câu nhắc
* @return Chuỗi đã nhập và trim
*************/
string inputNonEmptyString(const string &strPrompt)
{
    string strInput;

    while (true)
    {
        cout << strPrompt;
        getline(cin, strInput);

        strInput = trim(strInput);

        if (!strInput.empty())
        {
            return strInput;
        }

        setTextColor(COLOR_WARNING);
        cout << "Khong duoc de trong. Nhap lai.\n";
        setTextColor(COLOR_DEFAULT);
    }
}

/************
* @Description Nhập mật khẩu có che ký tự
* @param strPrompt Câu nhắc
* @return Chuỗi mật khẩu
*************/
string inputPassword(const string &strPrompt)
{
    cout << strPrompt;

    string strPassword;

    while (true)
    {
        char cCh = _getch();

        // ENTER -> kết thúc
        if (cCh == '\r')
        {
            cout << "\n";
            break;
        }
        // BACKSPACE
        else if (cCh == 8)
        {
            if (!strPassword.empty())
            {
                strPassword.pop_back();
                // Xóa 1 ký tự '*' trên màn hình:
                // 1) \b : lùi lại
                // 2) ' ' : ghi đè bằng khoảng trắng
                // 3) \b : lùi lại lần nữa
                cout << "\b \b";
            }
        }
        else
        {
            strPassword.push_back(cCh);
            cout << PASSWORD_MASK_CHAR;
        }
    }

    return strPassword;
}

/************
* @Description Nhập ngày hợp lệ dd/mm/yyyy
* @param strPrompt Câu nhắc
* @return Chuỗi ngày hợp lệ
*************/
string inputValidDate(const string &strPrompt)
{
    while (true)
    {
        string strDate = inputNonEmptyString(strPrompt);

        if (isValidDate(strDate))
        {
            return strDate;
        }

        setTextColor(COLOR_ERROR);
        cout << "Ngay thang khong hop le! Dinh dang phai la dd/mm/yyyy. Nhap lai.\n";
        setTextColor(COLOR_DEFAULT);
    }
}

/************
* @Description Nhập ngày giờ hợp lệ dd/mm/yyyy HH:MM
* @param strPrompt Câu nhắc
* @return Chuỗi ngày giờ hợp lệ
*************/
string inputValidDateTime(const string &strPrompt)
{
    while (true)
    {
        string s = inputNonEmptyString(strPrompt);

        if (isValidDateTime(s))
            return s;

        setTextColor(COLOR_ERROR);
        cout << "Loi: Ngay gio khong hop le! Dinh dang dung: dd/mm/yyyy HH:MM\n";
        setTextColor(COLOR_DEFAULT);
    }
}

// ===== NHẬP HỌ TÊN HỢP LỆ =====

/************
* @Description Nhập họ tên hợp lệ
* @param strPrompt Câu nhắc
* @return Họ tên đã chuẩn hóa (UPPER)
*************/
string inputValidFullName(const string &strPrompt)
{
    while (true)
    {
        cout << strPrompt;
        string name;
        getline(cin, name);

        name = trim(name);

        if (name.size() < 2)
        {
            setTextColor(COLOR_ERROR);
            cout << "Loi: Ho ten phai co it nhat 2 ky tu!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        bool valid = true;
        for (char c : name)
        {
            if (!isalpha((unsigned char)c) && c != ' ')
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            setTextColor(COLOR_ERROR);
            cout << "Loi: Ho ten chi duoc chua chu cai va dau cach!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        return toUpper(name);
    }
}

// ===== NHẬP CMND / CCCD HỢP LỆ =====

/************
* @Description Nhập CMND/CCCD hợp lệ
* @param strPrompt Câu nhắc
* @return Chuỗi số ID
*************/
string inputValidIDNumber(const string &strPrompt)
{
    while (true)
    {
        cout << strPrompt;
        string id;
        getline(cin, id);
        id = trim(id);

        if (id.empty())
        {
            setTextColor(COLOR_ERROR);
            cout << "Loi: Khong duoc de trong!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        if (id.find(' ') != string::npos)
        {
            setTextColor(COLOR_ERROR);
            cout << "Loi: CMND/CCCD khong duoc chua khoang trang!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        bool allDigit = true;
        for (char c : id)
        {
            if (!isdigit((unsigned char)c))
            {
                allDigit = false;
                break;
            }
        }

        if (!allDigit)
        {
            setTextColor(COLOR_ERROR);
            cout << "Loi: CMND/CCCD chi duoc chua chu so!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        if (id.size() == 9 || id.size() == 12)
        {
            return id;
        }

        setTextColor(COLOR_ERROR);
        cout << "Loi: CMND phai 9 so, CCCD phai 12 so!\n";
        setTextColor(COLOR_DEFAULT);
    }
}

// ===== NHẬP MÃ CHUYẾN BAY HỢP LỆ =====

/************
* @Description Nhập mã chuyến bay hợp lệ
* @param strPrompt Câu nhắc
* @return Mã chuyến bay (UPPER)
*************/
string inputValidFlightId(const string &strPrompt)
{
    while (true)
    {
        cout << strPrompt;
        string id;
        getline(cin, id);
        id = trim(id);

        if (id.empty())
        {
            setTextColor(COLOR_ERROR);
            cout << "Loi: Ma chuyen bay khong duoc de trong!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        if (id.find(' ') != string::npos)
        {
            setTextColor(COLOR_ERROR);
            cout << "Loi: Ma chuyen bay khong duoc chua khoang trang!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        return toUpper(id);
    }
}

// ===== NHẬP SỐ NGUYÊN KIỂM TRA CHẶT =====

/************
* @Description Nhập số nguyên kiểm tra chặt chẽ
* @param strPrompt Câu nhắc
* @param iMin Giá trị min
* @param iMax Giá trị max
* @return Số nguyên
*************/
int inputIntStrict(const string &strPrompt, int iMin, int iMax)
{
    while (true)
    {
        cout << strPrompt;
        string s;
        getline(cin, s);
        s = trim(s);

        if (s.empty())
        {
            setTextColor(COLOR_ERROR);
            cout << "Loi: Khong duoc de trong!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        bool allDigit = true;
        for (char c : s)
        {
            if (!isdigit((unsigned char)c))
            {
                allDigit = false;
                break;
            }
        }

        if (!allDigit)
        {
            setTextColor(COLOR_ERROR);
            cout << "Loi: Chi duoc nhap chu so!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        int value = stoi(s);
        if (value < iMin || value > iMax)
        {
            setTextColor(COLOR_ERROR);
            cout << "Loi: Gia tri phai nam trong [" << iMin << " - " << iMax << "]!\n";
            setTextColor(COLOR_DEFAULT);
            continue;
        }

        return value;
    }
}


// ================== HÀM XỬ LÝ CHUỖI ==================

/************
* @Description Cắt khoảng trắng 2 đầu chuỗi
* @param strInput Chuỗi vào
* @return Chuỗi kết quả
*************/
string trim(const string &strInput)
{
    if (strInput.empty())
    {
        return strInput;
    }

    int iLeft  = 0;
    int iRight = (int)strInput.size() - 1;

    while (iLeft <= iRight && isspace((unsigned char)strInput[iLeft]))
    {
        iLeft++;
    }

    while (iRight >= iLeft && isspace((unsigned char)strInput[iRight]))
    {
        iRight--;
    }

    return strInput.substr(iLeft, iRight - iLeft + 1);
}

/************
* @Description Chuyển chuỗi sang in hoa
* @param strInput Chuỗi vào
* @return Chuỗi in hoa
*************/
string toUpper(const string &strInput)
{
    string strResult = strInput;

    transform(strResult.begin(), strResult.end(), strResult.begin(),
              [](unsigned char cCh)
              {
                  return (char)toupper(cCh);
              });

    return strResult;
}


// ================== KIỂM TRA NGÀY ==================

/************
* @Description Kiểm tra tính hợp lệ của ngày dd/mm/yyyy
* @param strDate Chuỗi ngày
* @return true nếu hợp lệ
*************/
bool isValidDate(const string &strDate)
{
    // Dinh dang: dd/mm/yyyy -> 10 ky tu
    if (strDate.size() != 10)
    {
        return false;
    }

    if (strDate[2] != '/' || strDate[5] != '/')
    {
        return false;
    }

    // Kiem tra cac ky tu con lai la so
    for (int iIndex = 0; iIndex < 10; iIndex++)
    {
        if (iIndex == 2 || iIndex == 5)
        {
            continue;
        }

        if (!isdigit((unsigned char)strDate[iIndex]))
        {
            return false;
        }
    }

    int iDay   = stoi(strDate.substr(0, 2));
    int iMonth = stoi(strDate.substr(3, 2));
    int iYear  = stoi(strDate.substr(6, 4));

    if (iYear < 1900 || iYear > 3000)
    {
        return false;
    }

    if (iMonth < 1 || iMonth > 12)
    {
        return false;
    }

    int iMaxDay = 0;

    switch (iMonth)
    {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:
            iMaxDay = 31;
            break;

        case 4: case 6: case 9: case 11:
            iMaxDay = 30;
            break;

        case 2:
        {
            bool bIsLeap = (iYear % 400 == 0) ||
                           (iYear % 4 == 0 && iYear % 100 != 0);

            iMaxDay = bIsLeap ? 29 : 28;
            break;
        }

        default:
            return false;
    }

    if (iDay < 1 || iDay > iMaxDay)
    {
        return false;
    }

    return true;
}

// ================== HỖ TRỢ TÍNH KHOẢNG CÁCH NGÀY ==================

/************
* @Description Parse chuỗi ngày sang struct tm
* @param strDate Chuỗi ngày
* @param outTm Struct tm kết quả
* @return true nếu thành công
*************/
static bool parseDateDMY(const string &strDate, std::tm &outTm)
{
    if (!isValidDate(strDate))
    {
        return false;
    }

    int iDay   = stoi(strDate.substr(0, 2));
    int iMonth = stoi(strDate.substr(3, 2));
    int iYear  = stoi(strDate.substr(6, 4));

    std::tm tm{};
    tm.tm_mday = iDay;
    tm.tm_mon  = iMonth - 1;        // tm_mon: 0..11
    tm.tm_year = iYear - 1900;      // tm_year: từ 1900
    tm.tm_hour = 0;
    tm.tm_min  = 0;
    tm.tm_sec  = 0;

    outTm = tm;
    return true;
}

/************
* @Description Tính số ngày giữa 2 mốc thời gian
* @param dateFrom Ngày bắt đầu
* @param dateTo Ngày kết thúc
* @return Số ngày chênh lệch
*************/
int daysBetweenDates(const string &dateFrom, const string &dateTo)
{
    std::tm tmFrom{}, tmTo{};
    if (!parseDateDMY(dateFrom, tmFrom) || !parseDateDMY(dateTo, tmTo))
    {
        return 0;
    }

    std::time_t tFrom = std::mktime(&tmFrom);
    std::time_t tTo   = std::mktime(&tmTo);

    double diffSeconds = std::difftime(tTo, tFrom);
    int diffDays = static_cast<int>( diffSeconds / (60 * 60 * 24) );

    return diffDays;
}

/************
* @Description Tính số ngày từ hôm nay đến ngày chỉ định
* @param strDate Ngày đích
* @return Số ngày chênh lệch (âm nếu quá khứ)
*************/
int getDaysFromTodayToDate(const std::string &strDate)
{
    if (!isValidDate(strDate))
    {
        return 0;
    }

    int iDay   = std::stoi(strDate.substr(0, 2));
    int iMonth = std::stoi(strDate.substr(3, 2));
    int iYear  = std::stoi(strDate.substr(6, 4));

    // Ngày mục tiêu
    std::tm tmTarget = {};
    tmTarget.tm_mday = iDay;
    tmTarget.tm_mon  = iMonth - 1;      // tm_mon: 0..11
    tmTarget.tm_year = iYear - 1900;    // tm_year: từ 1900
    tmTarget.tm_hour = 0;
    tmTarget.tm_min  = 0;
    tmTarget.tm_sec  = 0;

    std::time_t tTarget = std::mktime(&tmTarget);

    // Hôm nay
    std::time_t tNow = std::time(nullptr);
    std::tm *pLocal = std::localtime(&tNow);
    if (!pLocal)
    {
        return 0;
    }

    std::tm tmToday = *pLocal;
    tmToday.tm_hour = 0;
    tmToday.tm_min  = 0;
    tmToday.tm_sec  = 0;

    std::time_t tToday = std::mktime(&tmToday);

    double diffSeconds = std::difftime(tTarget, tToday);
    int diffDays = static_cast<int>(diffSeconds / (60 * 60 * 24));

    return diffDays;
}

/************
* @Description Kiểm tra hợp lệ ngày giờ dd/mm/yyyy HH:MM
* @param strDateTime Chuỗi ngày giờ
* @return true nếu hợp lệ
*************/
bool isValidDateTime(const string &strDateTime)
{
    // Dinh dang: dd/mm/yyyy HH:MM -> 16 ky tu
    if (strDateTime.size() != 16)
        return false;

    string strDate = strDateTime.substr(0, 10);
    if (!isValidDate(strDate))
        return false;

    if (strDateTime[10] != ' ')
        return false;

    // HH:MM
    string strTime = strDateTime.substr(11, 5);
    if (strTime[2] != ':')
        return false;

    if (!isdigit((unsigned char)strTime[0]) ||
        !isdigit((unsigned char)strTime[1]) ||
        !isdigit((unsigned char)strTime[3]) ||
        !isdigit((unsigned char)strTime[4]))
    {
        return false;
    }

    int hour = stoi(strTime.substr(0, 2));
    int min  = stoi(strTime.substr(3, 2));

    if (hour < 0 || hour > 23) return false;
    if (min  < 0 || min  > 59) return false;

    return true;
}

// ================== HÀM THỜI GIAN / TIỆN ÍCH KHÁC ==================

/************
* @Description Dừng chương trình trong iSeconds giây
* @param iSeconds Số giây
*************/
void sleepSeconds(int iSeconds)
{
    if (iSeconds <= 0)
    {
        return;
    }

    // Đã sửa: Dùng hàm Sleep của Windows (tính bằng ms)
    Sleep(iSeconds * 1000);
}

/************
* @Description Lấy chuỗi thời gian hiện tại
* @return Chuỗi dd/mm/yyyy HH:MM:SS
*************/
string getCurrentTimeString()
{
    std::time_t tNow = std::time(nullptr);
    std::tm *pLocalTime = std::localtime(&tNow);

    if (pLocalTime == nullptr)
    {
        return "";
    }

    std::ostringstream oss;
    oss.fill('0');

    // dd/mm/yyyy HH:MM:SS
    oss << std::setw(2) << pLocalTime->tm_mday << "/"
        << std::setw(2) << (pLocalTime->tm_mon + 1) << "/"
        << std::setw(4) << (pLocalTime->tm_year + 1900) << " "
        << std::setw(2) << pLocalTime->tm_hour << ":"
        << std::setw(2) << pLocalTime->tm_min << ":"
        << std::setw(2) << pLocalTime->tm_sec;

    return oss.str();
}


// ================== HỖ TRỢ DI CHUYỂN CON TRỎ ==================

/************
* @Description Di chuyển con trỏ console tới toạ độ x,y
* @param x Toạ độ cột
* @param y Toạ độ dòng
*************/
void gotoXY(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = static_cast<SHORT>(x);
    pos.Y = static_cast<SHORT>(y);
    SetConsoleCursorPosition(hConsole, pos);
}

/************
* @Description Ẩn/Hiện con trỏ console
* @param bVisible true để hiện, false để ẩn
*************/
static void setCursorVisible(bool bVisible)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(hConsole, &info);
    info.bVisible = bVisible ? TRUE : FALSE;
    SetConsoleCursorInfo(hConsole, &info);
}


// ================== MÀN HÌNH LOADING KHI KHỞI ĐỘNG ==================

/************
* @Description Lấy chiều rộng console hiện tại
* @return Chiều rộng (số cột)
*************/
int getConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        // Nếu lỗi thì dùng tạm 100
        return 100;
    }

    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

/************
* @Description In text căn giữa màn hình
* @param y Dòng cần in
* @param text Nội dung text
* @param color Màu sắc
*************/
static void printCentered(int y, const std::string &text, int color = COLOR_DEFAULT)
{
    int width = getConsoleWidth();
    int x = (width - static_cast<int>(text.size())) / 2;
    if (x < 0) x = 0;

    gotoXY(x, y);
    if (color != COLOR_DEFAULT) setTextColor(color);
    std::cout << text;
    if (color != COLOR_DEFAULT) setTextColor(COLOR_DEFAULT);
}

// Vẽ chiếc máy bay banner tại vị trí (x, y)
static const int PLANE_WIDTH = 40;

/************
* @Description Vẽ hình máy bay ASCII tại toạ độ
* @param x Cột
* @param y Dòng
*************/
static void drawPlaneAt(int x, int y)
{
    gotoXY(x, y + 0);
    std::cout << "______";

    gotoXY(x, y + 1);
    std::cout << "_\\ _~-\\___";

    gotoXY(x, y + 2);
    std::cout << "=  = ==(____AA____D";

    gotoXY(x, y + 3);
    std::cout << "\\_____\\___________________,-~~~~~~~`-.._";

    gotoXY(x, y + 4);
    std::cout << "/     o O o o o o O O o o o o o o O o  |\\_";

    gotoXY(x, y + 5);
    std::cout << "`~-.__        ___..----..                  )";

    gotoXY(x, y + 6);
    std::cout << "      `---~~\\___________/------------`````";

    gotoXY(x, y + 7);
    std::cout << "      =   ===(_________D";
}

/************
* @Description Xóa vùng vẽ máy bay
* @param y Dòng bắt đầu xóa
*************/
static void clearPlaneArea(int y)
{
    const int PLANE_AREA_HEIGHT = 8;
    int width = getConsoleWidth();

    for (int row = 0; row < PLANE_AREA_HEIGHT; ++row)
    {
        gotoXY(0, y + row);
        std::cout << std::string(width, ' ');
    }
}

/************
* @Description Hiển thị màn hình Loading với hiệu ứng
*************/
void showLoadingScreen()
{
    clearScreen();
    setCursorVisible(false);

    int width = getConsoleWidth();

    // Chiều rộng thanh loading (tùy theo độ rộng màn hình)
    int BAR_WIDTH = width / 2;
    if (BAR_WIDTH > 60) BAR_WIDTH = 60;
    if (BAR_WIDTH < 30) BAR_WIDTH = 30;

    // Vị trí thanh loading (căn giữa)
    int BAR_X = (width - (BAR_WIDTH + 2)) / 2;  // +2 vì có 2 dấu [ ]
    int BAR_Y = 18;

    // Máy bay bay phía trên
    int PLANE_Y       = 5;
    int PLANE_START_X = 0;
    int PLANE_END_X   = width - PLANE_WIDTH - 1;
    if (PLANE_END_X < 0) PLANE_END_X = 0;

    int planeX = PLANE_START_X;

    // Tiêu đề
    printCentered(3, "HE THONG QUAN LY CHUYEN BAY", COLOR_HIGHLIGHT);
    printCentered(4, "Dang khoi dong, vui long cho...", COLOR_HIGHLIGHT);

    // Loading 0 -> 100, mỗi lần tăng 1%
    for (int percent = 0; percent <= 100; ++percent)
    {
        // ===== Vẽ thanh LOADING =====
        printCentered(BAR_Y - 2, "LOADING:");

        // Khung [    ]
        gotoXY(BAR_X, BAR_Y);
        std::cout << "[";

        gotoXY(BAR_X + BAR_WIDTH + 1, BAR_Y);
        std::cout << "]";

        // Phần đã fill
        int filled = BAR_WIDTH * percent / 100;
        gotoXY(BAR_X + 1, BAR_Y);

        for (int i = 0; i < BAR_WIDTH; ++i)
        {
            if (i < filled)
            {
                setTextColor(COLOR_SUCCESS);
                std::cout << (char)219;
                setTextColor(COLOR_DEFAULT);
            }
            else
            {
                std::cout << ' ';
            }
        }

        // Hiển thị % bên phải
        gotoXY(BAR_X + BAR_WIDTH + 5, BAR_Y);
        std::cout << std::setw(3) << percent << " %";

        // ===== Máy bay bay nhanh, độc lập với % =====
        clearPlaneArea(PLANE_Y);

        // Mỗi frame bay thêm 3 cột (nhanh hơn tiến độ %)
        planeX += 3;
        if (planeX > PLANE_END_X)
        {
            // Khi bay tới sát mép phải thì quay lại từ bên trái
            planeX = PLANE_START_X;
        }

        setTextColor(COLOR_HIGHLIGHT);
        drawPlaneAt(planeX, PLANE_Y);
        setTextColor(COLOR_DEFAULT);

        std::cout.flush();

        // Loading chậm: 80ms / 1%
        // Đã sửa: Dùng hàm Sleep (ms) thay cho std::this_thread
        Sleep(80);
    }

    // Hoàn tất
    clearPlaneArea(PLANE_Y);
    printCentered(BAR_Y + 2,
                  "Khoi dong hoan tat! Nhan phim bat ky de tiep tuc...",
                  COLOR_SUCCESS);

    _getch();
    setCursorVisible(true);
    clearScreen();
}