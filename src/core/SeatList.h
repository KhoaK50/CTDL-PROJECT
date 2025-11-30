#pragma once

#include <vector>

class SeatList
{
private:
    int                 _iTotalSeats;
    std::vector<bool>   _vSeatFree;

public:
    SeatList();
    explicit SeatList(int iTotalSeats);
    ~SeatList();

    // Khởi tạo / reset danh sách ghế theo tổng số ghế
    void init(int iTotalSeats);

    // Thông tin cơ bản
    int  getTotalSeats() const;
    bool isSeatValid(int iSeatNumber) const;
    bool isSeatFree(int iSeatNumber) const;
    int  countFreeSeats() const;

    // Đặt / hủy ghế
    bool bookSeat(int iSeatNumber);
    bool cancelSeat(int iSeatNumber);

    // In thông tin ghế (debug / hiển thị)
    void printFreeSeats() const;
    void printSeatMap() const;
};
