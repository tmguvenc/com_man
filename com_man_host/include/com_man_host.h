#ifndef COM_MAN_HOST_COM_MAN_HOST_H_
#define COM_MAN_HOST_COM_MAN_HOST_H_

#include <poll.h>

#include <algorithm>
#include <atomic>
#include <future>
#include <vector>

#include "channel.h"
#include "common.h"
#include "type_traits.h"

namespace com_man {

template <typename Device, typename MessageHandler, typename Payload>
class ComManHost {
  static_assert(!is_device_v<Device>, "Invalid device type!");
  static_assert(!is_message_handler_v<MessageHandler>,
                "Invalid message handler type!");
  static_assert(!std::is_default_constructible_v<Payload>,
                "Payload must be default constructible");

  static constexpr auto kMaxFdCount = 100;

#pragma pack(push, 1)
  struct InternalPayload {
    std::array<int, kMaxFdCount> fd_list{};
    std::size_t fd_list_size{0};
    Payload payload{};
  };
#pragma pack(pop)

 public:
  explicit ComManHost(Device& device, MessageHandler& message_handler,
                      const size_t timeout_ms) noexcept
      : device_{device},
        message_handler_{message_handler},
        timeout_ms_{timeout_ms},
        run_{false} {
    std::tie(sender_, receiver_) = channels::Channel<InternalPayload>::Open();
  }

  ~ComManHost() noexcept = default;

  [[nodiscard]] OpResult Init() noexcept {
    if (const auto ret = device_.Init(); ret.has_value()) {
      return *ret;
    }
    initialized_ = true;
    return {};
  }

  [[nodiscard]] OpResult Start() noexcept {
    if (!initialized_) {
      return "Call init function before starting..";
    }

    receive_thread_ = std::async([&]() {
      run_.store(true);
      std::vector<pollfd> fd_list;
      for (const auto fd : device_.GetFdList()) {
        fd_list.push_back({.fd = fd, .events = POLLIN});
      }

      while (run_) {
        const auto ret = poll(fd_list.data(), fd_list.size(), timeout_ms_);
        if (ret < 0) {
          std::cerr << std::strerror(errno) << std::endl;
          continue;
        }

        std::for_each(fd_list.begin(), fd_list.end(), [&](const pollfd& pfd) {
          if (pfd.revents & POLLIN) {
            const auto mes = device_.ReadDataFrom(pfd.fd);
            if (const auto result = message_handler_.Execute(mes);
                result.has_value()) {
              std::cerr << *result << std::endl;
            }
          } else {
            const auto mes = device_.HandleUnreceivedDataFrom(pfd.fd);
            if (const auto result = message_handler_.Execute(mes);
                result.has_value()) {
              std::cerr << *result << std::endl;
            }
          }
        });
      }
    });

    return {};
  }

  [[nodiscard]] OpResult Stop() noexcept {
    if (!run_.load()) {
      return "Not started yet";
    }

    run_.store(false);

    if (!receive_thread_.valid()) {
      return "receiving thread is not valid";
    }

    receive_thread_.get();

    return {};
  }

  [[nodiscard]] OpResult SendTo(int fd, const Payload& payload) noexcept {
    return sender_->Send({
        .fd_list = {fd},
        .fd_list_size = 1,
        .payload = payload,
    });
  }

  ComManHost(const ComManHost&) = delete;
  ComManHost& operator=(const ComManHost&) = delete;
  ComManHost(ComManHost&&) = delete;
  ComManHost& operator=(ComManHost&&) = delete;

 private:
  Device& device_;
  MessageHandler& message_handler_;
  bool initialized_{false};
  const size_t timeout_ms_;
  std::atomic_bool run_{false};
  std::future<void> receive_thread_;
  std::unique_ptr<channels::Sender<InternalPayload>> sender_;
  std::unique_ptr<channels::Receiver<InternalPayload>> receiver_;
};

}  // namespace com_man

#endif  //! COM_MAN_HOST_COM_MAN_HOST_H_