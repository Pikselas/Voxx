#pragma once
#include<d3d11.h>
#include<DirectXMath.h>

#pragma comment(lib,"d3d11.lib")

class Engine
{
public:
	virtual void SetComponent(const DirectX::XMMATRIX transformation) = 0;
	virtual void SetComponent(ID3D11ShaderResourceView* texture_view) = 0;
	virtual void SetComponent(ID3D11Buffer* vertices) = 0;
public:
	virtual void Draw() = 0;
};