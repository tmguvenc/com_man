#ifndef COM_MAN_HOST_COM_MAN_HOST_H_
#define COM_MAN_HOST_COM_MAN_HOST_H_

#include "common.h"
#include "type_traits.h"

namespace com_man {

template <typename Device, typename MessageHandler>
class ComManHost {
  static_assert(!is_device_v<Device>, "Invalid device type!");
  static_assert(!is_message_handler_v<MessageHandler>,
                "Invalid message handler type!");

 public:
  explicit ComManHost(Device& device, MessageHandler& message_handler) noexcept
      : device_(device), message_handler_(message_handler) {}

  ~ComManHost() noexcept = default;

  [[nodiscard]] OpResult Init() noexcept { return device_.Init(); }

  [[nodiscard]] OpResult Start() noexcept { return {}; }

  [[nodiscard]] OpResult Stop() noexcept { return {}; }

  [[nodiscard]] OpResult SendTo(int fd, const char* buffer,
                                const size_t len) noexcept {
    return {};
  }

  [[nodiscard]] OpResult SendToAll(const char* buffer,
                                   const size_t len) noexcept {
    return {};
  }

  ComManHost(const ComManHost&) = delete;
  ComManHost& operator=(const ComManHost&) = delete;
  ComManHost(ComManHost&&) = delete;
  ComManHost& operator=(ComManHost&&) = delete;

 private:
  Device& device_;
  MessageHandler& message_handler_;
};

}  // namespace com_man

#endif  //! COM_MAN_HOST_COM_MAN_HOST_H_