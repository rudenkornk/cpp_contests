module;

#include <cstddef>
#include <cstdint>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

export module utils:type_traits;

export namespace cpp_contests {

template <typename T, template <typename...> typename Template> struct IsInstanceOf : std::false_type {};
template <template <typename...> typename Template, typename... Args>
struct IsInstanceOf<Template<Args...>, Template> : std::true_type {};

template <template <typename...> typename Template, typename Instance>
constexpr inline auto kIsInstanceOf = IsInstanceOf<Instance, Template>::value;

template <typename Callable> struct CallableTraits final {
private:
  enum class CallableKind : std::uint8_t { Function, Method, Lambda };

  template <typename CallableType> struct FunctionArgTypes;
  template <typename CallableType, typename... Args> struct FunctionArgTypes<CallableType(Args...)> {
    using types = std::tuple<CallableType, Args...>;
  };
  template <typename CallableAddress> struct LambdaOrMethodArgTypes;
  template <typename CallableAddress, typename Result, typename... Args>
  struct LambdaOrMethodArgTypes<Result (CallableAddress::*)(Args...) const> {
    using types = std::tuple<Result, Args...>;
    auto constexpr static kIsCallableConst = true;
  };
  template <typename CallableAddress, typename Result, typename... Args>
  struct LambdaOrMethodArgTypes<Result (CallableAddress::*)(Args...) const noexcept> {
    using types = std::tuple<Result, Args...>;
    constexpr static auto kIsCallableConst = true;
  };
  template <typename CallableAddress, typename Result, typename... Args>
  struct LambdaOrMethodArgTypes<Result (CallableAddress::*)(Args...)> {
    using types = std::tuple<Result, Args...>;
    constexpr static auto kIsCallableConst = false;
  };
  template <typename CallableAddress, typename Result, typename... Args>
  struct LambdaOrMethodArgTypes<Result (CallableAddress::*)(Args...) noexcept> {
    using types = std::tuple<Result, Args...>;
    constexpr static auto kIsCallableConst = false;
  };

  template <typename CallableType, CallableKind> struct ArgTypes_;
  template <typename CallableType> struct ArgTypes_<CallableType, CallableKind::Function> {
    using types = FunctionArgTypes<CallableType>::types;
    constexpr static auto kIsCallableConst = true;
  };
  template <typename CallableType> struct ArgTypes_<CallableType, CallableKind::Lambda> {
    using types = LambdaOrMethodArgTypes<decltype(&CallableType::operator())>::types;
    constexpr static auto kIsCallableConst =
        LambdaOrMethodArgTypes<decltype(&CallableType::operator())>::kIsCallableConst;
  };
  template <typename CallableType> struct ArgTypes_<CallableType, CallableKind::Method> {
    using types = LambdaOrMethodArgTypes<CallableType>::types;
    constexpr static auto kIsCallableConst = LambdaOrMethodArgTypes<CallableType>::kIsCallableConst;
  };

  template <class CallableType> auto constexpr static get_callable_kind() -> CallableKind {
    if (std::is_function_v<CallableType>) {
      return CallableKind::Function;
    }
    if (std::is_class_v<CallableType>) {
      return CallableKind::Lambda;
    }
    return CallableKind::Method;
  }

  template <class CallableType, std::size_t... Indices>
  constexpr static auto generate_function_type(std::integer_sequence<std::size_t, Indices...>)
      -> std::function<typename CallableTraits<CallableType>::return_type(
          typename CallableTraits<CallableType>::template arg_type<Indices>...)>;

  using raw_callable = std::remove_cv_t<std::remove_reference_t<Callable>>;
  using arg_types = ArgTypes_<raw_callable, get_callable_kind<raw_callable>()>;
  using types = arg_types::types;

public:
  constexpr static std::size_t kNTypes = std::tuple_size_v<types>;
  constexpr static std::size_t kNArguments = kNTypes - 1;
  template <std::size_t N> using type = std::tuple_element_t<N, types>;
  template <std::size_t N> using arg_type = std::tuple_element_t<N + 1, types>;
  using return_type = std::tuple_element_t<0, types>;
  using std_function = decltype(generate_function_type<raw_callable>(std::make_index_sequence<kNArguments>{}));
  template <std::size_t N> constexpr static bool kIsLValueReference = std::is_lvalue_reference_v<type<N>>;
  template <std::size_t N> constexpr static bool kIsRValueReference = std::is_rvalue_reference_v<type<N>>;
  template <std::size_t N> constexpr static bool kIsReference = std::is_reference_v<type<N>>;
  template <std::size_t N> constexpr static bool kIsValue = !kIsReference<N> && !std::is_void_v<type<N>>;
  template <std::size_t N> constexpr static bool kIsConst = std::is_const_v<std::remove_reference_t<type<N>>>;
  constexpr static bool kIsCallableConst = arg_types::kIsCallableConst;

  template <std::size_t N>
  constexpr static auto forward(std::add_lvalue_reference_t<std::remove_reference_t<type<N>>> arg) noexcept
      -> decltype(auto) {
    if constexpr (kIsValue<N> || kIsRValueReference<N>) {
      return std::move(arg);
    } else {
      return arg;
    }
  }
};
} // namespace cpp_contests
