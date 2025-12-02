#pragma once

#include <vector>

class SeatList
{
private:
    int                 _iTotalSeats;
    std::vector<bool>   _vSeatFree;

public:
    /************
    * @Description Constructor mặc định
    *************/
    SeatList();

    /************
    * @Description Constructor khởi tạo với số lượng ghế
    * @param iTotalSeats Tổng số ghế
    *************/
    explicit SeatList(int iTotalSeats);

    /************
    * @Description Destructor
    *************/
    ~SeatList();

    /************
    * @Description Khởi tạo / reset danh sách ghế theo tổng số ghế
    * @param iTotalSeats Số lượng ghế cần khởi tạo
    *************/
    void init(int iTotalSeats);

    // ========================= Getters / Checkers =========================

    /************
    * @Description Lấy tổng số ghế
    * @return Tổng số ghế (int)
    *************/
    int getTotalSeats() const;

    /************
    * @Description Kiểm tra số ghế có hợp lệ không (nằm trong khoảng 1..Total)
    * @param iSeatNumber Số ghế cần kiểm tra
    * @return true nếu hợp lệ
    *************/
    bool isSeatValid(int iSeatNumber) const;

    /************
    * @Description Kiểm tra ghế có đang trống không
    * @param iSeatNumber Số ghế cần kiểm tra
    * @return true nếu ghế trống
    *************/
    bool isSeatFree(int iSeatNumber) const;

    /************
    * @Description Đếm số lượng ghế còn trống
    * @return Số lượng ghế trống
    *************/
    int countFreeSeats() const;

    // ========================= Actions =========================

    /************
    * @Description Đặt ghế (chuyển trạng thái sang đã đặt)
    * @param iSeatNumber Số ghế muốn đặt
    * @return true nếu đặt thành công (ghế hợp lệ và đang trống)
    *************/
    bool bookSeat(int iSeatNumber);

    /************
    * @Description Hủy đặt ghế (chuyển trạng thái sang trống)
    * @param iSeatNumber Số ghế muốn hủy
    * @return true nếu hủy thành công
    *************/
    bool cancelSeat(int iSeatNumber);

    // ========================= Display =========================

    /************
    * @Description In danh sách các số ghế đang trống
    *************/
    void printFreeSeats() const;

    /************
    * @Description In sơ đồ ghế trực quan (Ma trận ghế)
    *************/
    void printSeatMap() const;
};