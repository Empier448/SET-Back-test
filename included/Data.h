#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>

namespace indicator {
    struct Data {
        // ฟิลด์ต่างๆ ของ Data
        std::string Date;
        std::string Ticker;
        double Open;
        double High;
        double Low;
        //double adjustedClose;
        double Close;
        double Volume;
        double EPS;
        double SMA;
        double EMA;
        double RSI;
        double PE;
        bool SETNewHigh;
        bool SETNewLow;
        bool SETGood;
        bool Buy1;
        bool Buy2;
        bool Buy3;
        bool Buy4;
        bool Buy5;
        bool Buy6;
        bool BuySignal;
        double P_BV;
        double Yield;

        // คอนสตรัคเตอร์
        Data(const std::string& date = "", const std::string& ticker = "", double open = 0.0, double high = 0.0, double low = 0.0, double close = 0.0, double adjustedClose = 0.0, double volume = 0.0, double eps = 0.0)
            : Date(date), Ticker(ticker), Open(open), High(high), Low(low), Close(close), Volume(volume), EPS(eps), PE(0.0), SETNewHigh(false), SETNewLow(false), SETGood(false), Buy1(false), Buy2(false), Buy3(false), Buy4(false), Buy5(false), Buy6(false), BuySignal(false), P_BV(0.0), Yield(0.0) {}
    };
}           //adjustedClose(adjustedClose),

#endif // DATA_H
