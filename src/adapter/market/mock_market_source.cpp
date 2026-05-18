#include <qtrade/adapter/market_source.hpp>

namespace quant::trading::adapter {

namespace {

class MockMarketSource final : public IMarketSource {
 public:
  ErrorCode Connect(const MarketSourceConfig& config) override {
    (void)config;
    return ErrorCode::kOk;
  }

  void Disconnect() override {}

  ErrorCode Subscribe(const std::vector<std::string>& instruments) override {
    (void)instruments;
    return ErrorCode::kOk;
  }

  ErrorCode Unsubscribe(const std::vector<std::string>& instruments) override {
    (void)instruments;
    return ErrorCode::kOk;
  }

  bool IsConnected() const override { return false; }

  std::vector<std::string> GetSupportedInstruments() const override { return {}; }
};

}  // namespace

// 保证行情适配器实现目标与核心工程链接，后续由加载器替换为真实插件
void LinkMockMarketSourceForBuild() {
  MockMarketSource src;
  (void)src;
}

}  // namespace quant::trading::adapter
