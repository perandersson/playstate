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

			float32 Colors[4];
		};

		Color();
		Color(float32 c);
		Color(float32 r, float32 g, float32 b);
		Color(float32 r, float32 g, float32 b, float32 a);
		Color(const Color& c);

		void Set(const Color& c);
		bool operator == (const Color& c) const;
		bool operator != (const Color& c) const;

		void operator = (const Color &c);

		static const Color WHITE;
		static const Color BLACK;
		static const Color NOTHING;
	};
}
