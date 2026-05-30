/// @file      market_source.hpp
/// @brief     行情源适配器接口
/// @details   定义行情源适配器的抽象接口，用于接收市场数据
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_ADAPTER_MARKET_SOURCE_HPP_
#define QTRADE_TRADING_ADAPTER_MARKET_SOURCE_HPP_
#include <qtrade/error_code/code_define.hpp>
#include <qtrade/structs/bar_data.hpp>
#include <qtrade/structs/tick_data.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace qtrade::adapter {

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
  BarCallback on_bar_;
  TickCallback on_tick_;
};

// 工厂函数：创建模拟行情源
std::unique_ptr<IMarketSource> CreateMockMarketSource();

}  // namespace qtrade::adapter

#endif  // QTRADE_TRADING_ADAPTER_MARKET_SOURCE_HPP_
