#include "ticker.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>

namespace util {

    // ฟังก์ชันดาวน์โหลดข้อมูลจาก URL
    bool Ticker::downloadTickers() {
        CURL* curl;
        FILE* fp;
        CURLcode res;

        curl = curl_easy_init();

        if (curl) {
            errno_t err = fopen_s(&fp, fileName.c_str(), "wb");

            if (err != 0) {
                std::cerr << "Could not open file for writing: " << fileName << '\n';
                return false;
            }

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);

            curl_easy_cleanup(curl);
            fclose(fp);

            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
                return false;
            }

            return true;
        }

        return false;
    }

    // ฟังก์ชันอ่านข้อมูลหุ้นจากไฟล์ NASDAQ
    bool Ticker::readNASDAQTickers() {
        std::ifstream file(fileName);  //ใช้ std::ifstream เพื่อเปิดไฟล์สำหรับอ่าน ข้อมูลจากไฟล์ที่ระบุด้วย
        std::string line = "";

        std::cout << "Opening file: " << fileName << '\n';

        if (!file.is_open()) {
            std::cerr << "Could not open file for reading: " << fileName << '\n';
            return false;
        }
        /*
        // ตรวจสอบว่าไฟล์ว่างหรือไม่
        file.seekg(0, std::ios::end);
        if (file.tellg() == 0) {
            std::cerr << "File is empty: " << fileName << '\n';
            file.close();
            return false;
        }
        file.seekg(0, std::ios::beg); // ย้ายตำแหน่งการอ่านกลับไปที่จุดเริ่มต้น

        std::cout << "Reading file contents...\n";
        */
        /*
        // พิมพ์เนื้อหาของไฟล์ออกหน้าจอ
        std::cout << "Contents of file " << fileName << ":\n";
        while (std::getline(file, line)) {
            std::cout << line << '\n'; // พิมพ์แต่ละบรรทัดออกหน้าจอ
        }

        std::cout << "Resetting file reading position...\n";
        // รีเซ็ตตำแหน่งการอ่านไฟล์ไปที่จุดเริ่มต้น
        file.clear();                // เคลียร์สถานะผิดพลาด
        file.seekg(0, std::ios::beg); // ย้ายตำแหน่งการอ่านกลับไปที่จุดเริ่มต้น

        std::cout << "Extracting tickers...\n";
        // รีเซ็ตค่า line
        line = "";
        */

        // อ่านและแยกข้อมูลจากไฟล์
        while (std::getline(file, line)) {  //ใช้ std::getline() เพื่ออ่านข้อมูลจากไฟล์ทีละบรรทัด.
            size_t position = line.find_first_of("|");//ใช้ line.find_first_of("|") เพื่อหาตำแหน่งของเครื่องหมาย | ซึ่งใช้เป็นตัวคั่นข้อมูลในไฟล์.

            try {
                std::string ticker = line.substr(0, position);//ใช้ line.substr(0, position) เพื่อดึงข้อมูลที่อยู่ก่อนเครื่องหมาย | และเพิ่มข้อมูลนั้นลงในคอลเลกชัน tickers.
                tickers.push_back(ticker);
                std::cout << "Ticker added: " << ticker << '\n';
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
        }
        //ขั้นตอนนี้ใช้เพื่อลบรายการแรกและสุดท้ายออกจาก tickers, ซึ่งอาจเป็นการทำความสะอาดข้อมูลที่อ่านเข้ามา.
        if (!tickers.empty()) {     //ถ้า tickers ไม่ว่าง, 
            std::cout << "Removing first and last ticker...\n";
            tickers.pop_back();     //ใช้ tickers.pop_back() เพื่อลบรายการสุดท้าย
            if (!tickers.empty()) {
                tickers.erase(tickers.begin());  // เพื่อลบรายการแรก.
            }
        }
        //ปิดไฟล์และลบไฟล์ที่อ่านแล้ว:
        file.close();
        std::cout << "File processing completed.\n";
        std::remove(fileName.c_str());//เพื่อลบไฟล์ที่อ่านเสร็จแล้ว.
        std::cout << "File removed: " << fileName << '\n';

        return true;
    }


    // ฟังก์ชันอ่านข้อมูลหุ้นจากไฟล์ SET
    bool Ticker::readSETTickers() {
        std::ifstream file(fileName);//ใช้ std::ifstream เพื่อเปิดไฟล์สำหรับอ่าน ข้อมูลจากไฟล์ที่ระบุด้วย fileName. ตัวแปร line จะใช้เพื่อเก็บบรรทัดที่อ่านจากไฟล์.
        std::string line = "";

        if (!file.is_open()) {  //ตรวจสอบว่าไฟล์ถูกเปิดสำเร็จหรือไม่ ถ้าไม่สามารถเปิดไฟล์ได้ ฟังก์ชันจะพิมพ์ข้อความผิดพลาดและคืนค่า false.
            std::cerr << "Could not open file for reading: " << fileName << '\n';
            return false;
        }
        /*
        /// พิมพ์เนื้อหาของไฟล์ออกหน้าจอ
        std::cout << "Contents of file " << fileName << ":\n";
        while (std::getline(file, line)) {
            std::cout << line << '\n'; // พิมพ์แต่ละบรรทัดออกหน้าจอ
        }
        */

        //ใช้ std::getline() เพื่ออ่านข้อมูลจากไฟล์ทีละบรรทัดจนกว่าจะอ่านจบ.
        while (std::getline(file, line)) {
            // แยกข้อมูลจาก CSV โดยใช้เครื่องหมายคอมมา
            size_t position = line.find_first_of(",");  //line.find_first_of(",")ใช้เพื่อหาตำแหน่งของคอมมาในบรรทัด ถ้าพบคอมมา, position จะเป็นตำแหน่งที่พบ.
            if (position != std::string::npos) {
                try {  //พื่อเพิ่มข้อมูลจากแต่ละบรรทัดลงในเวกเตอร์ tickers
                    tickers.push_back(line.substr(0, position));//line.substr(0, position) ใช้เพื่อดึงข้อมูลก่อนคอมมา (ซึ่งเป็นชื่อหุ้น) และเก็บไว้ในตัวแปร tickers.
                }
                catch (const std::exception& e) {  //ใช้ try-catch เพื่อจับข้อผิดพลาดที่อาจเกิดขึ้นระหว่างการแยกข้อมูลและการเพิ่มข้อมูลลงใน tickers.
                    std::cerr << e.what() << '\n';
                }
            }
        }

        file.close();
        // ไม่ลบไฟล์ที่อ่านแล้วในกรณีนี้ เพราะเราไม่ต้องการลบไฟล์ข้อมูลที่ใช้
        // std::remove(fileName.c_str());

        return true;
    }

    // ฟังก์ชันดึงข้อมูลหุ้น NASDAQ
    void Ticker::getNASDAQTickers() {
        url = "ftp://ftp.nasdaqtrader.com/symboldirectory/nasdaqlisted.txt";//url ถูกตั้งค่าเป็นที่อยู่ FTP ที่ใช้ดาวน์โหลดไฟล์ข้อมูล ticker ของ NASDAQ จากเซิร์ฟเวอร์
        fileName = "NASDAQ" + fileNameAppend;//fileName ถูกตั้งค่าเป็นชื่อไฟล์ที่จะใช้เก็บข้อมูลที่ดาวน์โหลด ซึ่งประกอบด้วยคำว่า "NASDAQ" และค่า fileNameAppend (ซึ่งอาจเป็นส่วนขยายหรือค่าที่เพิ่มเติมจากการกำหนดอื่น).

        downloadTickers();
        readNASDAQTickers();
    }

    // ฟังก์ชันดึงข้อมูลหุ้น SET
    void Ticker::getSETTickers() {
        // URL ของข้อมูลหุ้น SET ถูกกำหนดให้เป็นไฟล์ที่อยู่ในระบบของคุณ
        fileName = "C:\\Users\\Thanimwas\\Downloads\\Data\\stock\\stock_data3.csv";

        readSETTickers();
        
    }

    // ฟังก์ชันคืนค่าเวกเตอร์ของรายชื่อหุ้น
    std::vector<std::string> Ticker::getTickerList() {
    
        return tickers;
    }
}
