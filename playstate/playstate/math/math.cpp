#include "../memory/memory.h"
#include "math.h"
using namespace playstate;

float32 Math::Lerp(float32 v0, float32 v1, float32 t)
{
	return v0 + (v1 - v0) * t;
}
