#ifndef QUANT_TRADING_ENGINE_MARKET_HANDLER_HPP_
#define QUANT_TRADING_ENGINE_MARKET_HANDLER_HPP_

#include <qtrade/adapter/market_source.hpp>
#include <qtrade/engine/event_bus.hpp>
#include <memory>
#include <mutex>
#include <vector>

namespace quant::trading::engine::market {

class MarketHandler {
 public:
  explicit MarketHandler(event_bus::EventBus& event_bus);
  ~MarketHandler();

  void Start();
  void Stop();
  
  // 设置行情源
  void SetMarketSource(std::unique_ptr<adapter::IMarketSource> source);
  
  // 获取当前行情源（用于外部初始化）
  adapter::IMarketSource* GetMarketSource();
  
  // 订阅合约
  void Subscribe(const std::vector<std::string>& instruments);
  void Unsubscribe(const std::vector<std::string>& instruments);

 private:
  event_bus::EventBus& event_bus_;
  std::unique_ptr<adapter::IMarketSource> market_source_;
  std::mutex mutex_;
  bool running_;
  
  // 行情回调处理
  void OnTick(const MarketTick& tick);
  void OnBar(const Bar& bar);
};

}  // namespace quant::trading::engine::market

#endif  // QUANT_TRADING_ENGINE_MARKET_HANDLER_HPP_
