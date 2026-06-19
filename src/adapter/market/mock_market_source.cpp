/// @file      mock_market_source.cpp
/// @brief     模拟行情源实现
#include <qtrade_sdk/quote/quote_api.hpp>

#include <spdlog/spdlog.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <random>
#include <thread>
#include <utility>

namespace qtrade_sdk::quote {

namespace {

class MockMarketSource final : public IMarketSource {
 public:
  MockMarketSource() : connected_(false), running_(false) {}

  ~MockMarketSource() override { Disconnect(); }

  void RegisterSpi(QuoteSpi& quote_spi) override { quote_spi_ = &quote_spi; }

  void UnregisterSpi() override { quote_spi_ = nullptr; }

  qtrade::ErrorCode Connect(const ConnectRequest& request) override {
    if (connected_) {
      return qtrade::ErrorCode::kAlreadyConnected;
    }
    config_ = request;
    connected_ = true;
    running_ = true;
    tick_thread_ = std::thread([this]() { GenerateMockTicks(); });
    spdlog::info("[MockMarketSource] connected: {}", config_.name);
    return qtrade::ErrorCode::kSuccess;
  }

  void Disconnect() override {
    if (!connected_) {
      return;
    }
    running_ = false;
    if (tick_thread_.joinable()) {
      tick_thread_.join();
    }
    connected_ = false;
    spdlog::info("[MockMarketSource] disconnected");
  }

  bool IsConnected() const override { return connected_; }

  std::int32_t Login(const std::string& ip,
                     std::uint16_t port,
                     const std::string& user,
                     const std::string& pwd) override {
    (void)ip;
    (void)port;
    (void)user;
    (void)pwd;
    return 0;
  }

  void Logout() override { Disconnect(); }

  std::int32_t RebuildSzData(std::uint32_t channel_no,
                             std::uint64_t begin_seq,
                             std::uint64_t end_seq,
                             std::uint64_t request_id) override {
    (void)channel_no;
    (void)begin_seq;
    (void)end_seq;
    (void)request_id;
    return -1;
  }

  void SetThreadAffinity(std::int32_t recv_cpu_no, std::int32_t process_cpu_no) override {
    (void)recv_cpu_no;
    (void)process_cpu_no;
  }

  std::int32_t SetBuffer(std::size_t buffer_size) override {
    (void)buffer_size;
    return 0;
  }

  int SubscribeAllIndexData(ExchangeType exchange_id) override {
    (void)exchange_id;
    return -1;
  }

  int UnSubscribeAllIndexData(ExchangeType exchange_id) override {
    (void)exchange_id;
    return -1;
  }

  int SubscribeIndexData(const std::vector<std::string>& tickers, ExchangeType exchange_id) override {
    (void)tickers;
    (void)exchange_id;
    return -1;
  }

  int UnSubscribeIndexData(const std::vector<std::string>& tickers, ExchangeType exchange_id) override {
    (void)tickers;
    (void)exchange_id;
    return -1;
  }

  int SubscribeAllMarketData(ExchangeType exchange_id) override {
    (void)exchange_id;
    return -1;
  }

  int UnSubscribeAllMarketData(ExchangeType exchange_id) override {
    (void)exchange_id;
    return -1;
  }

  int SubscribeMarketData(const std::vector<std::string>& tickers, ExchangeType exchange_id) override {
    (void)tickers;
    (void)exchange_id;
    return -1;
  }

  int UnSubscribeMarketData(const std::vector<std::string>& tickers, ExchangeType exchange_id) override {
    (void)tickers;
    (void)exchange_id;
    return -1;
  }

  qtrade::ErrorCode Subscribe(const SubscribeRequest& request) override {
    std::lock_guard<std::mutex> lock(mutex_);
    instruments_ = request.instruments;
    spdlog::info("[MockMarketSource] subscribed {} instruments", instruments_.size());
    return qtrade::ErrorCode::kSuccess;
  }

  qtrade::ErrorCode Unsubscribe(const UnsubscribeRequest& request) override {
    std::lock_guard<std::mutex> lock(mutex_);
    (void)request;
    instruments_.clear();
    return qtrade::ErrorCode::kSuccess;
  }

  int QueryAllTickers(ExchangeType exchange_id) override {
    (void)exchange_id;
    return -1;
  }

  int QueryAllTickersFullInfo(ExchangeType exchange_id) override {
    (void)exchange_id;
    return -1;
  }

  int QueryLatestInfo(const std::vector<std::string>& tickers,
                      TickerType ticker_type,
                      ExchangeType exchange_id) override {
    (void)tickers;
    (void)ticker_type;
    (void)exchange_id;
    return -1;
  }

  int QueryTickersPriceInfo(const std::vector<std::string>& tickers, ExchangeType exchange_id) override {
    (void)tickers;
    (void)exchange_id;
    return -1;
  }

  qtrade::ErrorCode QuerySnapshot(const QuerySnapshotRequest& request, QuerySnapshotResponse& response) override {
    (void)request;
    response.ticks.clear();
    return qtrade::ErrorCode::kNotSupported;
  }

  void SetTickCallback(TickCallback cb) override { on_tick_ = std::move(cb); }

  void SetBarCallback(BarCallback cb) override { on_bar_ = std::move(cb); }

  std::vector<std::string> GetSupportedInstruments() const override { return {"IF2401", "IC2401", "IH2401"}; }

 private:
  void GenerateMockTicks() {
    std::random_device rd;
    std::mt19937 gen(rd());
    double base_price = 100.0;

    while (running_) {
      {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& instrument : instruments_) {
          MarketTick tick;
          tick.instrument = instrument;
          tick.data_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::system_clock::now().time_since_epoch())
                               .count();
          base_price += std::uniform_real_distribution<>(-0.5, 0.5)(gen);
          tick.last_price = base_price;

          for (std::size_t i = 0; i < constants::kDefaultOrderBookDepth; ++i) {
            tick.bid_price[i] = tick.last_price - 0.2 * static_cast<double>(i + 1);
            tick.bid_volume[i] = 100 + static_cast<int64_t>(i) * 50;
            tick.ask_price[i] = tick.last_price + 0.2 * static_cast<double>(i + 1);
            tick.ask_volume[i] = 100 + static_cast<int64_t>(i) * 50;
          }

          tick.volume = 1000;
          tick.turnover = tick.last_price * static_cast<double>(tick.volume);
          tick.avg_price = tick.last_price;
          tick.open_interest = 50000;
          tick.open_price = base_price - 1.0;
          tick.high_price = base_price + 2.0;
          tick.low_price = base_price - 2.0;
          tick.pre_close_price = base_price - 0.5;

          if (on_tick_) {
            on_tick_(tick);
          }
        }
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  QuoteSpi* quote_spi_ = nullptr;
  TickCallback on_tick_;
  BarCallback on_bar_;
  ConnectRequest config_;
  std::atomic<bool> connected_;
  std::atomic<bool> running_;
  std::vector<std::string> instruments_;
  std::mutex mutex_;
  std::thread tick_thread_;
};

}  // namespace

std::unique_ptr<IMarketSource> CreateMockMarketSource() { return std::make_unique<MockMarketSource>(); }

void LinkMockMarketSourceForBuild() {
  MockMarketSource src;
  (void)src;
}

}  // namespace qtrade_sdk::quote
