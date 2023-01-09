// Source & credit:
// https://www.fluentcpp.com/2019/08/30/how-to-disable-a-warning-in-cpp/

// clang-format off
// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#if defined(_MSC_VER)
  #define DISABLE_WARNING_PUSH           __pragma(warning( push ))
  #define DISABLE_WARNING_POP            __pragma(warning( pop ))
  #define DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))

  #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    DISABLE_WARNING(4100)
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION            DISABLE_WARNING(4505)
  // other warnings you want to deactivate...
#elif defined(__GNUC__) || defined(__clang__)
  #define DO_PRAGMA(X) _Pragma(#X)
  #define DISABLE_WARNING_PUSH           DO_PRAGMA(GCC diagnostic push)
  #define DISABLE_WARNING_POP            DO_PRAGMA(GCC diagnostic pop)
  #define DISABLE_WARNING(warningName)   DO_PRAGMA(GCC diagnostic ignored #warningName)

  #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    DISABLE_WARNING(-Wunused-parameter)
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION            DISABLE_WARNING(-Wunused-function)
  // other warnings you want to deactivate...
#endif
// NOLINTEND(cppcoreguidelines-macro-usage)
// clang-format on

/* example use:
DISABLE_WARNING_PUSH
DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER

void f(int a, int b)
{
    std::cout << a << '\n';
    // we are not using b!
}

DISABLE_WARNING_POP
*/
