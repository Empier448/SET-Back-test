#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

namespace indicator
{
    struct Data
    {
        // ข้อมูลในไฟล์ CSV มีวันที่, ราคาเปิด, ราคาสูงสุด, ราคาต่ำสุด, ราคาปิด, ราคาปิดปรับปรุง, และปริมาณการซื้อขาย
        std::string date;
        std::string Ticker; // เปลี่ยนจาก double เป็น string เพื่อรองรับชื่อหุ้นที่เป็นตัวอักษร
        double open = 0.0;
        double high = 0.0;
        double low = 0.0;
        double Close = 0.0; // แก้ชื่อเป็นตัวใหญ่
        double adjustedClose = 0.0;
        unsigned long volume = 0;
        double EPS = 0.0;

        // ตัวชี้วัด
        double SMA = 0.0; // Simple Moving Average
        double WMA = 0.0; // Weighted Moving Average
        double EMA = 0.0; // Exponential Moving Average
        double RSI = 0.0; // Relative Strength Index
        double PE = 0.0; // Price-to-Earnings Ratio
        double P_BV = 0.0;
        double Yield = 0.0;
        bool SETNewHigh = 0.0;
        bool SETNewLow = 0.0;
        bool SETGood = 0.0;
        double Buy1 = 0.0;
        double Buy2 = 0.0;
        double Buy3 = 0.0;
        double Buy4 = 0.0;
        double Buy5 = 0.0;
        double Buy6 = 0.0;
        double BuySignal = 0.0;
         
 
        // เพิ่มโอเปอเรเตอร์ << สำหรับพิมพ์ข้อมูลออกทาง ostream
        friend std::ostream& operator<<(std::ostream& out, const Data& d)
        {
            out << d.date << " " << d.Ticker << " " << d.open << " " << d.high << " " << d.low << " " << d.Close
                << " " << d.adjustedClose << " " << d.volume << " " << d.SMA << " " << d.WMA << " " << d.EMA << " " << d.RSI << " " << d.PE << "\n"; // เพิ่ม RSI,P/E
            return out;
        }
    };

    // ฟังก์ชันสำหรับนำเข้าข้อมูลจากไฟล์ CSV
    std::vector<Data> importData(const std::string& fileName);

    class MA // Moving Averages
    {
    public:
        void simpleMA(std::vector<Data>& dataset, const unsigned int period); // Simple moving average
        void weightedMA(std::vector<Data>& dataset, const unsigned int period); // Weighted moving average
        void exponentialMA(std::vector<Data>& dataset, const unsigned int period); // Exponential moving average
        void exponentialMA15(std::vector<Data>& dataset); // เพิ่มบรรทัดนี้เพื่อประกาศ EMA15
        void calculatePERatio(std::vector<Data>& dataset, const std::vector<double>& epsData); // คำนวณค่า P/E Ratio
        void saveBuySignalToFile(const std::vector<Data>& dataset, const std::string& fileName);
        void saveToFile(const std::vector<Data>& dataset, const std::string& fileName); // Save data to CSV file
       // void printWithColors(const std::vector<Data>& dataset); // เพิ่มการประกาศฟังก์ชันนี้
    };

    class Volatility
    {
    public:
        void bollinger(std::vector<Data>& dataset); // Bollinger bands
        void donchian(std::vector<Data>& dataset); // Donchian channels
        void keltner(std::vector<Data>& dataset); // Keltner channels
    };

    class Oscillator
    {
    public:
        void rsi(std::vector<Data>& dataset, const unsigned int period); // Relative Strength Index
        void stochastic(std::vector<Data>& dataset); // Stochastic oscillator
        void tsi(std::vector<Data>& dataset); // True Strength Index
        void ultimate(std::vector<Data>& dataset); // Ultimate oscillator
    };
     
    void defineBuyingConditions(std::vector<Data>& dataset); // ฟังก์ชันสำหรับคำนวณเงื่อนไขการซื้อ
    //void saveBuySignalToFile(const std::vector<Data>& dataset, const std::string& fileName);
}
