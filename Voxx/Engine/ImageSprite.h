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
public:
	void Draw(Engine& engine) const override
	{
		engine.SetComponent(GetTransformation());
		DrawNonTransformed(engine);
	}

	void DrawNonTransformed(Engine& engine) const override
	{
		engine.SetComponent(index_buffer.Get());
		engine.SetComponent(TEXTURE_VIEW.Get());
		engine.SetComponent(vertex_buffer.Get());

		engine.Draw();
	}
};