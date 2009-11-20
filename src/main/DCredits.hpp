#ifndef CREDITS
#define CREDITS

#include "./Exports.hpp"

#define AI_CODENAME    std::string("Initial")
#define AI_NAME(t)     std::string("MAI ") + aiexport_getVersion(t) + " - " + AI_CODENAME
#define AI_DATE        __DATE__
#define AI_VERSION(t)  (AI_NAME(t) + " (built " + AI_DATE + ")")
#define AI_CREDITS     "(developed by Error323 and Kloot)"

#endif
