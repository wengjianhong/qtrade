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

/// @brief 行情源配置结构体
struct MarketSourceConfig {
  std::string name;               /// 行情源名称
  std::string connection_string;  /// 连接字符串
};

/// @brief 行情适配器抽象接口
/// @details 插件侧实现，核心通过回调收取 Tick/Bar 数据
class IMarketSource {
 public:
  virtual ~IMarketSource() = default;

  /// @brief Tick数据回调函数类型
  using TickCallback = std::function<void(const MarketTick&)>;
  /// @brief Bar数据回调函数类型
  using BarCallback = std::function<void(const Bar&)>;

  /// @brief 连接到行情源
  /// @param config 行情源配置
  /// @return ErrorCode::kSuccess 表示成功，其他表示失败
  virtual ErrorCode Connect(const MarketSourceConfig& config) = 0;

  /// @brief 断开连接
  virtual void Disconnect() = 0;

  /// @brief 订阅合约
  /// @param instruments 合约代码列表
  /// @return ErrorCode::kSuccess 表示成功，其他表示失败
  virtual ErrorCode Subscribe(const std::vector<std::string>& instruments) = 0;

  /// @brief 取消订阅合约
  /// @param instruments 合约代码列表
  /// @return ErrorCode::kSuccess 表示成功，其他表示失败
  virtual ErrorCode Unsubscribe(const std::vector<std::string>& instruments) = 0;

  /// @brief 设置Tick回调
  /// @param cb 回调函数
  virtual void SetTickCallback(TickCallback cb) { on_tick_ = std::move(cb); }

  /// @brief 设置Bar回调
  /// @param cb 回调函数
  virtual void SetBarCallback(BarCallback cb) { on_bar_ = std::move(cb); }

  /// @brief 检查连接状态
  /// @return true表示已连接，false表示未连接
  virtual bool IsConnected() const = 0;

  /// @brief 获取支持的合约列表
  /// @return 合约代码列表
  virtual std::vector<std::string> GetSupportedInstruments() const = 0;

 protected:
  BarCallback on_bar_;    /// Bar数据回调
  TickCallback on_tick_;  /// Tick数据回调
};

/// @brief 创建模拟行情源工厂函数
/// @return 模拟行情源实例
std::unique_ptr<IMarketSource> CreateMockMarketSource();

}  // namespace qtrade::adapter

#endif  // QTRADE_TRADING_ADAPTER_MARKET_SOURCE_HPP_
