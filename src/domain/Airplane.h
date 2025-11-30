#pragma once

#include <string>
#include <iostream>

class Airplane
{
private:
    std::string _strPlaneId;   // So hieu may bay (duy nhat)
    int         _iSeatCount;   // So cho ngoi

public:
    // Constructor
    Airplane();
    Airplane(const std::string &planeId, int seatCount);

    // Setter
    void setPlaneId(const std::string &planeId);
    void setSeatCount(int seatCount);

    // Getter
    std::string getPlaneId() const;
    int         getSeatCount() const;

    // In thong tin
    void print() const;

    // So sanh object
    bool operator==(const Airplane &other) const;
};
