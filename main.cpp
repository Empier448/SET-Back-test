#include "main.h"
#include "yahooAPI.h"
#include "ticker.h"
#include "indicators.h"
#include <iostream>
#include <vector>
#include <fstream>

int main(int, char**)
{
    // Create the objects we need to do all our stuff
    yAPI::Yahoo yahoo;
    util::Ticker ticker;
    std::vector<indicator::Data> dataset;

    // Example: Get SET tickers
    ticker.getSETTickers();
    std::vector<std::string> setTickers = ticker.getTickerList();

    // Import our downloaded data
    dataset = indicator::importData("C:\\Users\\Thanimwas\\Downloads\\Python\\Data\\stocks_data.csv");

    // Extract EPS values from dataset
    std::vector<double> epsData;
    for (const auto& data : dataset) {
        epsData.push_back(data.EPS); // Assumes `EPS` is a member of `indicator::Data`
    }

    // Check if epsData size matches dataset size
    if (epsData.size() != dataset.size())
    {
        std::cerr << "EPS data size does not match dataset size.\n";
        return 1;
    }

    // Calculate P/E Ratio
    indicator::MA ma;
    ma.calculatePERatio(dataset, epsData);

    // Calculate SMA (14 day period)
   // ma.simpleMA(dataset, 14);

    // Calculate EMA (14 day period)
  //  ma.exponentialMA(dataset, 14);

    // Calculate RSI (14 day period)
    indicator::Oscillator osc;
    osc.rsi(dataset, 14);

    // Calculate buying conditions
    indicator::defineBuyingConditions(dataset);

    // Save results to a file
    ma.saveBuySignalToFile(dataset, "output_BuySignal.csv");

    // Save results to a file
    ma.saveToFile(dataset, "output.csv");

    // Set interval to DAILY if required
    yahoo.setInterval(yAPI::DAILY);

    // Download the data & save into csv file
    //yahoo.getData({ "AAPL" }, "2024-08-01", "2024-08-02");

    // Download and create a vector of tickers to download from Yahoo Finance
    //ticker.getNASDAQTickers();
   // ticker.getSETTickers();

   // yahoo.getData(ticker.getTickerList(), "2024-07-25", "2024-08-07");

    return 0;
}
