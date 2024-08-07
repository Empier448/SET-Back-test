#ifndef __YAHOOAPI_H__
#define __YAHOOAPI_H__

#include <string>
#include <vector>

namespace yAPI
{
    enum Interval {
        MIN_5,
        MIN_15,
        MIN_30,
        HR_1,
        DAILY,
        WEEKLY,
        MONTHLY
    };

    class Yahoo
    {
    private:
        std::string url;
        std::string prefix;
        Interval interval;
        std::vector<std::string> stamps{ "5m", "15m", "30m", "1hr", "1d", "1wk", "1mo" }; // Strings that Yahoo Finance accepts for intervals

    public:
        Yahoo()
        {
            // Default values for the Yahoo Finance URL and Interval  
            // Default URL Template: https://query1.finance.yahoo.com/v7/finance/download/{ticker}?period1={start_time}&period2={end_time}&interval={interval}&events=history&includeAdjustedClose=true
            // Default Daily Value: 4 (Refer to stamps vector) 
            prefix = "Data/";
            url = "https://query1.finance.yahoo.com/v7/finance/download/{ticker}?period1={start_time}&period2={end_date}&interval={interval}&events=history&includeAdjustedClose=true";
            interval = DAILY; // Set default interval to DAILY
        }

        void setInterval(Interval VALUE);
        std::string getYahooInterval(Interval interval);
        bool downloadData(const std::string& url, const std::string& fileName);
        bool getData(const std::vector<std::string>& tickers, const std::string& startDate, const std::string& endDate);
        std::string fixYahooURL(const std::string& ticker, const std::string& startDate, const std::string& endDate);
        std::string fixYahooDate(std::string date);
        void resetURL();

       // void setTickers(const std::vector<std::string>& tickers);
    };

}

#endif // __YAHOOAPI_H__