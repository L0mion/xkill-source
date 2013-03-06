#include "LoaderMD5Helper.h"

int MD5Helper::getFileLength(const std::string& filename)
{
	long begin, end;
	std::ifstream file(filename);
	begin = file.tellg();
	file.seekg(0, std::ios::end);
	end = file.tellg();
	file.close();

	int fileLength = end-begin;
	return fileLength;
}
void MD5Helper::ignoreLine(std::ifstream& file, int length)
{
	file.ignore(length, '\n');
}
void MD5Helper::removeQuotes(std::string& str)
{
	size_t n;
	while( (n = str.find('\"')) != std::string::npos)
		str.erase(n, 1);
}
void MD5Helper::computeQuaternionW(DirectX::XMFLOAT4& quaternion)
{
	float t = 1.0f - ( quaternion.x * quaternion.x ) - ( quaternion.y * quaternion.y ) - ( quaternion.z * quaternion.z );
    if ( t < 0.0f )
    {
        quaternion.w = 0.0f;
    }
    else
    {
        quaternion.w = -sqrtf(t);
    }
}

DirectX::XMFLOAT3 MD5Helper::normalizeVector(DirectX::XMFLOAT3 vec)
{
	DirectX::XMVECTOR xmVec = DirectX::XMLoadFloat3(&vec);
	xmVec = DirectX::XMVector3Normalize(xmVec);
	
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, xmVec);

	return result;
}
DirectX::XMFLOAT3 MD5Helper::rotateVector(DirectX::XMFLOAT3 vec, DirectX::XMFLOAT4 quaternion)
{
	DirectX::XMVECTOR xmQuaternion = DirectX::XMLoadFloat4(&quaternion);
	DirectX::XMVECTOR xmVec = DirectX::XMLoadFloat3(&vec);
	xmVec = DirectX::XMVector3Rotate(xmVec, xmQuaternion);
	
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, xmVec);

	return result;
}
DirectX::XMFLOAT3 MD5Helper::addVectors(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2)
{
	DirectX::XMVECTOR xmVec1	= DirectX::XMLoadFloat3(&vec1);
	DirectX::XMVECTOR xmVec2	= DirectX::XMLoadFloat3(&vec2);
	DirectX::XMVECTOR xmResult  = DirectX::XMVectorAdd(xmVec1, xmVec2);

	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, xmResult);

	return result;
}
DirectX::XMFLOAT4 MD5Helper::multiplyQuaternions(DirectX::XMFLOAT4 q1, DirectX::XMFLOAT4 q2)
{
	DirectX::XMVECTOR xmQ1		= DirectX::XMLoadFloat4(&q1);
	DirectX::XMVECTOR xmQ2		= DirectX::XMLoadFloat4(&q2);
	DirectX::XMVECTOR xmResult	= DirectX::XMQuaternionMultiply(xmQ1, xmQ2);

	DirectX::XMFLOAT4 result;
	DirectX::XMStoreFloat4(&result, xmResult);

	return result;
}
DirectX::XMFLOAT4 MD5Helper::normalizeQuaternion(DirectX::XMFLOAT4 quaternion)
{
	DirectX::XMVECTOR xmQuaternion = DirectX::XMLoadFloat4(&quaternion);
	DirectX::XMVECTOR xmResult = DirectX::XMQuaternionNormalize(xmQuaternion);

	DirectX::XMFLOAT4 result;
	DirectX::XMStoreFloat4(&result, xmResult);

	return result;
}