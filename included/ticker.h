#ifndef __TICKER_H__
#define __TICKER_H__
// ส่วนหัวของไฟล์ (Header Guard)
// เป็นการป้องกันไม่ให้ไฟล์นี้ถูก include ซ้ำ ๆ ในโปรเจ็กต์เดียวกัน

#include <vector>
#include <string>

namespace util // การประกาศ namespace และคลาส ใช้ namespace util เพื่อแยกแยะการใช้งานฟังก์ชันและคลาสจาก namespace อื่น ๆ
{
    class Ticker // คลาส Ticker ประกอบด้วยสมาชิกข้อมูล (member variables) และฟังก์ชัน (member functions) ที่ใช้จัดการข้อมูลหุ้น
    {
        // สมาชิกข้อมูล (Member Variables)
        const std::string fileNameAppend = "tickers.txt"; // ชื่อไฟล์ที่เป็นค่าคงที่สำหรับเก็บข้อมูล tickers

        std::string fileName; // ชื่อไฟล์ที่ใช้จัดเก็บหรืออ่านข้อมูลหุ้น
        std::string url; // URL ที่ใช้ดาวน์โหลดข้อมูลหุ้น
        std::vector<std::string> tickers{}; // เวกเตอร์ที่เก็บรายชื่อหุ้นในรูปของสตริง

        // ฟังก์ชัน (Member Functions)
    public:
        bool downloadTickers(); // ฟังก์ชันที่ใช้ดาวน์โหลดข้อมูลหุ้นจาก URL ที่กำหนด (ฟังก์ชันนี้ควรจะคืนค่า true ถ้าการดาวน์โหลดสำเร็จ และ false ถ้าล้มเหลว)
        bool readNASDAQTickers(); // ฟังก์ชันที่อ่านรายชื่อหุ้นจากไฟล์ (ควรจะคืนค่า true ถ้าอ่านสำเร็จ และ false ถ้าล้มเหลว)
        void getNASDAQTickers(); // ฟังก์ชันที่ใช้เพื่อดึงรายชื่อหุ้น NASDAQ (รายละเอียดการทำงานของฟังก์ชันนี้จะอยู่ในส่วนที่เป็นการ implement ในไฟล์ .cpp)
        bool readSETTickers(); // ฟังก์ชันที่อ่านรายชื่อหุ้น SET จากไฟล์ (ควรจะคืนค่า true ถ้าอ่านสำเร็จ และ false ถ้าล้มเหลว)
        void getSETTickers(); // ฟังก์ชันที่ใช้เพื่อดึงรายชื่อหุ้น SET (รายละเอียดการทำงานของฟังก์ชันนี้จะอยู่ในส่วนที่เป็นการ implement ในไฟล์ .cpp)
        std::vector<std::string> getTickerList(); // ฟังก์ชันที่คืนค่าเวกเตอร์ของรายชื่อหุ้นที่มีการจัดเก็บอยู่ในคลาส
    };
}

#endif // __TICKER_H__


//สรุป
//คลาส Ticker ถูกออกแบบมาเพื่อจัดการและดาวน์โหลดข้อมูลหุ้นจาก NASDAQ ผ่านทาง URL รวมถึงการอ่านข้อมูลจากไฟล์และการดึงรายชื่อหุ้นออกมาใช้งาน

//การ implement ฟังก์ชันเหล่านี้จะอยู่ในไฟล์.cpp ซึ่งคุณต้องเขียนต่อไปเพื่อให้คลาสนี้สามารถทำงานได้ตามที่ออกแบบไว้