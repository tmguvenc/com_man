#ifndef COM_MAN_COMMON_MESSAGE_HANDLER_H_
#define COM_MAN_COMMON_MESSAGE_HANDLER_H_

#include "common.h"
#include "message.h"

namespace com_man {

class IMessageHandler {
 public:
  virtual ~IMessageHandler() = default;
  [[nodiscard]] virtual OpResult Execute(Message&& received_mes) = 0;
};

}  // namespace com_man

#endif  //! COMMON_MESSAGE_HANDLER_H_