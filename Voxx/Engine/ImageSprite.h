#pragma 
#include<wrl.h>
#include"Sprite.h"

class ImageSprite : public Sprite
{
	friend class CoreEngine;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			 TEXTURE;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TEXTURE_VIEW;
private:
	unsigned int width;
	unsigned int height;
public:
	void Draw(RenderCommandEngine& engine) const override
	{
		engine.SetComponent(GetTransformation());
		DrawNonTransformed(engine);
	}

	void DrawNonTransformed(RenderCommandEngine& engine) const override
	{
		engine.SetComponent(index_buffer.Get());
		engine.SetComponent(TEXTURE_VIEW.Get());
		engine.SetComponent(vertex_buffer.Get());

		engine.Draw();
	}

public:
	unsigned int GetWidth() const
	{
		return width;
	}
	unsigned int GetHeight() const
	{
		return height;
	}
};