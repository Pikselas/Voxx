#pragma once

#include<D3DCompiler.h>
#pragma comment(lib,"D3DCompiler.lib")

#include"Image.h"
#include"ImageSprite.h"
#include"CustomWindow.h"

#include "Engine.h"

class CoreEngine : public Engine
{
private:
	template<typename ObjectT>
	using ObjectManager = Microsoft::WRL::ComPtr<ObjectT>;
private:
	struct VertexType
	{
		float x, y;
		float u, v;
	};
private:
	struct VertexShaderBufferT
	{
		float half_window_width;
		float half_window_height;
		DirectX::XMMATRIX transformation;
	};
private:
	ObjectManager<ID3D11Device>				graphics_device;
	ObjectManager<ID3D11DeviceContext>		device_context;
	ObjectManager<IDXGISwapChain>			swap_chain;
	ObjectManager<ID3D11RenderTargetView>	render_target_view;
private:
	ObjectManager<ID3D11InputLayout>		input_layout;
private:
	ObjectManager<ID3D11VertexShader>		vertex_shader;
	ObjectManager<ID3D11PixelShader>		pixel_shader;
private:
	ObjectManager<ID3D11Buffer>				vertex_shader_buffer;
	ObjectManager<ID3D11Buffer>				index_buffer;
private:
	const float half_window_width;
	const float half_window_height;
public:
	CoreEngine(CustomWindow& window);
private:
	ObjectManager<ID3D11SamplerState>		SAMPLER_STATE;
public:
	void SetComponent(const DirectX::XMMATRIX transformation) override;
	void SetComponent(ID3D11ShaderResourceView* texture_view) override;
	void SetComponent(ID3D11Buffer* vertices) override;
public:
	ImageSprite CreateSprite(const Image& image);
public:
	void Draw() override;
public:
	void ClearFrame();
	void RenderFrame();
};