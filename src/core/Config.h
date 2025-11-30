#pragma once

#include <string>

// ====================== TRANG THAI CHUYEN BAY ======================

enum FlightStatus
{
    FLIGHT_CANCELLED = 0,   // Huy chuyen
    FLIGHT_AVAILABLE = 1,   // Con ve
    FLIGHT_FULL      = 2,   // Het ve
    FLIGHT_COMPLETED = 3    // Da hoan tat
};

// ====================== DUONG DAN FILE DU LIEU ======================

inline constexpr const char* PATH_MAYBAY        = "data/MayBay.txt";
inline constexpr const char* PATH_CHUYENBAY     = "data/ChuyenBay.txt";
inline constexpr const char* PATH_KHACHHANG     = "data/KhachHang.txt";
inline constexpr const char* PATH_ADMIN         = "data/Admin.txt";
inline constexpr const char* PATH_TICKET_DIR    = "data/tickets/";
inline constexpr const char* PATH_VE_DANG_CHO   = "data/VeDangCho.txt";
inline constexpr const char* PATH_VE_THANH_CONG = "data/VeThanhCong.txt";

// ====================== MAU CONSOLE ======================

// Mac dinh
const int COLOR_DEFAULT   = 7;   // trang
const int COLOR_SUCCESS   = 10;  // xanh la
const int COLOR_WARNING   = 14;  // vang
const int COLOR_ERROR     = 12;  // do

// Dung cho giao dien
const int COLOR_HIGHLIGHT   = 11; // cyan tong quat neu can
const int COLOR_TITLE       = 11; // tieu de lon (cyan)
const int COLOR_HEADER      = 14; // header bang (vang)
const int COLOR_MENU_FRAME  = 11; // khung menu (cyan)
const int COLOR_MENU_TEXT   = 10; // noi dung menu (xanh la)

// ====================== NHAP MAT KHAU ======================

const char PASSWORD_MASK_CHAR = '*';
