#pragma once

#include "../core/Repository.h"

/************
* @Description Lớp Menu chịu trách nhiệm giao diện console
* In menu, nhận input và gọi Repository xử lý
*************/
class Menu
{
private:
    Repository &_repo;   // Tham chiếu tới kho dữ liệu dùng chung

    // ========================= MAIN MENU =========================

    /************
    * @Description Hiển thị các tùy chọn của Menu chính
    *************/
    void showMainMenu() const;

    /************
    * @Description Xử lý lựa chọn từ người dùng ở Menu chính
    * @param iChoice Số thứ tự chức năng người dùng chọn
    *************/
    void handleMainChoice(int iChoice);

    // ========================= AUTH & MANAGEMENT =========================

    /************
    * @Description Điều hướng quy trình đăng nhập
    * Nếu đăng nhập thành công sẽ chuyển sang Menu quản lý
    *************/
    void loginAndManagementMenu();

    /************
    * @Description Hiển thị Menu quản lý (dành cho Admin)
    *************/
    void showManagementMenu() const;

    /************
    * @Description Xử lý lựa chọn ở Menu quản lý
    * @param iChoice Số thứ tự chức năng Admin chọn
    *************/
    void handleManagementChoice(int iChoice);

    // ========================= STATISTICS =========================

    /************
    * @Description Hiển thị Menu thống kê
    *************/
    void showStatisticMenu() const;

    /************
    * @Description Xử lý lựa chọn ở Menu thống kê
    * @param iChoice Số thứ tự chức năng thống kê
    *************/
    void handleStatisticChoice(int iChoice);

    // ========================= BUSINESS FUNCTIONS =========================

    /************
    * @Description Hiển thị danh sách tất cả chuyến bay
    *************/
    void displayAllFlights();

    /************
    * @Description Chức năng đặt vé cho người dùng
    *************/
    void userBookTicket();

    /************
    * @Description Admin thực hiện đặt vé/xử lý vé
    *************/
    void adminProcessBooking();

    /************
    * @Description Admin thực hiện hủy/trả vé
    *************/
    void adminCancelTicket();

    /************
    * @Description Thống kê hành khách của một chuyến bay cụ thể
    *************/
    void statisticsByFlight();

    /************
    * @Description Thống kê số lượng ghế trống
    *************/
    void statisticsFreeSeats();

    /************
    * @Description Thống kê tần suất hoạt động của máy bay
    *************/
    void statisticsPlaneUsage();

    /************
    * @Description Hủy thao tác đặt vé gần nhất (Undo dùng Stack)
    *************/
    void undoLastBooking();

public:
    // ========================= Constructor & Run =========================

    /************
    * @Description Constructor nhận tham chiếu Repository
    * @param repo Đối tượng kho dữ liệu
    *************/
    Menu(Repository &repo);

    /************
    * @Description Vòng lặp chính của chương trình
    * Giữ chương trình chạy cho đến khi người dùng chọn Thoát
    *************/
    void run();
};