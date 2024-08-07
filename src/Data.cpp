#include "Data.h"

namespace indicator {
    std::ostream& operator<<(std::ostream& os, const Data& data) {
        os << data.Date << ", "
            << data.Ticker << ", "
            << data.Open << ", "
            << data.High << ", "
            << data.Low << ", "
            << data.Close << ", "
            //<< data.adjustedClose << ", "  // เพิ่มที่นี่
            << data.Volume << ", "
            << data.EPS << ", "
            << data.SMA << ", "
            << data.EMA << ", "
            << data.RSI << ", "
            << data.PE << ", "
            << (data.SETNewHigh ? "true" : "false") << ", "
            << (data.SETNewLow ? "true" : "false") << ", "
            << (data.SETGood ? "true" : "false") << ", "
            << (data.Buy1 ? "true" : "false") << ", "
            << (data.Buy2 ? "true" : "false") << ", "
            << (data.Buy3 ? "true" : "false") << ", "
            << (data.Buy4 ? "true" : "false") << ", "
            << (data.Buy5 ? "true" : "false") << ", "
            << (data.Buy6 ? "true" : "false") << ", "
            << (data.BuySignal ? "true" : "false") << ", "
            << data.P_BV << ", "
            << data.Yield;

        return os;
    }
}
