#include <quant-trading/adapter/execution_adapter.hpp>

namespace quant::trading::adapter {

namespace {

class MockExecutionAdapter final : public IExecutionAdapter {
 public:
  ErrorCode Connect(const ExecutionAdapterConfig& config) override {
    (void)config;
    return ErrorCode::kOk;
  }

  void Disconnect() override {}

  ErrorCode SendOrder(const Order& order) override {
    (void)order;
    return ErrorCode::kOk;
  }

  ErrorCode CancelOrder(const std::string& order_id) override {
    (void)order_id;
    return ErrorCode::kOk;
  }

  bool IsConnected() const override { return false; }
};

}  // namespace

void LinkMockExecutionAdapterForBuild() {
  MockExecutionAdapter adapter;
  (void)adapter;
}

}  // namespace quant::trading::adapter
