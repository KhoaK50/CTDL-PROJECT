#pragma once

#include <string>
#include <vector>

// ================== HÀM XỬ LÝ CONSOLE ==================

// Xóa màn hình
void clearScreen();

// Tạm dừng, chờ người dùng bấm Enter
void pauseScreen();

// Đổi màu chữ (dùng trong Windows console)
void setTextColor(int iColorCode);


// ================== HÀM NHẬP LIỆU AN TOÀN ==================

// Nhập 1 số nguyên trong [iMin, iMax], lặp lại nếu sai
int inputIntInRange(const std::string &strPrompt, int iMin, int iMax);

// Nhập chuỗi không rỗng, tự động trim 2 đầu
std::string inputNonEmptyString(const std::string &strPrompt);

// Nhập mật khẩu, hiển thị ký tự '*' thay vì ký tự thật
std::string inputPassword(const std::string &strPrompt);

// Nhập ngày hợp lệ dạng "dd/mm/yyyy", lặp lại đến khi đúng
std::string inputValidDate(const std::string &strPrompt);

// Kiểm tra ngày giờ dạng "dd/mm/yyyy HH:MM"
bool isValidDateTime(const std::string &strDateTime);

// Nhập ngày giờ hợp lệ "dd/mm/yyyy HH:MM"
std::string inputValidDateTime(const std::string &strPrompt);

// Nhập họ tên hợp lệ (chỉ chữ + khoảng trắng, >= 2 ký tự, auto UPPER)
std::string inputValidFullName(const std::string &strPrompt);

// Nhập CMND/CCCD hợp lệ (CMND = 9 số, CCCD = 12 số, chỉ được chứa số)
std::string inputValidIDNumber(const std::string &strPrompt);

// Nhập mã chuyến bay hợp lệ (không rỗng, không khoảng trắng, auto UPPER)
std::string inputValidFlightId(const std::string &strPrompt);

// Nhập số nguyên bằng chuỗi, kiểm tra chặt + báo lỗi đỏ (min..max)
int inputIntStrict(const std::string &strPrompt, int iMin, int iMax);


// ================== HÀM XỬ LÝ CHUỖI / NGÀY ==================

// Cắt khoảng trắng ở đầu và cuối chuỗi
std::string trim(const std::string &strInput);

// Đổi chuỗi sang in hoa
std::string toUpper(const std::string &strInput);

// Kiểm tra ngày dạng "dd/mm/yyyy" có hợp lệ hay không (mức cơ bản)
bool isValidDate(const std::string &strDate);


// ================== HÀM THỜI GIAN / TIỆN ÍCH KHÁC ==================

// Tạm dừng chương trình iSeconds giây (dùng cho delay, hiệu ứng,...)
void sleepSeconds(int iSeconds);

// Lấy thời gian hiện tại dưới dạng chuỗi "dd/mm/yyyy HH:MM:SS"
std::string getCurrentTimeString();

// Trả về số ngày giữa dateFrom -> dateTo (dd/mm/yyyy).
// Nếu dateTo > dateFrom thì > 0, nếu dateTo == dateFrom thì = 0, nếu dateTo < dateFrom thì < 0.
int daysBetweenDates(const std::string &dateFrom, const std::string &dateTo);

// Trả về số ngày từ hôm nay tới strDate (dd/mm/yyyy).
// > 0 : còn trong tương lai
//   0 : hôm nay
// < 0 : ngày trong quá khứ
int getDaysFromTodayToDate(const std::string &strDate);


// ================== TEMPLATE FUNCTION DÙNG CHUNG ==================

// Tìm phần tử trong vector thỏa điều kiện pred. Trả về con trỏ nếu tìm thấy.

template <typename DataType, typename Pred>
DataType *findIf(std::vector<DataType> &vItems, Pred fnPred)
{
    for (auto &item : vItems)
    {
        if (fnPred(item))
        {
            return &item;
        }
    }
    return nullptr;
}

template <typename DataType, typename Pred>
const DataType *findIf(const std::vector<DataType> &vItems, Pred fnPred)
{
    for (const auto &item : vItems)
    {
        if (fnPred(item))
        {
            return &item;
        }
    }
    return nullptr;
}


// ================== HÀM VẼ GIAO DIỆN ĐẶC BIỆT ==================

// Di chuyển con trỏ tới vị trí (x, y) trong console
void gotoXY(int x, int y);

// Màn hình loading khi khởi động chương trình
void showLoadingScreen();
