#ifndef QTRADE_TRADING_ENGINE_EXECUTION_MANAGER_HPP_
#define QTRADE_TRADING_ENGINE_EXECUTION_MANAGER_HPP_

namespace qtrade::engine::ems {

class ExecutionManager {
 public:
  void Start();
  void Stop();
};

}  // namespace qtrade::engine::ems

#endif  // QTRADE_TRADING_ENGINE_EXECUTION_MANAGER_HPP_
