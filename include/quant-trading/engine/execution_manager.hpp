#ifndef QUANT_TRADING_ENGINE_EXECUTION_MANAGER_HPP_
#define QUANT_TRADING_ENGINE_EXECUTION_MANAGER_HPP_

namespace quant::trading::engine::ems {

class ExecutionManager {
 public:
  void Start();
  void Stop();
};

}  // namespace quant::trading::engine::ems

#endif  // QUANT_TRADING_ENGINE_EXECUTION_MANAGER_HPP_
