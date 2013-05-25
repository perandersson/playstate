#include "memory/memory.h"
#include "version.h"
using namespace playstate;

Version::Version()
	: mMajor(0), mMinor(0)
{}

Version::Version(int major, int minor)
	: mMajor(major), mMinor(minor)
{
}

Version::Version(const Version& version)
	: mMajor(version.mMajor), mMinor(version.mMinor)
{
}

Version::~Version()
{
}

bool Version::HigherOrEqual(const Version& version) const
{
	double versionVal = (double)version.mMajor;
	versionVal += (double)version.mMinor * 0.1;

	double val = (double)mMajor;
	val += (double)mMinor * 0.1;

	return val >= versionVal;
}

void Version::operator=(const Version& other)
{
	mMinor = other.mMinor;
	mMajor = other.mMajor;
}
