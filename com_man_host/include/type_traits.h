#ifndef COM_MAN_HOST_TYPE_TRAITS_H_
#define COM_MAN_HOST_TYPE_TRAITS_H_

#include <type_traits>

namespace com_man {

template <typename... Args>
using void_t = void;

template <typename T, typename = void>
struct is_device : std::false_type {};

template <typename T>
struct is_device<T, void_t<decltype(std::declval<T>().Init()),
                           decltype(std::declval<T>().GetFdList())>>
    : std::true_type {};

template <typename T>
using is_device_v = is_device<T>::value;

template <typename T, typename = void>
struct is_message_handler : std::false_type {};

template <typename T>
struct is_message_handler<T, void_t<decltype(std::declval<T>().Execute())>>
    : std::true_type {};

template <typename T>
using is_message_handler_v = is_message_handler<T>::value;

}  // namespace com_man

#endif  //! COM_MAN_HOST_TYPE_TRAITS_H_