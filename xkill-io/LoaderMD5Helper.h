#ifndef XKILL_IO_LOADER_MD5_HELPER_H
#define XKILL_IO_LOADER_MD5_HELPER_H

#include <string>
#include <fstream>
#include <DirectXMath.h>

namespace MD5Helper
{
	int getFileLength(const std::string& filename);
	void ignoreLine(std::ifstream& file, int length);
	void removeQuotes(std::string& str);

	void computeQuaternionW(DirectX::XMFLOAT4& quaternion);
	
	DirectX::XMFLOAT3 normalizeVector(DirectX::XMFLOAT3 vec);
	DirectX::XMFLOAT3 rotateVector(DirectX::XMFLOAT3 vec, DirectX::XMFLOAT4 quaternion);
	DirectX::XMFLOAT3 addVectors(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2);
	DirectX::XMFLOAT4 multiplyQuaternions(DirectX::XMFLOAT4 q1, DirectX::XMFLOAT4 q2);
	DirectX::XMFLOAT4 normalizeQuaternion(DirectX::XMFLOAT4 quaternion);
}

#endif //XKILL_IO_LOADER_MD5_HELPER_H