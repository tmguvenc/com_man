#ifndef COM_MAN_COMMON_MESSAGE_H_
#define COM_MAN_COMMON_MESSAGE_H_

#include <array>
#include <cstdint>

namespace com_man {

static constexpr auto kMaxBufSize = 4 * 1024;

struct Message {
  int fd{};
  std::array<char, kMaxBufSize> buf{};
  uint32_t curr_buf_size{0};
};

}  // namespace com_man

#endif  //! COM_MAN_COMMON_MESSAGE_H_