#pragma once

#include "bypto/exchange.h"
#include "bypto/exchange/binance.h"
#include "bypto/exchange/back_testing.h"
#include "bypto/data/price.h"
#include "command.h"

#include <map>
#include <string>
#include <fstream>
#include <memory>

using namespace bypto::indicator;
using namespace bypto::strategy;
using namespace bypto::exchange;

typedef std::unique_ptr<BackTest> BackTestPtr;

BackTestPtr backtest(const CommandOptions &opts) {
    //parse binance klines historical data
    std::fstream fs("/home/ferdinand/dev/bypto/historical/binance/kline/BTCUSDT-15m-2021-07.csv");
    auto klines = parseCSV(opts.m_sym,fs);
    
    auto conn = tao::pq::connection::create("dbname=historical");
    prepareTable(conn);
    storeKlines(conn,klines);

    //Start with 1 BTC and 1000 USDT
    BackTestParams btp{opts.m_sym,1,1000,std::nullopt,std::nullopt,std::move(klines)};
    
    return std::make_unique<BackTest>(std::move(btp));
}

typedef std::unique_ptr<Binance> BinancePtr;
BinancePtr binance(const CommandOptions &opts,binapi::rest::api &api) {
    return std::make_unique<Binance>(api);
}

BinancePtr binance_test(const CommandOptions &opts,binapi::rest::api &api) {
    return std::make_unique<Binance>(api);
}