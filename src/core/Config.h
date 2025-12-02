#pragma once

#include <string>

// ====================== TRANG THAI CHUYEN BAY ======================

/************
* @Description Enum định nghĩa các trạng thái của một chuyến bay
*************/
enum FlightStatus
{
    FLIGHT_CANCELLED = 0,   // Huy chuyen
    FLIGHT_AVAILABLE = 1,   // Con ve
    FLIGHT_FULL      = 2,   // Het ve
    FLIGHT_COMPLETED = 3    // Da hoan tat
};

// ====================== DUONG DAN FILE DU LIEU ======================

/************
* @Description Định nghĩa đường dẫn tuyệt đối/tương đối đến các file dữ liệu
* Dùng #define để tránh warning "unused variable" khi include ở nhiều nơi
*************/
#define PATH_MAYBAY        "data/MayBay.txt"
#define PATH_CHUYENBAY     "data/ChuyenBay.txt"
#define PATH_KHACHHANG     "data/KhachHang.txt"
#define PATH_ADMIN         "data/Admin.txt"
#define PATH_TICKET_DIR    "data/tickets/"
#define PATH_VE_DANG_CHO   "data/VeDangCho.txt"
#define PATH_VE_THANH_CONG "data/VeThanhCong.txt"

// ====================== MAU CONSOLE ======================

/************
* @Description Các mã màu cơ bản cho Console (Windows API)
*************/
const int COLOR_DEFAULT   = 7;   // trang
const int COLOR_SUCCESS   = 10;  // xanh la
const int COLOR_WARNING   = 14;  // vang
const int COLOR_ERROR     = 12;  // do

/************
* @Description Các mã màu dùng cho giao diện (UI Theme)
*************/
const int COLOR_HIGHLIGHT   = 11; // cyan tong quat neu can
const int COLOR_TITLE       = 11; // tieu de lon (cyan)
const int COLOR_HEADER      = 14; // header bang (vang)
const int COLOR_MENU_FRAME  = 11; // khung menu (cyan)
const int COLOR_MENU_TEXT   = 10; // noi dung menu (xanh la)

// ====================== NHAP MAT KHAU ======================

/************
* @Description Ký tự dùng để che mật khẩu khi nhập
*************/
const char PASSWORD_MASK_CHAR = '*';