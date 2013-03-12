#ifndef XKILL_IO_LOADER_MD5_JOINT_INFO_H
#define XKILL_IO_LOADER_MD5_JOINT_INFO_H

#include <string>

struct LoaderMD5JointInfo
{
	std::string name_;
	int parentID_;
	int flags_;
	int startIndex_;

	LoaderMD5JointInfo()
	{
		name_		= "Unknown";
		parentID_	= -1;
		flags_		= 0;
		startIndex_ = 0;
	}
	LoaderMD5JointInfo(std::string name, int parentID, int flags, int startIndex)
	{
		name_		= name;
		parentID_	= parentID;
		flags_		= flags;
		startIndex_ = startIndex;
	}
};

#endif //XKILL_IO_LOADER_MD_JOINT_INFO_H