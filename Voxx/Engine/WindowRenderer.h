#pragma once
#include"RenderDevice.h"
#pragma comment(lib,"dxgi.lib")

class WindowRenderer : public RenderDevice
{
	friend class CoreEngine;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain;
private:
	WindowRenderer(ID3D11Device* device , HWND handle , unsigned int width , unsigned int height) 		:
	RenderDevice(width, height)
	{	
		DXGI_SWAP_CHAIN_DESC sd = { 0 };
		sd.BufferDesc.Width = 0;												// look at the window and use it's size
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.RefreshRate.Numerator = 0;								// pick the default refresh rates
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferCount = 1;														// one back buffer -> one back and one front (double buffering)
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;						// this is the color format (BGRA) 
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// not specifying any scaling because we want the renedred frame same as window size
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// how buffer scaning will be done for copying all to video memory
		sd.Flags = 0;															// not setting any flags
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						// use the buffer for render target
		sd.OutputWindow = handle;												// set the window handle
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;								// discard the effects for swapping frames
		sd.Windowed = TRUE;

		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<IDXGIFactory> dxgi_factory;
		CreateDXGIFactory(__uuidof(IDXGIFactory), &dxgi_factory);
		dxgi_factory->CreateSwapChain(device, &sd, &swap_chain);

		Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer;
		swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer);

		CreateTarget(device, BackBuffer.Get());
	}
public:
	void RenderFrame() override
	{
		swap_chain->Present(1u, 0u);
	}
};