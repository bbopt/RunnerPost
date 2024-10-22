#ifdef _MSC_VER
#define RUNNERPOST_UNUSED(x)
#define RUNNERPOST_PRETTY_FUNCTION __FUNCSIG__
#else
#define RUNNERPOST_UNUSED(x) x __attribute__((unused))
#define RUNNERPOST_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

#ifdef _MSC_VER
#pragma warning(disable:4275)
#pragma warning(disable:4251)
#endif

#if defined(_WIN32) && !defined(RUNNERPOST_STATIC_BUILD)
#ifdef DLL_UTIL_EXPORTS
# define DLL_API __declspec(dllexport)
#else
# define DLL_UTIL_API __declspec(dllimport)
#endif
#else
#define DLL_API
#endif
