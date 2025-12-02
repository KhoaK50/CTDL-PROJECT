#pragma once

#include <string>
#include <vector>

// ================== HÀM XỬ LÝ CONSOLE ==================

/************
* @Description Xóa toàn bộ nội dung màn hình console
*************/
void clearScreen();

/************
* @Description Tạm dừng chương trình, chờ người dùng bấm phím bất kỳ (thường là Enter)
*************/
void pauseScreen();

/************
* @Description Đổi màu chữ trong Console (Windows)
* @param iColorCode Mã màu (Ví dụ: COLOR_ERROR, COLOR_SUCCESS...)
*************/
void setTextColor(int iColorCode);


// ================== HÀM NHẬP LIỆU AN TOÀN ==================

/************
* @Description Nhập số nguyên trong khoảng [iMin, iMax], lặp lại nếu nhập sai
* @param strPrompt Câu nhắc nhập liệu
* @param iMin Giá trị nhỏ nhất chấp nhận
* @param iMax Giá trị lớn nhất chấp nhận
* @return Số nguyên hợp lệ
*************/
int inputIntInRange(const std::string &strPrompt, int iMin, int iMax);

/************
* @Description Nhập chuỗi không rỗng, tự động cắt khoảng trắng thừa 2 đầu
* @param strPrompt Câu nhắc nhập liệu
* @return Chuỗi đã chuẩn hóa
*************/
std::string inputNonEmptyString(const std::string &strPrompt);

/************
* @Description Nhập mật khẩu (hiển thị ký tự '*' thay vì ký tự thật)
* @param strPrompt Câu nhắc nhập liệu
* @return Chuỗi mật khẩu
*************/
std::string inputPassword(const std::string &strPrompt);

/************
* @Description Nhập ngày hợp lệ dạng "dd/mm/yyyy", lặp lại đến khi đúng format và logic
* @param strPrompt Câu nhắc nhập liệu
* @return Chuỗi ngày hợp lệ
*************/
std::string inputValidDate(const std::string &strPrompt);

/************
* @Description Kiểm tra chuỗi ngày giờ có đúng định dạng "dd/mm/yyyy HH:MM" hay không
* @param strDateTime Chuỗi ngày giờ cần kiểm tra
* @return true nếu hợp lệ
*************/
bool isValidDateTime(const std::string &strDateTime);

/************
* @Description Nhập ngày giờ hợp lệ dạng "dd/mm/yyyy HH:MM"
* @param strPrompt Câu nhắc nhập liệu
* @return Chuỗi ngày giờ hợp lệ
*************/
std::string inputValidDateTime(const std::string &strPrompt);

/************
* @Description Nhập họ tên hợp lệ (chỉ chữ + khoảng trắng, >= 2 ký tự, tự động UPPERCASE)
* @param strPrompt Câu nhắc nhập liệu
* @return Họ tên đã chuẩn hóa
*************/
std::string inputValidFullName(const std::string &strPrompt);

/************
* @Description Nhập CMND/CCCD hợp lệ (9 hoặc 12 số, chỉ chứa số)
* @param strPrompt Câu nhắc nhập liệu
* @return Chuỗi CMND/CCCD hợp lệ
*************/
std::string inputValidIDNumber(const std::string &strPrompt);

/************
* @Description Nhập mã chuyến bay hợp lệ (không rỗng, không khoảng trắng, tự động UPPERCASE)
* @param strPrompt Câu nhắc nhập liệu
* @return Mã chuyến bay chuẩn
*************/
std::string inputValidFlightId(const std::string &strPrompt);

/************
* @Description Nhập số nguyên bằng chuỗi, kiểm tra chặt chẽ ký tự và khoảng giá trị
* @param strPrompt Câu nhắc nhập liệu
* @param iMin Giá trị min
* @param iMax Giá trị max
* @return Số nguyên hợp lệ
*************/
int inputIntStrict(const std::string &strPrompt, int iMin, int iMax);


// ================== HÀM XỬ LÝ CHUỖI / NGÀY ==================

/************
* @Description Cắt khoảng trắng ở đầu và cuối chuỗi
* @param strInput Chuỗi đầu vào
* @return Chuỗi đã cắt khoảng trắng
*************/
std::string trim(const std::string &strInput);

/************
* @Description Đổi toàn bộ chuỗi sang chữ in hoa
* @param strInput Chuỗi đầu vào
* @return Chuỗi in hoa
*************/
std::string toUpper(const std::string &strInput);

/************
* @Description Kiểm tra ngày dạng "dd/mm/yyyy" có hợp lệ về mặt logic (năm nhuận, số ngày trong tháng)
* @param strDate Chuỗi ngày
* @return true nếu hợp lệ
*************/
bool isValidDate(const std::string &strDate);


// ================== HÀM THỜI GIAN / TIỆN ÍCH KHÁC ==================

/************
* @Description Tạm dừng chương trình trong một khoảng thời gian (delay)
* @param iSeconds Số giây cần dừng
*************/
void sleepSeconds(int iSeconds);

/************
* @Description Lấy thời gian hiện tại của hệ thống
* @return Chuỗi thời gian dạng "dd/mm/yyyy HH:MM:SS"
*************/
std::string getCurrentTimeString();

/************
* @Description Tính số ngày chênh lệch giữa hai mốc thời gian (dd/mm/yyyy)
* @param dateFrom Ngày bắt đầu
* @param dateTo Ngày kết thúc
* @return Số ngày (dateTo - dateFrom). Dương nếu To > From, Âm nếu ngược lại.
*************/
int daysBetweenDates(const std::string &dateFrom, const std::string &dateTo);

/************
* @Description Tính số ngày từ hôm nay đến ngày được truyền vào
* @param strDate Ngày mốc (dd/mm/yyyy)
* @return > 0 (Tương lai), = 0 (Hôm nay), < 0 (Quá khứ)
*************/
int getDaysFromTodayToDate(const std::string &strDate);


// ================== TEMPLATE FUNCTION DÙNG CHUNG ==================

/************
* @Description Tìm phần tử trong vector thỏa mãn điều kiện (Non-const version)
* @param vItems Vector dữ liệu
* @param fnPred Hàm điều kiện (Predicate)
* @return Con trỏ đến phần tử tìm thấy hoặc nullptr
*************/
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

/************
* @Description Tìm phần tử trong vector thỏa mãn điều kiện (Const version)
* @param vItems Vector dữ liệu (const)
* @param fnPred Hàm điều kiện (Predicate)
* @return Con trỏ const đến phần tử tìm thấy hoặc nullptr
*************/
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

/************
* @Description Di chuyển con trỏ văn bản tới tọa độ (x, y) trong console
* @param x Tọa độ cột
* @param y Tọa độ dòng
*************/
void gotoXY(int x, int y);

/************
* @Description Hiển thị màn hình Loading giả lập khi khởi động
*************/
void showLoadingScreen();