#include <qtrade/adapter/execution_adapter.hpp>

namespace qtrade::adapter {

namespace {

class MockExecutionAdapter final : public IExecutionAdapter {
 public:
  ErrorCode Connect(const ExecutionAdapterConfig& config) override {
    (void)config;
    return ErrorCode::kSuccess;
  }

  void Disconnect() override {}

  ErrorCode SendOrder(const Order& order) override {
    (void)order;
    return ErrorCode::kSuccess;
  }

  ErrorCode CancelOrder(const std::string& order_id) override {
    (void)order_id;
    return ErrorCode::kSuccess;
  }

  bool IsConnected() const override { return false; }
};

}  // namespace

void LinkMockExecutionAdapterForBuild() {
  MockExecutionAdapter adapter;
  (void)adapter;
}

}  // namespace qtrade::adapter
