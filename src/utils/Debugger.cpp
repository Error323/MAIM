#include "./Debugger.hpp"

#include <iostream>

pcChar NC = "\E[0m"; // No Color (reset to default)
pcChar HOME_CURSOR  = "\E[0;0H"; // Place the cursor at 0;0 position.
pcChar CLEAR_SCREEN = "\E[2J";
pcChar FATAL = "\E[1m\E[37m\E[41m"; // Red background, white bold text

Debugger* Debugger::GetInstance() {
	static Debugger* d = NULL;

	if (d == NULL) {
		d = new Debugger();
	}

	return d;
}

void Debugger::FreeInstance(Debugger* d) {
	delete d;
}



#ifdef DEBUG
Debugger::Debugger(): mEnabled(false) {
}

Debugger::~Debugger() {
}

bool Debugger::Begin(pcChar filename, int line) {
	snprintf(gDebugMessageKey, 1024, "%s:%d", filename, line);

	mKey = std::string(gDebugMessageKey);
	std::map<std::string, bool>::iterator i = mIgnoreForever.find(mKey);

	if (i == mIgnoreForever.end())
		mIgnoreForever[mKey] = false;

	// If we are ignored don't start
	if (mIgnoreForever[mKey])
		return false;

	mEnabled = true;
	mMessage.clear();

	return true;
}

void Debugger::Print(pcChar msg) {
	mMessage += std::string(msg);
#if (!defined(WIN32) && !defined(__powerpc64__))
	fprintf(stderr, "%s%s%s%s", NC, FATAL, msg, NC);
#else
	fprintf(stderr, msg);
#endif
}

bool Debugger::End() {
	bool breakPoint = true;

	Print("\n\nType `continue` in gdb to ignore.");

	mEnabled = false;
	mMessage.clear();

	return breakPoint;
}

void Debugger::DumpStack(pChar* symbols, size_t size) {
	if (symbols != NULL) {
		Print("\n");

		for (size_t i = 0; i < size; i++) {
			Print(symbols[i]);
			Print("\n");
		}
	}
}
#endif // DEBUG
