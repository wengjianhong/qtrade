/// @file      mock_execution_adapter.cpp
/// @brief     模拟交易网关实现
#include <qtrade_sdk/trader/trader_api.hpp>

#include <string>

namespace qtrade_sdk::trader {

namespace {

class MockTradeGateway final : public ITradeGateway {
 public:
  void SetCpuAffinity(std::int32_t thread1_cpu_core_id, std::int32_t thread2_cpu_core_id) override {
    (void)thread1_cpu_core_id;
    (void)thread2_cpu_core_id;
  }

  std::string GetErrMsgByCode(std::int32_t error_code, std::uint64_t session_id) override {
    (void)error_code;
    (void)session_id;
    return {};
  }

  void Release() override {}

  std::string GetTradingDay() const override { return trading_day_; }

  void RegisterSpi(TraderSpi& spi) override { spi_ = &spi; }

  void UnregisterSpi() override { spi_ = nullptr; }

  const RspInfo* GetApiLastError() const override { return &last_error_; }

  std::string GetApiVersion() const override { return api_version_; }

  std::uint8_t GetClientIDByEMTID(std::uint64_t order_emt_id) override {
    (void)order_emt_id;
    return 0;
  }

  std::string GetAccountByEMTID(std::uint64_t order_emt_id) override {
    (void)order_emt_id;
    return account_id_;
  }

  void SubscribePublicTopic(ResumeType resume_type) override { (void)resume_type; }

  void SetSoftwareVersion(const std::string& version) override { software_version_ = version; }

  void SetHeartBeatInterval(std::uint32_t interval) override { (void)interval; }

  qtrade::ErrorCode Connect(const ConnectRequest& request) override {
    (void)request;
    return qtrade::ErrorCode::kSuccess;
  }

  void Disconnect() override {}

  bool IsConnected() const override { return false; }

  std::uint64_t Login(const std::string& ip,
                      int port,
                      const std::string& user,
                      const std::string& password,
                      ProtocolType sock_type,
                      const std::string& local_ip,
                      const UserTerminalInfo* terminal_info) override {
    (void)ip;
    (void)port;
    (void)user;
    (void)password;
    (void)sock_type;
    (void)local_ip;
    (void)terminal_info;
    return 0;
  }

  int Logout(std::uint64_t session_id) override {
    (void)session_id;
    return 0;
  }

  bool IsServerRestart(std::uint64_t session_id) override {
    (void)session_id;
    return false;
  }

  std::uint64_t InsertOrder(const OrderRequest& order, std::uint64_t session_id) override {
    (void)session_id;
    return order.order_emt_id;
  }

  qtrade::ErrorCode SendOrder(const OrderRequest& request) override {
    (void)request;
    return qtrade::ErrorCode::kSuccess;
  }

  std::uint64_t CancelOrder(std::uint64_t order_emt_id, std::uint64_t session_id) override {
    (void)order_emt_id;
    (void)session_id;
    return 0;
  }

  qtrade::ErrorCode CancelOrder(const CancelOrderRequest& request) override {
    (void)request;
    return qtrade::ErrorCode::kSuccess;
  }

  int QueryOrderByEMTID(std::uint64_t order_emt_id, std::uint64_t session_id, int request_id) override {
    (void)order_emt_id;
    (void)session_id;
    (void)request_id;
    return -1;
  }

  int QueryOrders(const QueryOrdersRequest& query_param, std::uint64_t session_id, int request_id) override {
    (void)query_param;
    (void)session_id;
    (void)request_id;
    return -1;
  }

  qtrade::ErrorCode QueryOrders(const QueryOrdersRequest& request, QueryOrdersResponse& response) override {
    (void)request;
    response.orders.clear();
    return qtrade::ErrorCode::kNotSupported;
  }

  int QueryUnfinishedOrders(std::uint64_t session_id, int request_id) override {
    (void)session_id;
    (void)request_id;
    return -1;
  }

  int QueryOrdersByPage(const QueryOrdersByPageRequest& query_param,
                        std::uint64_t session_id,
                        int request_id) override {
    (void)query_param;
    (void)session_id;
    (void)request_id;
    return -1;
  }

  int QueryTradesByEMTID(std::uint64_t order_emt_id, std::uint64_t session_id, int request_id) override {
    (void)order_emt_id;
    (void)session_id;
    (void)request_id;
    return -1;
  }

  int QueryTrades(const QueryTradesRequest& query_param, std::uint64_t session_id, int request_id) override {
    (void)query_param;
    (void)session_id;
    (void)request_id;
    return -1;
  }

  qtrade::ErrorCode QueryTrades(const QueryTradesRequest& request, QueryTradesResponse& response) override {
    (void)request;
    response.trades.clear();
    return qtrade::ErrorCode::kNotSupported;
  }

  int QueryPosition(const std::string& ticker,
                    std::uint64_t session_id,
                    int request_id,
                    MarketType market) override {
    (void)ticker;
    (void)session_id;
    (void)request_id;
    (void)market;
    return -1;
  }

  qtrade::ErrorCode QueryPositions(const QueryPositionRequest& request, QueryPositionResponse& response) override {
    (void)request;
    response.positions.clear();
    return qtrade::ErrorCode::kNotSupported;
  }

  int QueryAsset(std::uint64_t session_id, int request_id) override {
    (void)session_id;
    (void)request_id;
    return -1;
  }

  qtrade::ErrorCode QueryAsset(const QueryAssetRequest& request, QueryAssetResponse& response) override {
    (void)request;
    response.asset = {};
    return qtrade::ErrorCode::kNotSupported;
  }

  std::uint64_t FundTransfer(const FundTransferRequest& fund_transfer, std::uint64_t session_id) override {
    (void)fund_transfer;
    (void)session_id;
    return 0;
  }

  int QueryOtherServerFund(const FundQueryRequest& query_param, std::uint64_t session_id, int request_id) override {
    (void)query_param;
    (void)session_id;
    (void)request_id;
    return -1;
  }

  std::uint64_t CreditQuotaTransfer(const QuotaTransferRequest& quota_transfer, std::uint64_t session_id) override {
    (void)quota_transfer;
    (void)session_id;
    return 0;
  }

  int QueryCreditQuotaTransfer(const QueryQuotaTransferLogRequest& query_param,
                               std::uint64_t session_id,
                               int request_id) override {
    (void)query_param;
    (void)session_id;
    (void)request_id;
    return -1;
  }

  int QueryFundTransfer(const QueryFundTransferLogRequest& query_param,
                        std::uint64_t session_id,
                        int request_id) override {
    (void)query_param;
    (void)session_id;
    (void)request_id;
    return -1;
  }

  void SetOrderCallback(OrderCallback cb) override { on_order_ = std::move(cb); }

  void SetTradeCallback(TradeCallback cb) override { on_trade_ = std::move(cb); }

 private:
  TraderSpi* spi_ = nullptr;
  RspInfo last_error_;
  OrderCallback on_order_;
  TradeCallback on_trade_;
  std::string trading_day_;
  std::string api_version_{"mock"};
  std::string account_id_;
  std::string software_version_;
};

}  // namespace

void LinkMockExecutionAdapterForBuild() {
  MockTradeGateway gateway;
  (void)gateway;
}

}  // namespace qtrade_sdk::trader
