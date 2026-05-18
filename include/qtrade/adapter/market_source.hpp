#ifndef QUANT_TRADING_ADAPTER_MARKET_SOURCE_HPP_
#define QUANT_TRADING_ADAPTER_MARKET_SOURCE_HPP_

#include <functional>
#include <string>
#include <vector>

#include <qtrade/structs/bar_data.hpp>
#include <qtrade/structs/error_code.hpp>
#include <qtrade/structs/tick_data.hpp>

namespace quant::trading::adapter {

struct MarketSourceConfig {
  std::string name;
  std::string connection_string;
};

// 行情适配器抽象接口（插件侧实现，核心通过回调收取 Tick/Bar）
class IMarketSource {
 public:
  virtual ~IMarketSource() = default;

  using TickCallback = std::function<void(const MarketTick&)>;
  using BarCallback = std::function<void(const Bar&)>;

  virtual ErrorCode Connect(const MarketSourceConfig& config) = 0;
  virtual void Disconnect() = 0;
  virtual ErrorCode Subscribe(const std::vector<std::string>& instruments) = 0;
  virtual ErrorCode Unsubscribe(const std::vector<std::string>& instruments) = 0;

  virtual void SetTickCallback(TickCallback cb) { on_tick_ = std::move(cb); }
  virtual void SetBarCallback(BarCallback cb) { on_bar_ = std::move(cb); }

  virtual bool IsConnected() const = 0;
  virtual std::vector<std::string> GetSupportedInstruments() const = 0;

 protected:
  TickCallback on_tick_;
  BarCallback on_bar_;
};

}  // namespace quant::trading::adapter

#endif  // QUANT_TRADING_ADAPTER_MARKET_SOURCE_HPP_
