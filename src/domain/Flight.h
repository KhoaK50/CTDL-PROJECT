#pragma once

#include <string>
#include "../core/Config.h"
#include "../core/SeatList.h"
#include "../data_structure/LinkedList.h"
#include "Ticket.h"

/************
* @Description Lớp đại diện cho một chuyến bay
* Quản lý thông tin chuyến bay, danh sách ghế và vé
*************/
class Flight
{
private:
    std::string _strFlightId;       
    std::string _strPlaneId;        
    std::string _strDate;           
    std::string _strDepartTime;     
    std::string _strDestination;    

    FlightStatus _eStatus;          
    SeatList     _SeatList;         
    LinkedList<Ticket> _ListTickets;

public:
    // ========================= Constructors =========================

    /************
    * @Description Constructor mặc định
    *************/
    Flight();

    /************
    * @Description Constructor đầy đủ tham số
    * @param strFlightId Mã chuyến bay
    * @param strPlaneId Mã máy bay
    * @param strDate Ngày khởi hành
    * @param strDestination Sân bay đến
    * @param eStatus Trạng thái chuyến bay
    * @param iTotalSeats Tổng số ghế (để khởi tạo danh sách ghế)
    *************/
    Flight(const std::string &strFlightId,
           const std::string &strPlaneId,
           const std::string &strDate,
           const std::string &strDestination,
           FlightStatus eStatus,
           int iTotalSeats);

    /************
    * @Description Copy Constructor
    * @param other Chuyến bay nguồn
    *************/
    Flight(const Flight &other);

    /************
    * @Description Destructor
    *************/
    ~Flight();

    // ========================= Setters =========================

    /************
    * @Description Thiết lập mã chuyến bay
    * @param strFlightId Mã chuyến bay mới
    *************/
    void setFlightId(const std::string &strFlightId);

    /************
    * @Description Thiết lập mã máy bay
    * @param strPlaneId Mã máy bay mới
    *************/
    void setPlaneId(const std::string &strPlaneId);

    /************
    * @Description Thiết lập ngày khởi hành
    * @param strDate Ngày khởi hành (dd/mm/yyyy)
    *************/
    void setDate(const std::string &strDate);

    /************
    * @Description Thiết lập giờ khởi hành
    * @param strDepartTime Giờ khởi hành (HH:MM)
    *************/
    void setDepartTime(const std::string &strDepartTime);

    /************
    * @Description Thiết lập sân bay đến
    * @param strDestination Địa điểm đến
    *************/
    void setDestination(const std::string &strDestination);

    /************
    * @Description Thiết lập trạng thái chuyến bay
    * @param eStatus Trạng thái mới (HUY_CHUYEN, CON_VE...)
    *************/
    void setStatus(FlightStatus eStatus);

    // ========================= Getters =========================

    /************
    * @Description Lấy mã chuyến bay
    * @return Mã chuyến bay (string)
    *************/
    std::string getFlightId() const;

    /************
    * @Description Lấy mã máy bay
    * @return Mã máy bay (string)
    *************/
    std::string getPlaneId() const;

    /************
    * @Description Lấy ngày khởi hành
    * @return Ngày khởi hành (string)
    *************/
    std::string getDate() const;

    /************
    * @Description Lấy giờ khởi hành
    * @return Giờ khởi hành (string)
    *************/
    std::string getDepartTime() const;

    /************
    * @Description Lấy sân bay đến
    * @return Địa điểm đến (string)
    *************/
    std::string getDestination() const;

    /************
    * @Description Lấy trạng thái chuyến bay (enum)
    * @return Trạng thái hiện tại
    *************/
    FlightStatus getStatus() const;

    /************
    * @Description Lấy chuỗi mô tả trạng thái (dùng để hiển thị)
    * @return Chuỗi trạng thái ("Huy chuyen", "Con ve"...)
    *************/
    std::string getStatusString() const;

    // ========================= Seat Info =========================

    /************
    * @Description Lấy tổng số ghế
    * @return Tổng số ghế (int)
    *************/
    int getTotalSeatCount() const;

    /************
    * @Description Lấy số ghế còn trống
    * @return Số ghế trống (int)
    *************/
    int getFreeSeatCount() const;

    /************
    * @Description Lấy số ghế đã đặt
    * @return Số ghế đã đặt (int)
    *************/
    int getBookedSeatCount() const;

    /************
    * @Description Lấy danh sách số ghế đã đặt dưới dạng chuỗi
    * @return Chuỗi danh sách (Ví dụ: "2, 3, 5")
    *************/
    std::string getBookedSeatListString() const;

    // ========================= Checkers =========================

    /************
    * @Description Kiểm tra chuyến bay có bị hủy không
    * @return true nếu đã hủy
    *************/
    bool isCancelled() const;

    /************
    * @Description Kiểm tra chuyến bay đã đầy chỗ chưa
    * @return true nếu đã full
    *************/
    bool isFull() const;

    /************
    * @Description Kiểm tra chuyến bay có khả dụng để đặt vé không
    * (Không hủy và còn chỗ)
    * @return true nếu khả dụng
    *************/
    bool isAvailable() const;

    // ========================= Business Logic =========================

    /************
    * @Description Đặt vé cho khách hàng
    * @param strCustomerId CMND/CCCD khách hàng
    * @param strCustomerName Tên khách hàng
    * @param iSeatNumber Số ghế muốn đặt
    * @return true nếu đặt thành công, false nếu thất bại
    *************/
    bool bookTicket(const std::string &strCustomerId,
                    const std::string &strCustomerName,
                    int iSeatNumber);

    /************
    * @Description Hủy vé theo số ghế
    * @param iSeatNumber Số ghế cần hủy
    * @return true nếu hủy thành công
    *************/
    bool cancelTicketBySeat(int iSeatNumber);

    // ========================= Display =========================

    /************
    * @Description In thông tin tóm tắt (dùng trong danh sách)
    *************/
    void print() const;

    /************
    * @Description In chi tiết chuyến bay, danh sách ghế và vé
    *************/
    void printDetail() const;
};