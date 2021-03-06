#pragma once
#include <string>
#include <regex>
#include <sstream>

#ifdef __GNUC__
#include <ext/hash_map>
namespace std { using namespace __gnu_cxx; }
#else
#include <hash_map>
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef NO_ID
#define NO_ID NULL
#endif

namespace playstate
{
	typedef char int8;
	typedef unsigned char uint8;

	typedef short int16;
	typedef unsigned short uint16;

	typedef int int32;
	typedef unsigned int uint32;

	typedef long long int64;
	typedef unsigned long long uint64;

	typedef float float32;
	typedef double float64;

	typedef unsigned char byte;

	typedef uint32 type_mask;

	typedef unsigned int script_ref;

#ifdef _UNICODE
	typedef std::wstring string;
	typedef wchar_t character;
	typedef std::wregex regex;
	typedef std::wstringstream stringstream;
#define SAFE_STRING(x) L##x
#else
	typedef std::string string;
	typedef char character;
	typedef std::regex regex;
	typedef std::stringstream stringstream;
#define SAFE_STRING(x) x
#endif
}

#ifndef ALIGN_DATA
#ifdef WIN32
#define ALIGN_DATA(x) __declspec(align(x))
#endif
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef BIT
#define BIT(x) 1 << x
#endif

#ifndef BIT_ISSET
#define BIT_ISSET(a, b) ( a & b ) != 0
#endif

#ifndef BIT_UNSET
#define BIT_UNSET(a, b) (a &= ~b)
#endif

#ifndef BIT_SET
#define BIT_SET(a, b) (a |= (b))
#endif

#ifndef BIT_ALL
#define BIT_ALL UINT_MAX
#endif

/* Constants rounded for 21 decimals. */
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

#ifndef M_LOG2E
#define M_LOG2E 1.44269504088896340736
#endif

#ifndef M_LOG10E
#define M_LOG10E 0.434294481903251827651
#endif

#ifndef M_LN2
#define M_LN2 0.693147180559945309417
#endif

#ifndef M_LN10
#define M_LN10 2.30258509299404568402
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4 0.785398163397448309616
#endif

#ifndef M_1_PI
#define M_1_PI 0.318309886183790671538
#endif

#ifndef M_2_PI
#define M_2_PI 0.636619772367581343076
#endif

#ifndef M_1_SQRTPI
#define M_1_SQRTPI 0.564189583547756286948
#endif

#ifndef M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551257390
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

#ifndef M_SQRT_2
#define M_SQRT_2 0.707106781186547524401
#endif

#ifndef ANG2RAD
// 2.0 * M_PI / 360.0
#define ANG2RAD 0.01745329251994329576922
#endif

#include <cassert>
#ifndef assert_not_null
#define assert_not_null(param) \
	assert(param != NULL && "Parameter "## #param ##" cannot be NULL")
#endif

#ifndef assert_not_implemented
#define assert_not_implemented() assert(false && "Not implemented")
#endif

#ifndef OFFSET
#define OFFSET(x) ((char *)NULL + x)
#endif

namespace playstate
{
	template<typename T>
	class auto_array
	{
	private:
		T* mMemory;
		uint32 mLength;

	public:
		auto_array(uint32 length) : mLength(length), mMemory(NULL) {
			if(length > 0) {
				mMemory = new T[length];
			}
		}

		auto_array(T* memory, uint32 length) 
			: mMemory(memory), mLength(length)
		{
		}

		auto_array(auto_array<T>& other)
			: mMemory(other.mMemory), mLength(other.mLength)
		{
			other.Reset();
		}

		~auto_array() { 
			if(mMemory != NULL) {
				delete[] mMemory; 
				mMemory = NULL;
				mLength = 0;
			}
		}

		T* Get() { 
			return mMemory;
		}

		void Reset() { 
			mMemory = NULL;
			mLength = 0;
		}

		uint32 Length() const {
			return mLength;
		}

		T operator [] (uint32 index) { 
			assert(index < mLength && "You are trying to index a to large element in this array");
			return mMemory[index];
		}

		T* operator * () { 
			return mMemory; 
		}

		auto_array<T>& operator = (auto_array<T>& other) {
			if(mMemory != NULL)
				delete[] mMemory;

			mMemory = other.mMemory;
			mLength = other.mLength;
			other.Reset();
			return *this;
		}
	};
}
