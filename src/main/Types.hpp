#include <vector>
#include <string>

#define DECLARE_HUNGARIAN(TYPE)                          \
	typedef       TYPE*                     p##TYPE;     \
	typedef       TYPE&                     r##TYPE;     \
	typedef const TYPE                      c##TYPE;     \
	typedef const TYPE*                     pc##TYPE;    \
	typedef const TYPE&                     rc##TYPE;    \
	typedef       std::vector<TYPE>         v##TYPE;     \
	typedef       std::vector<const TYPE>   vc##TYPE;    \
	typedef       std::vector<TYPE*>        vp##TYPE;    \
	typedef       std::vector<const TYPE*>  vpc##TYPE;   \
	typedef       std::vector<TYPE>&        rv##TYPE;    \
	typedef       std::vector<const TYPE>&  rvc##TYPE;   \
	typedef       std::vector<TYPE*>&       rvp##TYPE;   \
	typedef       std::vector<const TYPE*>& rvpc##TYPE;  \
	typedef const std::vector<TYPE>         cv##TYPE;    \
	typedef const std::vector<const TYPE>   cvc##TYPE;   \
	typedef const std::vector<TYPE*>        cvp##TYPE;   \
	typedef const std::vector<const TYPE*>  cvpc##TYPE;  \
	typedef const std::vector<TYPE>&        rcv##TYPE;   \
	typedef const std::vector<const TYPE>&  rcvc##TYPE;  \
	typedef const std::vector<TYPE*>&       rcvp##TYPE;  \
	typedef const std::vector<const TYPE*>& rcvpc##TYPE;

#define DECLARE_CLASS(CLS)                               \
	class CLS;                                           \
	DECLARE_HUNGARIAN(CLS)

#define DECLARE_STRUCT(STR)                              \
	struct STR;                                          \
	DECLARE_HUNGARIAN(STR)

typedef float            Float;  DECLARE_HUNGARIAN(Float) 
typedef int              Int;    DECLARE_HUNGARIAN(Int)
typedef bool             Bool;   DECLARE_HUNGARIAN(Bool)
typedef double           Double; DECLARE_HUNGARIAN(Double)
typedef char             Char;   DECLARE_HUNGARIAN(Char)
typedef std::string      String; DECLARE_HUNGARIAN(String)

//------------Unix: 32-bit/64-bit compatibility------------
#ifndef WIN32
#include <stdint.h>
typedef int64_t          Int64;  DECLARE_HUNGARIAN(Int64)
typedef uint64_t         Uint64; DECLARE_HUNGARIAN(Uint64)
typedef int32_t          Int32;  DECLARE_HUNGARIAN(Int32)
typedef uint32_t         Uint32; DECLARE_HUNGARIAN(Uint32)
typedef int16_t          Int16;  DECLARE_HUNGARIAN(Int16)
typedef uint16_t         Uint16; DECLARE_HUNGARIAN(Uint16)
typedef char             Int8;   DECLARE_HUNGARIAN(Int8)
typedef unsigned char    Uint8;  DECLARE_HUNGARIAN(Uint8)
//------------Win: 32-bit/64-bit compatibility-------------
#else
typedef __int64          Int64;  DECLARE_HUNGARIAN(Int64)
typedef unsigned __int64 Uint64; DECLARE_HUNGARIAN(Uint64)
typedef __int32          Int32;  DECLARE_HUNGARIAN(Int32)
typedef unsigned int     Uint32; DECLARE_HUNGARIAN(Uint32)
typedef __int16          Int16;  DECLARE_HUNGARIAN(Int16)
typedef unsigned __int16 Uint16; DECLARE_HUNGARIAN(Uint16)
typedef __int8           Int8;   DECLARE_HUNGARIAN(Int8)
typedef unsigned __int8  Uint8;  DECLARE_HUNGARIAN(Uint8)
#endif // WIN32
