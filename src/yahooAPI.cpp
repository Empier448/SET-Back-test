#include "yahooAPI.h"
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <thread>
#include <iomanip>

namespace yAPI {

    void Yahoo::setInterval(Interval VALUE)
    {
        interval = VALUE;
    }

    std::string Yahoo::getYahooInterval(Interval interval)
    {
        return stamps[interval];
    }

    size_t write_data(void* ptr, size_t size, size_t nmemb, std::string* data)
    {
        data->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }

    bool Yahoo::downloadData(const std::string& url, const std::string& fileName)
    {
        CURL* curl;
        CURLcode res;
        std::string buffer;

        curl = curl_easy_init();

        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\Users\\Thanimwas\\AppData\\Local\\Programs\\Python\\Python312\\Lib\\site-packages\\certifi\\cacert.pem");
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "YourCustomUserAgentString");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            res = curl_easy_perform(curl);

            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

            if (response_code == 404)
            {
                std::cerr << "Failed to download data for " << fileName << ". HTTP Response Code: 404. URL: " << url << std::endl;
            }
            else if (response_code == 429)
            {
                std::cout << "Rate limit exceeded. Sleeping for 60 seconds..." << std::endl;
                curl_easy_cleanup(curl);
                std::this_thread::sleep_for(std::chrono::seconds(60));
                return false;
            }
            else if (res != CURLE_OK)
            {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                curl_easy_cleanup(curl);
                return false;
            }
            else
            {
                std::ofstream outFile(fileName);
                if (outFile.is_open())
                {
                    outFile << buffer;
                    outFile.close();
                    std::cout << "Data downloaded successfully for " << fileName << std::endl;
                }
                else
                {
                    std::cerr << "Failed to open file: " << fileName << std::endl;
                    curl_easy_cleanup(curl);
                    return false;
                }
            }

            curl_easy_cleanup(curl);
        }
        else
        {
            std::cerr << "Failed to initialize CURL" << std::endl;
            return false;
        }

        return true;
    }

    bool Yahoo::getData(const std::vector<std::string>& tickers, const std::string& startDate, const std::string& endDate)
    {
        for (std::string ticker : tickers)
        {
            // แก้ไข ticker ให้มี .BK
            if (ticker.find(".BK") == std::string::npos) {
                ticker += ".BK";
            }

            std::cout << "Downloading data for {" << ticker << "}\n\\(^o^)/\n";

            resetURL();
            fixYahooURL(ticker, startDate, endDate);

            if (downloadData(url, prefix + ticker + ".csv"))
            {
                std::cout << "Finished downloading data for {" << ticker << "}.\n\n\n";
            }
            else
            {
                std::cerr << "ERROR: Could not download data for {" << ticker << "}. Please check the ticker.\n\n\n";
            }
        }

        return true;
    }

    std::string Yahoo::fixYahooURL(const std::string& ticker, const std::string& startDate, const std::string& endDate)
    {
        size_t startPos = url.find("{ticker}");
        if (startPos != std::string::npos)
        {
            url.replace(startPos, 8, ticker);
        }

        startPos = url.find("{start_time}");
        if (startPos != std::string::npos)
        {
            url.replace(startPos, 12, fixYahooDate(startDate));
        }

        startPos = url.find("{end_time}");
        if (startPos != std::string::npos)
        {
            url.replace(startPos, 10, fixYahooDate(endDate));
        }

        startPos = url.find("{interval}");
        if (startPos != std::string::npos)
        {
            url.replace(startPos, 10, getYahooInterval(interval));
        }

        return url;
    }

    std::string Yahoo::fixYahooDate(std::string date)
    {
        struct std::tm time = { 0,0,0,0,0,0,0,0,0 };
        std::istringstream ss(date);
        ss >> std::get_time(&time, "%Y-%m-%d");

        if (ss.fail())
        {
            std::cerr << "ERROR: Cannot parse date (" << date << "); Form: YYYY-MM-DD" << std::endl;
            exit(1);
        }

        time.tm_hour = 0;
        time.tm_min = 0;
        time.tm_sec = 0;

        std::time_t epoch = std::mktime(&time);

        return std::to_string(epoch);
    }

    void Yahoo::resetURL()
    {
        url = "https://query1.finance.yahoo.com/v7/finance/download/{ticker}?period1={start_time}&period2={end_time}&interval={interval}&events=history&includeAdjustedClose=true";
    }

} // namespace yAPI

