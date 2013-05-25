#pragma once

namespace playstate
{
	class Version
	{
	public:
		Version();
		Version(int major, int minor);
		Version(const Version& version);
		~Version();

		//
		// Compares if this version is higher or equal to the supplied version
		// @param version
		bool HigherOrEqual(const Version& version) const;

		//
		void operator = (const Version& version);


	private:
		int mMajor;
		int mMinor;
	};
}
