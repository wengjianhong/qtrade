/// @file      call_data_base.hpp
/// @brief     gRPC Async API CallData 基类
/// @author    wengjianhong
/// @date      2026-06-28
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_COMMON_GRPC_CALL_DATA_BASE_HPP_
#define QTRADE_COMMON_GRPC_CALL_DATA_BASE_HPP_

namespace qtrade::common::grpc_async {

/// @brief CompletionQueue tag 对应的异步 RPC 状态机入口
class CallDataBase {
 public:
  virtual ~CallDataBase() = default;

  /// @brief CQ 事件到达时由 CompletionQueueLoop 调用
  /// @param ok false 表示 RPC 已取消或失败
  virtual void Proceed(bool ok) = 0;
};

}  // namespace qtrade::common::grpc_async

#endif  // QTRADE_COMMON_GRPC_CALL_DATA_BASE_HPP_
