#pragma once
#include"RenderCommandEngine.h"

class Sprite
{
private:
	DirectX::XMVECTOR position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f , 1.0f);
	DirectX::XMMATRIX transformation = DirectX::XMMatrixIdentity();
public:
	virtual ~Sprite() = default;
public:
	virtual void Draw(RenderCommandEngine& engine) const = 0;
protected:
	virtual void DrawNonTransformed(RenderCommandEngine& engine) const = 0;
public:
	void SetPosition(const DirectX::XMVECTOR position)
	{
		this->position = position;
	}
	void SetTransformation(const DirectX::XMMATRIX transform)
	{
		transformation = transform;
	}
	DirectX::XMMATRIX GetTransformation() const
	{
		return transformation * DirectX::XMMatrixTranslationFromVector(position);
	}
	DirectX::XMVECTOR GetPosition() const
	{
		return position;
	}
};