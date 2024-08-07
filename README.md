### Build tools used:

    Visual Studio Code (Editor)
    CMake (Build System)
    VCPKG (Libraries/Dependencies)
    Git with Github (Source Control)

### Libraries Used

    STL
    LIBCURL

---

### How to use:

1) Create a yAPI::Yahoo object

2) Create a util::Ticker object

3) Call Ticker::getNASDAQTickers to populate a vector filled with NASDAQ tickers

4) Call Yahoo::getData to download all the data for each ticker!

5) The fun times have come and we can now begin to perform some analysis on all 
