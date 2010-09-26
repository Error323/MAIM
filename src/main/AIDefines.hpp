#ifndef AI_DEFINES_HDR
#define AI_DEFINES_HDR

// constants
#define TEAM_SU_INT_F 32.0f
#define TEAM_SU_INT_I 32

// directories (relative to "AI/Skirmish/$NAME/$VERSION/")
#define AI_ROOT_DIR  ""
#define AI_LOG_DIR   std::string(AI_ROOT_DIR) + "logs/"
#define AI_CFG_DIR   std::string(AI_ROOT_DIR) + "configs/"
#define AI_MTL_DIR   std::string(AI_ROOT_DIR) + "metal/"
#define AI_GRP_DIR   std::string(AI_ROOT_DIR) + "graphs/"
#define AI_LUA_DIR   std::string(AI_ROOT_DIR) + "modules/"

#endif
