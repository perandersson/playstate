#pragma once

#include "../types.h"

namespace playstate
{
	struct Color
	{
		union
		{
			struct
			{
				float32 Red;
				float32 Green;
				float32 Blue;
				float32 Alpha;
			};

			struct
			{
				float32 R;
				float32 G;
				float32 B;
				float32 A;
			};

			float32 Colors[4];
		};

		Color();
		Color(float32 c);
		Color(float32 r, float32 g, float32 b);
		Color(float32 r, float32 g, float32 b, float32 a);
		Color(const Color& c);

		void Set(const Color& c);

		inline bool operator == (const Color& c) const {
			return abs(c.Red - Red) <= FLT_EPSILON && abs(c.Green - Green) <= FLT_EPSILON &&
				abs(c.Blue - Blue) <= FLT_EPSILON && abs(c.Alpha - Alpha) <= FLT_EPSILON;
		}
		
		inline bool operator != (const Color& c) const {
			return abs(c.Red - Red) > FLT_EPSILON || abs(c.Green - Green) > FLT_EPSILON ||
				abs(c.Blue - Blue) > FLT_EPSILON || abs(c.Alpha - Alpha) > FLT_EPSILON;
		}

		void operator = (const Color &c);

		static const Color White;
		static const Color Black;
		static const Color Nothing;

		//
		// Converts a supplied hex value into a color usable by the game engine
		//
		// @param hex The hex decimal html color value. It can take the following combinations: "FF", "#FF", "AABBCC", "#AABBCC"
		// @return 
		static Color HexToRGB(const char* hex);
	};
}
