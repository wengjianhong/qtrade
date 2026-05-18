#ifndef QUANT_TRADING_ENGINE_EVENT_BUS_HPP_
#define QUANT_TRADING_ENGINE_EVENT_BUS_HPP_

namespace quant::trading::engine::event_bus {

class EventBus {
 public:
  void Start();
  void Stop();
};

}  // namespace quant::trading::engine::event_bus

#endif  // QUANT_TRADING_ENGINE_EVENT_BUS_HPP_
