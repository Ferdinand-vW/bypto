#pragma once

#include "bypto/account.h"
#include "bypto/common/either.h"
#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "bypto/data/binance.h"
#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/order/fill.h"
#include "bypto/order/generic.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"

#include <map>
#include <span>
#include <string>

namespace bypto::exchange {
using namespace common::types;
using namespace common::utils;
using namespace bypto::data::binance;

struct BackTestParams {
    // Support for backtesting is limited to one currencypair at a time
    Symbol m_sym;
    // initial funds
    Quantity m_base_qty;
    Quantity m_quote_qty;
    // start time defaults to first kline in @m_klines@
    std::optional<time_t> m_start_time;
    // tick rate defaults to tick rate in @m_klines@
    std::optional<time_unit> m_tick_rate;
    // historical kline data used for back testing
    std::vector<Kline_t> m_klines;
};

class BackTest : public Exchange<BackTest, PriceSource::Kline> {
    std::vector<Kline_t> m_klines;
    int m_kline_index = 0;

    std::map<time_t, account::Account> m_account_history;
    std::map<time_t, std::tuple<Symbol, long double>> m_price_history;
    std::map<time_t, order::Partial> m_order_history;

    std::map<int, order::GenericOrder> m_orders;
    account::Account m_account;

    int m_counter = 0;
    Symbol m_symbol;
    time_unit m_tick_rate;
    time_t m_curr_time;

    void update_account(time_t curr, order::Partial p);

  public:
    explicit BackTest(BackTestParams &&prms);
    BackTest(const BackTest &) = delete;
    BackTest &operator=(const BackTest &) = delete;
    ~BackTest() override = default;

    template <order::OType T> Error<int> execute_order(order::Order<T> go) {
        m_orders.insert({m_counter, order::GenericOrder(go)});
        m_counter++;
        return m_counter - 1;
    }

    // Return prices for symbols as given time
    Error<std::map<Symbol, long double>> get_price_map(time_t t = 0);

    Error<account::Account> get_account_info();
    Error<long double> fetch_price(Symbol symbol);
    Error<bool> cancel_order(int o_id);
    Error<bool> tick_once();
    time_t get_current_time();
    Klines_t get_historical_prices(time_t start, time_t end);
    Klines_t get_all_historical();

    std::stringstream get_funds_csv() const;
    std::stringstream get_pnls_csv() const;
    std::stringstream get_orders_csv() const;
    std::stringstream get_prices_csv() const;
};
} // namespace bypto::exchange