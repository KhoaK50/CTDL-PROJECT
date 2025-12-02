#include "core/SeatList.h"
#include "core/Config.h"   // COLOR_*
#include "utils/Utils.h"   // setTextColor()

#include <iostream>
#include <iomanip>
#include <sstream>     
#include <algorithm>

using std::cout;
using std::endl;
using std::setw;
using std::setfill;

// ========================= Constructor / Destructor =========================

/************
* @Description Constructor mặc định
*************/
SeatList::SeatList()
    : _iTotalSeats(0), _vSeatFree()
{
}

/************
* @Description Constructor khởi tạo với số lượng ghế
* @param iTotalSeats Tổng số ghế
*************/
SeatList::SeatList(int iTotalSeats)
    : _iTotalSeats(0), _vSeatFree()
{
    this->init(iTotalSeats);
}

/************
* @Description Destructor
*************/
SeatList::~SeatList()
{
}

// ============================== Init / Basic ==============================

/************
* @Description Khởi tạo danh sách ghế
*************/
void SeatList::init(int iTotalSeats)
{
    if (iTotalSeats > 0)
    {
        this->_iTotalSeats = iTotalSeats;
        this->_vSeatFree.assign(iTotalSeats, true);
    }
    else
    {
        this->_iTotalSeats = 0;
        this->_vSeatFree.clear();
    }
}

/************
* @Description Lấy tổng số ghế
*************/
int SeatList::getTotalSeats() const
{
    return this->_iTotalSeats;
}

/************
* @Description Kiểm tra số ghế có hợp lệ không
*************/
bool SeatList::isSeatValid(int iSeatNumber) const
{
    return (iSeatNumber >= 1 && iSeatNumber <= this->_iTotalSeats);
}

/************
* @Description Kiểm tra ghế có trống không
*************/
bool SeatList::isSeatFree(int iSeatNumber) const
{
    if (!this->isSeatValid(iSeatNumber))
    {
        return false;
    }

    return this->_vSeatFree[iSeatNumber - 1];
}

/************
* @Description Đếm số ghế trống
*************/
int SeatList::countFreeSeats() const
{
    int iCount = 0;

    for (bool bIsFree : this->_vSeatFree)
    {
        if (bIsFree)
        {
            iCount++;
        }
    }

    return iCount;
}

// ============================ Đặt / Hủy ghế ============================

/************
* @Description Đặt ghế
*************/
bool SeatList::bookSeat(int iSeatNumber)
{
    if (!this->isSeatValid(iSeatNumber) || !this->isSeatFree(iSeatNumber))
    {
        return false;
    }

    this->_vSeatFree[iSeatNumber - 1] = false;
    return true;
}

/************
* @Description Hủy đặt ghế
*************/
bool SeatList::cancelSeat(int iSeatNumber)
{
    if (!this->isSeatValid(iSeatNumber))
    {
        return false;
    }

    if (this->_vSeatFree[iSeatNumber - 1])
    {
        // Ghế đang trống, không thể hủy
        return false;
    }

    this->_vSeatFree[iSeatNumber - 1] = true;
    return true;
}

// ========================== In danh sách ghế ==========================

/************
* @Description In danh sách các số ghế đang trống
*************/
void SeatList::printFreeSeats() const
{
    cout << "Cac ghe con trong: ";

    for (int iIndex = 0; iIndex < this->_iTotalSeats; ++iIndex)
    {
        if (this->_vSeatFree[iIndex])
        {
            cout << (iIndex + 1) << " ";
        }
    }

    cout << endl;
}

/************
* @Description In sơ đồ ghế trực quan
*************/
void SeatList::printSeatMap() const
{
    const int iSeatsPerRow    = 6;  // mỗi hàng 6 ghế
    const int iCellInnerWidth = 5;  // " 001 " = 5 ký tự bên trong

    cout << "\n\tSO DO GHE (mau xanh = trong, do = da dat)\n\n";

    int total = this->_iTotalSeats;
    if (total <= 0)
    {
        cout << "\t(Chua co ghe nao duoc khai bao)\n";
        return;
    }

    for (int i = 0; i < total; ++i)
    {
        // ---- Vẽ đường ngang trên của hàng mới ----
        if (i % iSeatsPerRow == 0)
        {
            setTextColor(COLOR_DEFAULT);
            cout << "\t";
            int seatsThisRow = std::min(iSeatsPerRow, total - i);
            for (int j = 0; j < seatsThisRow; ++j)
            {
                cout << "+";
                for (int k = 0; k < iCellInnerWidth; ++k)
                    cout << "-";
            }
            cout << "+\n\t";
        }

        int seatNum = i + 1;

        // Format số ghế: 001, 010, 120,...
        std::ostringstream oss;
        oss << std::setw(3) << std::setfill('0') << seatNum;
        std::string seatStr = oss.str();

        // Vẽ cạnh trái ô
        setTextColor(COLOR_DEFAULT);
        cout << "|";

        // Chọn màu theo trạng thái ghế
        setTextColor(this->_vSeatFree[i] ? COLOR_SUCCESS : COLOR_ERROR);

        // Bên trong ô: " 001 "
        cout << " " << seatStr << " ";

        // Trả về màu mặc định
        setTextColor(COLOR_DEFAULT);

        // Cuối hàng hoặc ghế cuối cùng
        bool endOfRow = ((i + 1) % iSeatsPerRow == 0) || (i == total - 1);
        if (endOfRow)
        {
            cout << "|\n";

            int seatsThisRow = (i == total - 1)
                                   ? ((total - 1) % iSeatsPerRow + 1)
                                   : iSeatsPerRow;

            cout << "\t";
            for (int j = 0; j < seatsThisRow; ++j)
            {
                cout << "+";
                for (int k = 0; k < iCellInnerWidth; ++k)
                    cout << "-";
            }
            cout << "+\n";
        }
    }

    setTextColor(COLOR_DEFAULT);
}