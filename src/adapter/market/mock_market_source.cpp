#include <qtrade/adapter/market_source.hpp>
#include <spdlog/spdlog.h>
#include <thread>
#include <atomic>
#include <random>
#include <chrono>

namespace qtrade::adapter {

namespace {

class MockMarketSource final : public IMarketSource {
 public:
  MockMarketSource() : connected_(false), running_(false) {}

  ~MockMarketSource() override { Disconnect(); }

  ErrorCode Connect(const MarketSourceConfig& config) override {
    if (connected_) {
      return ErrorCode::kAlreadyConnected;
    }
    config_ = config;
    connected_ = true;
    running_ = true;

    // 启动模拟行情生成线程
    tick_thread_ = std::thread([this]() { GenerateMockTicks(); });

    spdlog::info("[MockMarketSource] connected successfully: {}", config.name);
    return ErrorCode::kSuccess;
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

  ErrorCode Subscribe(const std::vector<std::string>& instruments) override {
    std::lock_guard<std::mutex> lock(mutex_);
    instruments_ = instruments;
    spdlog::info("[MockMarketSource] subscribed to {} instruments", instruments.size());
    return ErrorCode::kSuccess;
  }

  ErrorCode Unsubscribe(const std::vector<std::string>& instruments) override {
    std::lock_guard<std::mutex> lock(mutex_);
    // 简单实现：清空订阅列表
    (void)instruments;
    instruments_.clear();
    return ErrorCode::kSuccess;
  }

  bool IsConnected() const override { return connected_; }

  std::vector<std::string> GetSupportedInstruments() const override { return {"IF2401", "IC2401", "IH2401"}; }

 private:
  void GenerateMockTicks() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> price_dist(100.0, 1.0);

    double base_price = 100.0;

    while (running_) {
      {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& instrument : instruments_) {
          MarketTick tick;
          tick.instrument = instrument;
          tick.timestamp = std::chrono::system_clock::now();

          // 生成随机价格波动
          base_price += (std::uniform_real_distribution<>(-0.5, 0.5)(gen));
          tick.last_price = base_price;

          // 简单的买卖盘
          for (int i = 0; i < 5; ++i) {
            tick.bid_price[i] = tick.last_price - 0.2 * (i + 1);
            tick.bid_volume[i] = 100 + i * 50;
            tick.ask_price[i] = tick.last_price + 0.2 * (i + 1);
            tick.ask_volume[i] = 100 + i * 50;
          }

          tick.volume = 1000;
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

  MarketSourceConfig config_;
  std::atomic<bool> connected_;
  std::atomic<bool> running_;
  std::vector<std::string> instruments_;
  std::mutex mutex_;
  std::thread tick_thread_;
};

}  // namespace

// 工厂函数：创建模拟行情源
std::unique_ptr<IMarketSource> CreateMockMarketSource() { return std::make_unique<MockMarketSource>(); }

// 保证行情适配器实现目标与核心工程链接，后续由加载器替换为真实插件
void LinkMockMarketSourceForBuild() {
  MockMarketSource src;
  (void)src;
}

}  // namespace qtrade::adapter
