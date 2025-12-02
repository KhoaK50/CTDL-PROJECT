#pragma once

#include <string>


class Ticket
{
private:
    std::string _strTicketId;      
    std::string _strFlightId;      
    std::string _strCustomerId;    
    std::string _strCustomerName;
    int         _iSeatNumber;      

public:
    // ========================= Constructors =========================

    /************
    * @Description Constructor mặc định
    *************/
    Ticket();

    /************
    * @Description Constructor đầy đủ tham số
    * @param strFlightId Mã chuyến bay
    * @param strCustomerId Mã khách hàng
    * @param strCustomerName Tên khách hàng
    * @param iSeatNumber Số ghế
    *************/
    Ticket(const std::string &strFlightId,
           const std::string &strCustomerId,
           const std::string &strCustomerName,
           int iSeatNumber);

    /************
    * @Description Copy Constructor
    * @param src Vé nguồn
    *************/
    Ticket(const Ticket &src);

    /************
    * @Description Destructor
    *************/
    ~Ticket();

    // ========================= Setters =========================

    /************
    * @Description Thiết lập mã chuyến bay
    * @param strFlightId Mã chuyến bay mới
    *************/
    void setFlightId(const std::string &strFlightId);

    /************
    * @Description Thiết lập mã khách hàng
    * @param strCustomerId Mã khách hàng mới
    *************/
    void setCustomerId(const std::string &strCustomerId);

    /************
    * @Description Thiết lập tên khách hàng
    * @param strCustomerName Tên khách hàng mới
    *************/
    void setCustomerName(const std::string &strCustomerName);

    /************
    * @Description Thiết lập số ghế
    * @param iSeatNumber Số ghế mới
    *************/
    void setSeatNumber(int iSeatNumber);

    // ========================= Getters =========================

    /************
    * @Description Lấy mã vé
    * @return Mã vé (string)
    *************/
    std::string getTicketId() const;

    /************
    * @Description Lấy mã chuyến bay
    * @return Mã chuyến bay (string)
    *************/
    std::string getFlightId() const;

    /************
    * @Description Lấy mã khách hàng
    * @return Mã khách hàng (string)
    *************/
    std::string getCustomerId() const;

    /************
    * @Description Lấy tên khách hàng
    * @return Tên khách hàng (string)
    *************/
    std::string getCustomerName() const;

    /************
    * @Description Lấy số ghế
    * @return Số ghế (int)
    *************/
    int getSeatNumber() const;

    // ========================= Business & Utils =========================

    /************
    * @Description Tự động tạo mã vé dựa trên Mã chuyến bay + Số ghế
    * Format: [FlightId]-[SeatNumber]
    *************/
    void makeTicketId();

    /************
    * @Description In thông tin vé ra màn hình
    *************/
    void print() const;

    // ========================= Operators =========================

    /************
    * @Description So sánh bằng (dùng cho tìm kiếm trong LinkedList/Queue)
    * @param other Vé cần so sánh
    * @return true nếu trùng mã vé (hoặc trùng chuyến bay + số ghế)
    *************/
    bool operator==(const Ticket &other) const;

    /************
    * @Description So sánh nhỏ hơn (dùng để sort vé theo số ghế)
    * @param other Vé cần so sánh
    * @return true nếu số ghế của vé này nhỏ hơn vé kia
    *************/
    bool operator<(const Ticket &other) const;
};