#include "domain/Airplane.h"

// ========================= Constructors =========================

Airplane::Airplane()
    : _strPlaneId(""), _iSeatCount(0)
{
}

Airplane::Airplane(const std::string &planeId, int seatCount)
    : _strPlaneId(planeId), _iSeatCount(seatCount)
{
}

// ========================= Setter =========================

void Airplane::setPlaneId(const std::string &planeId)
{
    _strPlaneId = planeId;
}

void Airplane::setSeatCount(int seatCount)
{
    _iSeatCount = seatCount;
}

// ========================= Getter =========================

std::string Airplane::getPlaneId() const
{
    return _strPlaneId;
}

int Airplane::getSeatCount() const
{
    return _iSeatCount;
}

// ========================= Print =========================

void Airplane::print() const
{
    std::cout << "\n===== THONG TIN MAY BAY =====\n";
    std::cout << "So hieu may bay : " << _strPlaneId << "\n";
    std::cout << "So cho ngoi     : " << _iSeatCount << "\n";
    std::cout << "===================================\n";
}

// ========================= Comparison =========================

bool Airplane::operator==(const Airplane &other) const
{
    return (_strPlaneId == other._strPlaneId &&
            _iSeatCount == other._iSeatCount);
}
