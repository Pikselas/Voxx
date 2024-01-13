#pragma once
#include<d3d11.h>
#include<wrl.h>

class RenderDevice
{
	friend class RenderCommandEngine;
private:
	const unsigned int width;
	const unsigned int height;
private:
	ID3D11Resource* render_surface = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
protected:
	RenderDevice(unsigned int width, unsigned int height)
		:
		width(width),
		height(height)
	{}
private:
	ID3D11RenderTargetView* GetTarget() const
	{
		return render_target_view.Get();
	}
protected:
	void CreateTarget(ID3D11Device* device, ID3D11Resource* surface)
	{
		render_surface = surface;
		device->CreateRenderTargetView(surface, nullptr, &render_target_view);
	}
public:
	virtual void RenderFrame() = 0;
public:
	void CopyFrame(const RenderDevice& renderer)
	{
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		render_surface->GetDevice(&device);
		device->GetImmediateContext(&context);

		context->CopyResource(render_surface, renderer.render_surface);
	}
};