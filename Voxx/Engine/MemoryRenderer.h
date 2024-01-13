#pragma once
#include"RenderDevice.h"
#include"Image.h"

class MemoryRenderer : public RenderDevice
{
	friend class CoreEngine;
private:
	Image& surface;
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> memory_data;
private:
	MemoryRenderer(ID3D11Device* device, Image& surface)
		:
		surface(surface),
		RenderDevice(surface.GetWidth() , surface.GetHeight())
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = surface.GetWidth();
		desc.Height = surface.GetHeight();
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		device->CreateTexture2D(&desc, nullptr, &memory_data);
		CreateTarget(device, memory_data.Get());
	}
public:
	void RenderFrame() override
	{
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context;
		
		memory_data->GetDevice(&device);
		device->GetImmediateContext(&device_context);

		// create texture for reading
		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = surface.GetWidth();
		desc.Height = surface.GetHeight();
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> read_texture;
		device->CreateTexture2D(&desc, nullptr, &read_texture);
		device_context->CopyResource(read_texture.Get(), memory_data.Get());

		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		device_context->Map(read_texture.Get(), 0, D3D11_MAP_READ, 0, & mapped_resource);
		std::memcpy(surface.Raw(), mapped_resource.pData ,sizeof(ColorType) * surface.GetWidth() * surface.GetHeight());
		device_context->Unmap(read_texture.Get(), 0);
	}
};