#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#include<DirectXColors.h>
#include"RenderDevice.h"
#include<memory>

#pragma comment(lib,"d3d11.lib")

class RenderCommandEngine
{
	template<typename ObjectT>
	using ObjectManager = Microsoft::WRL::ComPtr<ObjectT>;
protected:
	ObjectManager<ID3D11DeviceContext> device_context;
protected:
	ObjectManager<ID3D11Buffer> vertex_shader_surface_size_buffer;
public:
	virtual void SetComponent(const DirectX::XMMATRIX transformation) = 0;
	virtual void SetComponent(ID3D11ShaderResourceView* texture_view) = 0;
	virtual void SetComponent(ID3D11Buffer* vertices) = 0;
public:
	virtual void Draw() = 0;
public:
	void ClearFrame(const RenderDevice& render_device)
	{
		device_context->ClearRenderTargetView(render_device.GetTarget(),DirectX::Colors::White);
	}
	void SetRenderDevice(const RenderDevice& render_device)
	{
		// create and set the viewport
		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = render_device.width;
		viewport.Height = render_device.height;
		viewport.MaxDepth = 1;	// maximum depth for z axis
		viewport.MinDepth = 0;	// minimum depth for z axis

		device_context->RSSetViewports(1u, &viewport);

		const float surface_size[] = { static_cast<float>(render_device.width) / 2 , static_cast<float>(render_device.height) / 2 };

		// update the surface size in vertex shader
		D3D11_MAPPED_SUBRESOURCE ms;
		device_context->Map(vertex_shader_surface_size_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
		std::memcpy(ms.pData, surface_size, sizeof(float) * std::size(surface_size));
		device_context->Unmap(vertex_shader_surface_size_buffer.Get(), 0u);

		auto render_target_view = render_device.GetTarget();
		device_context->OMSetRenderTargets(1u, &render_target_view, nullptr);
	}
};