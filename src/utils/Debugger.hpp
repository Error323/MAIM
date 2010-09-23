#ifndef MAI_DEBUGGER_HDR
#define MAI_DEBUGGER_HDR

#include "../main/Types.hpp"

DECLARE_CLASS(Debugger)

#ifdef DEBUG
#include <cstdio>
#include <cstdlib>

#include <map>
#include <string>

#if (!defined(WIN32) && !defined(__powerpc64__))
#include <execinfo.h>
#endif

#ifdef WIN32
	#define BREAKPOINT __debugbreak()
#elif defined(__powerpc64__)
	#define BREAKPOINT asm volatile ("tw 31,1,1")
#else
	#define BREAKPOINT asm("int $3")
#endif

#define BEGIN() Debugger::GetInstance()->Begin(__FILE__, __LINE__)
#define END() if (Debugger::GetInstance()->End()) BREAKPOINT
#define FORMAT_STRING "***ASSERTION FAILED***\n\n\tfile\t%s\n\tline\t%d\n\tfunc\t%s\n\tcond\t%s\n"
#define FORMAT_STRING_MSG FORMAT_STRING"\ttext\t"

#define BACKTRACE()                                          \
	do {                                                     \
		void* addresses[16];                                 \
		size_t size = backtrace(addresses, 16);              \
		char** symbols = backtrace_symbols(addresses, size); \
		Debugger::GetInstance()->DumpStack(symbols, size);   \
		free(symbols);                                       \
	} while (0)

#define MAI_ASSERT(cond)                                                          \
	do {                                                                          \
		if ( !(cond) && BEGIN() ) {                                               \
			FATAL(FORMAT_STRING, __FILE__, __LINE__, __PRETTY_FUNCTION__, #cond); \
			BACKTRACE();                                                          \
			END();                                                                \
		}                                                                         \
	} while (0)

#define MAI_ASSERT_MSG(cond, ...)                                                     \
	do {                                                                              \
		if ( !(cond) && BEGIN() ) {                                                   \
			FATAL(FORMAT_STRING_MSG, __FILE__, __LINE__, __PRETTY_FUNCTION__, #cond); \
			FATAL(__VA_ARGS__);                                                       \
			FATAL("\n");                                                              \
			BACKTRACE();                                                              \
			END();                                                                    \
		}                                                                             \
	} while (0)

#define FATAL(...)                              \
	do {                                        \
		char buffer[2048];                      \
		snprintf(buffer, 2048, __VA_ARGS__);    \
		Debugger::GetInstance()->Print(buffer); \
	} while (0)


class Debugger {
public:
	Debugger();
	~Debugger();

	bool Begin(pcChar, int);
	bool End();
	void Print(pcChar);
	void DumpStack(pChar*, size_t);

	pcChar GetMessage() const { return mMessage.c_str(); }
	bool IsEnabled() const { return mEnabled; }

	static pDebugger GetInstance();
	static void FreeInstance(pDebugger);

private:
	std::string mMessage;

	bool mEnabled;
};

#else // DEBUG
#define MAI_ASSERT(c)
#define MAI_ASSERT_MSG(c, ...)

// dummy
class Debugger {
public:
	static pDebugger GetInstance();
	static void FreeInstance(pDebugger);

	pcChar GetMessage() const { return ""; }
	bool IsEnabled() const { return false; }
};
#endif // DEBUG

#endif // MAI_DEBUGGER_HDR
