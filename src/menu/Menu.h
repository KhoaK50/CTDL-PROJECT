#pragma once

#include "../core/Repository.h"

// Lớp chịu trách nhiệm:
// - In menu
// - Nhận lựa chọn từ người dùng
// - Gọi Repository để xử lý dữ liệu

class Menu
{
private:
    Repository &_repo;   // Tham chiếu tới kho dữ liệu dùng chung

    // ===== MENU CHÍNH =====
    void showMainMenu() const;
    void handleMainChoice(int iChoice);

    // ===== ĐĂNG NHẬP & MENU QUẢN LÝ =====
    void loginAndManagementMenu();

    void showManagementMenu() const;
    void handleManagementChoice(int iChoice);

    // ===== MENU THỐNG KÊ =====
    void showStatisticMenu() const;
    void handleStatisticChoice(int iChoice);

    // ===== CÁC CHỨC NĂNG NGHIỆP VỤ =====
    void displayAllFlights();           // Hiển thị danh sách chuyến bay
    void userBookTicket();              // Người dùng đặt vé
    void adminProcessBooking();         // Admin xử lý đặt vé
    void adminCancelTicket();           // Admin xử lý trả vé

    void statisticsByFlight();          // Thống kê khách của 1 chuyến bay
    void statisticsFreeSeats();         // Thống kê ghế trống
    void statisticsPlaneUsage();        // Thống kê số lần máy bay thực hiện chuyến

    void undoLastBooking();             // Undo booking gần nhất (stack)

public:
    Menu(Repository &repo);

    // Vòng lặp chính của chương trình
    void run();
};
