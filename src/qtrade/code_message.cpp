#include <qtrade/error_code/code_message.hpp>

namespace qtrade {

const std::string_view GetErrorCodeMessage(ErrorCode code) {
  auto it = kErrorCodeMessages.find(code);
  if (it != kErrorCodeMessages.end()) {
    return it->second;
  }
  return "UnknownError";
}

}  // namespace qtrade