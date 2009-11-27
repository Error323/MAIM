### Generic native Skirmish AI config
 
set(mySourceDirRel "")
set(additionalSources ${CUtils_SRC})
set(additionalCompileFlags "")
set(additionalLibraries ${LegacyCpp_AIWRAPPER_TARGET} ${Lua} ${SDL_LIBRARY} )
 
ConfigureNativeSkirmishAI(mySourceDirRel additionalSources additionalCompileFlags additionalLibraries)
