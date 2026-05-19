#ifndef QTRADE_ERROR_CODE_CODE_SEGMENT_HPP_
#define QTRADE_ERROR_CODE_CODE_SEGMENT_HPP_
#include <cstdint>

namespace qtrade {

/// AA 级：整个 qtrade 系统固定为 10
enum class CodeSystem : std::int32_t {
  kQTradeSystem = 10,  /// qtrade 系统错误码段
};

/// BBB 级模块编号
enum class ModuleNumber : std::int32_t {
  /// 系统级（BBB=000）
  kQTradeSystem = 0,

  /// 适配器层（BBB=100）
  kMarketAdapter = 10,     /// 行情适配器模块
  kExecutionAdapter = 11,  /// 交易执行适配器模块

  /// 交易引擎层（BBB=200）
  kEngineCommon = 200,    /// 交易引擎通用错误码段
  kAccount = 201,         /// 账号管理模块
  kEventBus = 202,        /// 事件总线模块
  kCompliance = 203,      /// 交易合规模块(cms)
  kExecution = 204,       /// 交易执行模块(ems)
  kOrder = 205,           /// 订单管理模块(oms)
  kMarket = 206,          /// 行情处理模块
  kPosition = 207,        /// 持仓管理模块
  kRisk = 208,            /// 风险管理模块
  kStrategyEngine = 209,  /// 策略引擎模块

  /// 支撑服务层（BBB=200）
  kServiceCommon = 300,  /// 支撑服务通用错误码段
  kAudit = 301,          /// 审计服务
  kBacktest = 302,       /// 回测服务
  kBackup = 303,         /// 备份服务
  kConfig = 304,         /// 配置服务
  kHistoryMarket = 305,  /// 历史行情服务
  kHistoryOrder = 306,   /// 历史订单服务
  kLog = 307,            /// 日志服务
  kMonitor = 308,        /// 监控服务
  kRegistry = 309,       /// 注册服务
  kStrategy = 310,       /// 策略管理服务
};

/// 构造 AABBBCCC 格式错误码（AA=10, BBB=module_number, CCC=detail）
inline constexpr std::int32_t MakeErrorCode(ModuleNumber module_number, std::int32_t code) {
  return static_cast<std::int32_t>(CodeSystem::kQTradeSystem) * 1'000'000 +
         static_cast<std::int32_t>(module_number) * 1'000 + code;
}

/// CCC 级：具体错误码从 1 起 到 999 止
inline constexpr std::int32_t kErrorCodeStart = 1;
inline constexpr std::int32_t kErrorCodeEnd = 999;

/// 各模块错误码划段（每模块预留 999 个 CCC）
struct ModuleErrorCodeRange {
  // 系统级（BBB=000）
  static constexpr std::int32_t kQTradeSystemStart = MakeErrorCode(ModuleNumber::kQTradeSystem, kErrorCodeStart);
  static constexpr std::int32_t kQTradeSystemEnd = MakeErrorCode(ModuleNumber::kQTradeSystem, kErrorCodeEnd);

  // 适配器模块（BBB 从 10 起）
  static constexpr std::int32_t kMarketAdapterStart = MakeErrorCode(ModuleNumber::kMarketAdapter, kErrorCodeStart);
  static constexpr std::int32_t kMarketAdapterEnd = MakeErrorCode(ModuleNumber::kMarketAdapter, kErrorCodeEnd);

  static constexpr std::int32_t kExecutionAdapterStart =
    MakeErrorCode(ModuleNumber::kExecutionAdapter, kErrorCodeStart);
  static constexpr std::int32_t kExecutionAdapterEnd = MakeErrorCode(ModuleNumber::kExecutionAdapter, kErrorCodeEnd);

  // 交易引擎模块（BBB 从 100 起）
  static constexpr std::int32_t kEngineCommonStart = MakeErrorCode(ModuleNumber::kEngineCommon, kErrorCodeStart);
  static constexpr std::int32_t kEngineCommonEnd = MakeErrorCode(ModuleNumber::kEngineCommon, kErrorCodeEnd);

  static constexpr std::int32_t kAccountStart = MakeErrorCode(ModuleNumber::kAccount, kErrorCodeStart);
  static constexpr std::int32_t kAccountEnd = MakeErrorCode(ModuleNumber::kAccount, kErrorCodeEnd);

  static constexpr std::int32_t kEventBusStart = MakeErrorCode(ModuleNumber::kEventBus, kErrorCodeStart);
  static constexpr std::int32_t kEventBusEnd = MakeErrorCode(ModuleNumber::kEventBus, kErrorCodeEnd);

  static constexpr std::int32_t kComplianceStart = MakeErrorCode(ModuleNumber::kCompliance, kErrorCodeStart);
  static constexpr std::int32_t kComplianceEnd = MakeErrorCode(ModuleNumber::kCompliance, kErrorCodeEnd);

  static constexpr std::int32_t kExecutionStart = MakeErrorCode(ModuleNumber::kExecution, kErrorCodeStart);
  static constexpr std::int32_t kExecutionEnd = MakeErrorCode(ModuleNumber::kExecution, kErrorCodeEnd);

  static constexpr std::int32_t kOrderStart = MakeErrorCode(ModuleNumber::kOrder, kErrorCodeStart);
  static constexpr std::int32_t kOrderEnd = MakeErrorCode(ModuleNumber::kOrder, kErrorCodeEnd);

  static constexpr std::int32_t kMarketStart = MakeErrorCode(ModuleNumber::kMarket, kErrorCodeStart);
  static constexpr std::int32_t kMarketEnd = MakeErrorCode(ModuleNumber::kMarket, kErrorCodeEnd);

  static constexpr std::int32_t kPositionStart = MakeErrorCode(ModuleNumber::kPosition, kErrorCodeStart);
  static constexpr std::int32_t kPositionEnd = MakeErrorCode(ModuleNumber::kPosition, kErrorCodeEnd);

  static constexpr std::int32_t kRiskStart = MakeErrorCode(ModuleNumber::kRisk, kErrorCodeStart);
  static constexpr std::int32_t kRiskEnd = MakeErrorCode(ModuleNumber::kRisk, kErrorCodeEnd);

  static constexpr std::int32_t kStrategyEngineStart = MakeErrorCode(ModuleNumber::kStrategyEngine, kErrorCodeStart);
  static constexpr std::int32_t kStrategyEngineEnd = MakeErrorCode(ModuleNumber::kStrategyEngine, kErrorCodeEnd);

  // 支撑服务（BBB 从 200 起）
  static constexpr std::int32_t kServiceCommonStart = MakeErrorCode(ModuleNumber::kServiceCommon, kErrorCodeStart);
  static constexpr std::int32_t kServiceCommonEnd = MakeErrorCode(ModuleNumber::kServiceCommon, kErrorCodeEnd);

  static constexpr std::int32_t kAuditStart = MakeErrorCode(ModuleNumber::kAudit, kErrorCodeStart);
  static constexpr std::int32_t kAuditEnd = MakeErrorCode(ModuleNumber::kAudit, kErrorCodeEnd);

  static constexpr std::int32_t kBacktestStart = MakeErrorCode(ModuleNumber::kBacktest, kErrorCodeStart);
  static constexpr std::int32_t kBacktestEnd = MakeErrorCode(ModuleNumber::kBacktest, kErrorCodeEnd);

  static constexpr std::int32_t kBackupStart = MakeErrorCode(ModuleNumber::kBackup, kErrorCodeStart);
  static constexpr std::int32_t kBackupEnd = MakeErrorCode(ModuleNumber::kBackup, kErrorCodeEnd);

  static constexpr std::int32_t kConfigStart = MakeErrorCode(ModuleNumber::kConfig, kErrorCodeStart);
  static constexpr std::int32_t kConfigEnd = MakeErrorCode(ModuleNumber::kConfig, kErrorCodeEnd);

  static constexpr std::int32_t kHistoryMarketStart = MakeErrorCode(ModuleNumber::kHistoryMarket, kErrorCodeStart);
  static constexpr std::int32_t kHistoryMarketEnd = MakeErrorCode(ModuleNumber::kHistoryMarket, kErrorCodeEnd);

  static constexpr std::int32_t kHistoryOrderStart = MakeErrorCode(ModuleNumber::kHistoryOrder, kErrorCodeStart);
  static constexpr std::int32_t kHistoryOrderEnd = MakeErrorCode(ModuleNumber::kHistoryOrder, kErrorCodeEnd);

  static constexpr std::int32_t kLogStart = MakeErrorCode(ModuleNumber::kLog, kErrorCodeStart);
  static constexpr std::int32_t kLogEnd = MakeErrorCode(ModuleNumber::kLog, kErrorCodeEnd);

  static constexpr std::int32_t kMonitorStart = MakeErrorCode(ModuleNumber::kMonitor, kErrorCodeStart);
  static constexpr std::int32_t kMonitorEnd = MakeErrorCode(ModuleNumber::kMonitor, kErrorCodeEnd);

  static constexpr std::int32_t kRegistryStart = MakeErrorCode(ModuleNumber::kRegistry, kErrorCodeStart);
  static constexpr std::int32_t kRegistryEnd = MakeErrorCode(ModuleNumber::kRegistry, kErrorCodeEnd);

  static constexpr std::int32_t kStrategyStart = MakeErrorCode(ModuleNumber::kStrategy, kErrorCodeStart);
  static constexpr std::int32_t kStrategyEnd = MakeErrorCode(ModuleNumber::kStrategy, kErrorCodeEnd);
};

}  // namespace qtrade

#endif  // QTRADE_ERROR_CODE_CODE_SEGMENT_HPP_
