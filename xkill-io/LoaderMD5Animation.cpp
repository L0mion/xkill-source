#include "LoaderMD5Animation.h"
#include "LoaderMD5Helper.h"

LoaderMD5Animation::LoaderMD5Animation()
{
	md5Version_				= 0;
	numFrames_				= 0;
	numJoints_				= 0;
	frameRate_				= 0;
	numAnimatedComponents_	= 0;

	animationDuration_	= 0.0f;
	frameDuration_		= 0.0f;
	animationTime_		= 0.0f;
}
LoaderMD5Animation::~LoaderMD5Animation()
{
	if(infile_.is_open())
		infile_.close();
}

void LoaderMD5Animation::loadAnimation(const std::string& filename, LoaderMD5AnimationDesc* animationDesc)
{
	reset();

	fileLength_ = MD5Helper::getFileLength(filename);

	std::string param;
	infile_.open(filename.c_str());
	infile_ >> param;

	while(!infile_.eof())
	{
		if(param == "MD5Version") 
			parseParamMD5Version();
		else if(param == "commandline")
			parseParamCommandline();
		else if(param == "numFrames")
			parseParamNumFrames();
		else if(param == "numJoints")
			parseParamNumJoints();
		else if(param == "frameRate")
			parseParamFrameRate();
		else if(param == "numAnimatedComponents")
			parseParamNumAnimatedComponents();
		else if(param == "hierarchy")
			parseParamHierarchy();
		else if(param == "bounds")
			parseParamBounds();
		else if(param == "baseframe")
			parseParamBaseFrame();
		else if(param == "frame")
			parseParamFrame();

		infile_ >> param;
	}

	animatedSkeleton_.joints_.assign(numJoints_, LoaderMD5SkeletonJointDesc());

	frameDuration_		= 1.0f / static_cast<float>(frameRate_);
	animationDuration_	= frameDuration_ * static_cast<float>(numFrames_);
	animationTime_		= 0.0f;

	animationDesc->frameDuration_ = frameDuration_;
	animationDesc->baseFrames_	  = baseFrames_;
	animationDesc->skeletons_	  = skeletons_;
	animationDesc->jointInfos_	  = jointInfos_;

	infile_.close();
}

void LoaderMD5Animation::parseParamMD5Version()
{
	infile_ >> md5Version_;
}
void LoaderMD5Animation::parseParamCommandline()
{
	//Commandline is uninteresting to us.
	MD5Helper::ignoreLine(infile_, fileLength_);
}
void LoaderMD5Animation::parseParamNumFrames()
{
	infile_ >> numFrames_;
	MD5Helper::ignoreLine(infile_, fileLength_);
}
void LoaderMD5Animation::parseParamNumJoints()
{
	infile_ >> numJoints_;
	MD5Helper::ignoreLine(infile_, fileLength_);
}
void LoaderMD5Animation::parseParamFrameRate()
{
	infile_ >> frameRate_;
	MD5Helper::ignoreLine(infile_, fileLength_);
}
void LoaderMD5Animation::parseParamNumAnimatedComponents()
{
	infile_ >> numAnimatedComponents_;
	MD5Helper::ignoreLine(infile_, fileLength_);
}
void LoaderMD5Animation::parseParamHierarchy()
{
	std::string dummy;
	infile_ >> dummy; // Read the '}' character.

	for(int i=0; i<numJoints_; i++)
	{
		LoaderMD5JointInfo joint;
		infile_ >> joint.name_ >> joint.parentID_ >> joint.flags_ >> joint.startIndex_;
		MD5Helper::removeQuotes(joint.name_);

		jointInfos_.push_back(joint);
		MD5Helper::ignoreLine(infile_, fileLength_);
	}

	infile_ >> dummy; //Read the '}' character.
}
void LoaderMD5Animation::parseParamBounds()
{
	std::string dummy;
	infile_ >> dummy; // Read the '{' character.
	MD5Helper::ignoreLine(infile_, fileLength_);

	for(int i=0; i<numFrames_; i++)
	{
		LoaderMD5BoundsDesc bound;
		infile_ >> dummy; // Read the '(' character.
		infile_ >> bound.min_.x >> bound.min_.y >> bound.min_.z;
		infile_ >> dummy >> dummy; //Read the ')' and '(' characters.
		infile_ >> bound.max_.x >> bound.max_.y >> bound.max_.z;

		bounds_.push_back(bound);
		MD5Helper::ignoreLine(infile_, fileLength_);
	}
	infile_ >> dummy; //Read the '}' character.
	MD5Helper::ignoreLine(infile_, fileLength_);
}
void LoaderMD5Animation::parseParamBaseFrame()
{
	std::string dummy;
	infile_ >> dummy; //Read the '{' character
	MD5Helper::ignoreLine(infile_, fileLength_);

	for(int i=0; i<numJoints_; i++)
	{
		LoaderMD5BaseFrameDesc baseFrame;
		infile_ >> dummy;
		infile_ >> baseFrame.position_.x >> baseFrame.position_.y >> baseFrame.position_.z;
		infile_ >> dummy >> dummy;
		infile_ >> baseFrame.orientationQuaternion_.x >> baseFrame.orientationQuaternion_.y >> baseFrame.orientationQuaternion_.z;
		MD5Helper::ignoreLine(infile_, fileLength_);

		baseFrames_.push_back(baseFrame);
	}
	infile_ >> dummy; // Read the '{' character.
	MD5Helper::ignoreLine(infile_, fileLength_);
}
void LoaderMD5Animation::parseParamFrame()
{
	std::string dummy;
	infile_ >> dummy; // Read the '{' character;
	MD5Helper::ignoreLine(infile_, fileLength_);

	LoaderMD5FrameDataDesc frame;
	for(int i=0; i<numAnimatedComponents_; i++)
	{
		float frameData;
		infile_ >> frameData;
		frame.frameData_.push_back(frameData);
	}
	frames_.push_back(frame);

	buildFrameSkeleton(skeletons_, jointInfos_, baseFrames_, frame);

	infile_ >> dummy; // Read the '}' character.
	MD5Helper::ignoreLine(infile_, fileLength_);
}

void LoaderMD5Animation::buildFrameSkeleton(std::vector<LoaderMD5FrameSkeleton>& skeletons, const std::vector<LoaderMD5JointInfo>& jointInfos,
											const std::vector<LoaderMD5BaseFrameDesc>& baseFrames, const LoaderMD5FrameDataDesc& frameData)
{
	LoaderMD5FrameSkeleton skeleton;
	for(unsigned int i=0; i<jointInfos.size(); i++)
	{
		unsigned int j = 0;

		const LoaderMD5JointInfo& jointInfo = jointInfos[i];
		LoaderMD5SkeletonJointDesc animatedJoint(baseFrames[i].position_, baseFrames[i].orientationQuaternion_);
		animatedJoint.parent_ = jointInfo.parentID_;

		if(jointInfo.flags_ & 1)
			animatedJoint.position_.x = frameData.frameData_[jointInfo.startIndex_ + j++];
		if(jointInfo.flags_ & 2)
			animatedJoint.position_.y = frameData.frameData_[jointInfo.startIndex_ + j++];
		if(jointInfo.flags_ & 4)
			animatedJoint.position_.z = frameData.frameData_[jointInfo.startIndex_ + j++];
		if(jointInfo.flags_ & 8)
			animatedJoint.orientationQuaternion_.x = frameData.frameData_[jointInfo.startIndex_ + j++];
		if(jointInfo.flags_ & 16)
			animatedJoint.orientationQuaternion_.y = frameData.frameData_[jointInfo.startIndex_ + j++];
		if(jointInfo.flags_ & 32)
			animatedJoint.orientationQuaternion_.z = frameData.frameData_[jointInfo.startIndex_ + j++];

		MD5Helper::computeQuaternionW(animatedJoint.orientationQuaternion_);

		if(animatedJoint.parent_ >= 0)
		{
			LoaderMD5SkeletonJointDesc& parentJoint = skeleton.joints_[animatedJoint.parent_];
			DirectX::XMFLOAT3 rotatedPosition = MD5Helper::rotateVector(parentJoint.position_, parentJoint.orientationQuaternion_);
			animatedJoint.position_ = MD5Helper::addVectors(parentJoint.position_, rotatedPosition);
			animatedJoint.orientationQuaternion_ = MD5Helper::multiplyQuaternions(parentJoint.orientationQuaternion_, animatedJoint.orientationQuaternion_);
			animatedJoint.orientationQuaternion_ = MD5Helper::normalizeQuaternion(animatedJoint.orientationQuaternion_);
		}

		skeleton.joints_.push_back(animatedJoint);
	}
	skeletons.push_back(skeleton);
}

void LoaderMD5Animation::reset()
{
	if(infile_.is_open())
		infile_.close();
	fileLength_ = 0;

	jointInfos_.clear();
	bounds_.clear();
	baseFrames_.clear();
	frames_.clear();
	skeletons_.clear();

	animatedSkeleton_.joints_.clear();

	md5Version_				= 0;
	numFrames_				= 0;
	numJoints_				= 0;
	frameRate_				= 0;
	numAnimatedComponents_	= 0;

	animationDuration_	= 0.0f;
	frameDuration_		= 0.0f;
	animationTime_		= 0.0f;
}