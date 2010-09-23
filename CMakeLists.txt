### Generic native Skirmish AI config
PROJECT(MAI)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#-------------------------------------------------------------------------------
# General Settings
# NOTE: we need -rdynamic for backtrace() to work properly
#-------------------------------------------------------------------------------
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_CXX_FLAGS "-Wall -fno-strict-aliasing")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -rdynamic -DDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -rdynamic -DDEBUG")

#-------------------------------------------------------------------------------
# Extract and set the version
#-------------------------------------------------------------------------------
EXECUTE_PROCESS(
	COMMAND "${MAI_SOURCE_DIR}/scripts/get-revision.sh"
	OUTPUT_VARIABLE GIT_VERSION
	ERROR_VARIABLE GIT_ERROR
	OUTPUT_STRIP_TRAILING_WHITESPACE
	)

IF(GIT_VERSION)
	SET(VERSION ${GIT_VERSION})
ELSE(NOT GIT_VERSION)
	SET(VERSION "0.0.0")
ENDIF(GIT_VERSION)

EXECUTE_PROCESS(
	COMMAND "${MAI_SOURCE_DIR}/scripts/update-version.sh" ${VERSION}
	OUTPUT_VARIABLE SET_VERSION
	ERROR_VARIABLE SET_VERSION_ERROR
	OUTPUT_STRIP_TRAILING_WHITESPACE
	)

IF(SET_VERSION_ERROR)
	MESSAGE(FATAL_ERROR ${SET_VERSION_ERROR})
ENDIF(SET_VERSION_ERROR)

CONFIGURE_FILE(
	"${MAI_SOURCE_DIR}/cmake/MAI.hpp.in"
	"${MAI_SOURCE_DIR}/src/main/MAI.hpp"
	)
 
set(mySourceDirRel "")
set(additionalSources ${CUtils_SRC})
set(additionalCompileFlags "")
set(additionalLibraries ${LegacyCpp_AIWRAPPER_TARGET} ${Lua} ${SDL_LIBRARY} )
 
ConfigureNativeSkirmishAI(mySourceDirRel additionalSources additionalCompileFlags additionalLibraries)
