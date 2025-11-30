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

SeatList::SeatList()
    : _iTotalSeats(0), _vSeatFree()
{
}

SeatList::SeatList(int iTotalSeats)
    : _iTotalSeats(0), _vSeatFree()
{
    init(iTotalSeats);
}

SeatList::~SeatList()
{
}

// ============================== Init / Basic ==============================

void SeatList::init(int iTotalSeats)
{
    if (iTotalSeats > 0)
    {
        _iTotalSeats = iTotalSeats;
        _vSeatFree.assign(iTotalSeats, true);
    }
    else
    {
        _iTotalSeats = 0;
        _vSeatFree.clear();
    }
}

int SeatList::getTotalSeats() const
{
    return _iTotalSeats;
}

bool SeatList::isSeatValid(int iSeatNumber) const
{
    return (iSeatNumber >= 1 && iSeatNumber <= _iTotalSeats);
}

bool SeatList::isSeatFree(int iSeatNumber) const
{
    if (!isSeatValid(iSeatNumber))
    {
        return false;
    }

    return _vSeatFree[iSeatNumber - 1];
}

int SeatList::countFreeSeats() const
{
    int iCount = 0;

    for (bool bIsFree : _vSeatFree)
    {
        if (bIsFree)
        {
            iCount++;
        }
    }

    return iCount;
}

// ============================ Đặt / Hủy ghế ============================

bool SeatList::bookSeat(int iSeatNumber)
{
    if (!isSeatValid(iSeatNumber) || !isSeatFree(iSeatNumber))
    {
        return false;
    }

    _vSeatFree[iSeatNumber - 1] = false;
    return true;
}

bool SeatList::cancelSeat(int iSeatNumber)
{
    if (!isSeatValid(iSeatNumber))
    {
        return false;
    }

    if (_vSeatFree[iSeatNumber - 1])
    {
        // Ghế đang trống, không thể hủy
        return false;
    }

    _vSeatFree[iSeatNumber - 1] = true;
    return true;
}

// ========================== In danh sách ghế ==========================

void SeatList::printFreeSeats() const
{
    cout << "Cac ghe con trong: ";

    for (int iIndex = 0; iIndex < _iTotalSeats; ++iIndex)
    {
        if (_vSeatFree[iIndex])
        {
            cout << (iIndex + 1) << " ";
        }
    }

    cout << endl;
}

void SeatList::printSeatMap() const
{
    const int iSeatsPerRow    = 6;  // mỗi hàng 6 ghế
    const int iCellInnerWidth = 5;  // " 001 " = 5 ký tự bên trong

    cout << "\n\tSO DO GHE (mau xanh = trong, do = da dat)\n\n";

    int total = _iTotalSeats;
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
        setTextColor(_vSeatFree[i] ? COLOR_SUCCESS : COLOR_ERROR);

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
