#ifndef AI_DEFINES_HDR
#define AI_DEFINES_HDR

// directories (relative to "AI/Skirmish/$NAME/$VERSION/")
#define AI_ROOT_DIR  ""
#define AI_LOG_DIR   std::string(AI_ROOT_DIR) + "logs/"
#define AI_CFG_DIR   std::string(AI_ROOT_DIR) + "configs/"
#define AI_MTL_DIR   std::string(AI_ROOT_DIR) + "metal/"
#define AI_GRP_DIR   std::string(AI_ROOT_DIR) + "graphs/"
#define AI_LUA_DIR   std::string(AI_ROOT_DIR) + "lua/"


#define AI_CATCH_EXCEPTIONS 1

#if (AI_CATCH_EXCEPTIONS != 0)
  #define AI_BEG_EXCEPTION     try {
  #define AI_END_EXCEPTION(s)  } catch (std::exception& e) {               \
                                   LOG_ERROR(s << "[" << e.what() << "]"); \
                               }
#else
  #define AI_BEG_EXCEPTION
  #define AI_END_EXCEPTION(s)
#endif


#define MAP_WORLD_2_HEIGHT(size) ((size) >> 3)
#define MAP_HEIGHT_2_WORLD(size) ((size) << 3)
#define MAP_METAL_2_HEIGHT(size) ((size) << 1)
#define MAP_HEIGHT_2_METAL(size) ((size) >> 1)
#define MAP_SLOPE_2_HEIGHT(size) ((size) << 1)
#define MAP_HEIGHT_2_SLOPE(size) ((size) >> 1)

#define MAP_WORLD_2_METAL(size) MAP_HEIGHT_2_METAL(MAP_WORLD_2_HEIGHT(size))
#define MAP_METAL_2_WORLD(size) MAP_HEIGHT_2_WORLD(MAP_METAL_2_HEIGHT(size))
#define MAP_WORLD_2_SLOPE(size) MAP_HEIGHT_2_SLOPE(MAP_WORLD_2_HEIGHT(size))
#define MAP_SLOPE_2_WORLD(size) MAP_HEIGHT_2_WORLD(MAP_SLOPE_2_HEIGHT(size))


#endif
