#include "CoreEngine.h"

CoreEngine::CoreEngine()
{
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &graphics_device, nullptr, &device_context);

	// set the primitive topology
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	char buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, 100);
	std::filesystem::path path = buffer;
	auto program_dir = path.parent_path();

	auto vertex_shader_path = program_dir / "VertexShader.cso";
	auto pixel_shader_path = program_dir / "PixelShader.cso";

	ObjectManager<ID3DBlob> shader_buffer;
	D3DReadFileToBlob(vertex_shader_path.c_str(), &shader_buffer);
	graphics_device->CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &vertex_shader);
	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);

	D3D11_INPUT_ELEMENT_DESC ied[] = 
	{
		{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,sizeof(float) * 2,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	graphics_device->CreateInputLayout(ied, 2, shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), &input_layout);
	device_context->IASetInputLayout(input_layout.Get());

	D3DReadFileToBlob(pixel_shader_path.c_str(), &shader_buffer);
	graphics_device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &pixel_shader);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(DirectX::XMMATRIX);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	graphics_device->CreateBuffer(&bd, nullptr, &vertex_shader_transform_buffer);
	

	// minimum constant buffer size is 16 bytes (multiple of 16)
	bd.ByteWidth = sizeof(float) * 2 < 16 ? 16 : sizeof(float) * 2;
	graphics_device->CreateBuffer(&bd, nullptr, &vertex_shader_surface_size_buffer);
	
	ID3D11Buffer* const shader_buffers[] = { vertex_shader_transform_buffer.Get() , vertex_shader_surface_size_buffer.Get() };
	device_context->VSSetConstantBuffers(0u, std::size(shader_buffers), shader_buffers);

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							// uses the alpha channel of the source pixel as the blend factor,
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;					// uses the inverse of the alpha channel of the source pixel as the blend factor
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								// adds the source and destination blend factors
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;							// it is fully opaque
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		// enables all the color channels for writing

	ObjectManager<ID3D11BlendState> blendState;
	graphics_device->CreateBlendState(&blendDesc, &blendState);
	device_context->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);

	constexpr unsigned int Indices[] =
	{
		2 , 0 ,1,
		2 , 1 ,3,
	};

	D3D11_BUFFER_DESC ibd = { 0 };
	ibd.ByteWidth = sizeof(unsigned int) * 6;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(unsigned int);

	D3D11_SUBRESOURCE_DATA isubd = { 0 };
	isubd.pSysMem = Indices;

	// create index buffer
	graphics_device->CreateBuffer(&ibd, &isubd, &index_buffer);
	device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

}

void CoreEngine::SetComponent(const DirectX::XMMATRIX transformation)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	device_context->Map(vertex_shader_transform_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
	std::memcpy(ms.pData, &transformation, sizeof(DirectX::XMMATRIX));
	device_context->Unmap(vertex_shader_transform_buffer.Get(), 0u);
}

void CoreEngine::SetComponent(ID3D11Buffer* vertices)
{
	constexpr unsigned int offset = 0u;
	constexpr unsigned int stride = sizeof(VertexType);
	device_context->IASetVertexBuffers(0u, 1u, &vertices, &stride , &offset);
}

ImageSprite CoreEngine::CreateSprite(const Image& image)
{
	const float x_ = image.GetWidth() * 0.5;
	const float y_ = -(image.GetHeight() * 0.5);

	VertexType Vertices[] =
	{
		{ -x_ , y_ , 0.0f , 0.0f},
		{ x_ , y_  , 1.0f , 0.0f},
		{-x_ , -y_  , 0.0f , 1.0f},
		{ x_ , -y_  , 1.0f , 1.0f},
	};

	ImageSprite sprite;

	sprite.width = image.GetWidth();
	sprite.height = image.GetHeight();

	// create vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(VertexType) * 4;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(VertexType);

	D3D11_SUBRESOURCE_DATA subd = { 0 };
	subd.pSysMem = Vertices;

	graphics_device->CreateBuffer(&bd, &subd, &sprite.vertex_buffer);

	// create texture data
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = image.GetWidth();
	desc.Height = image.GetHeight();
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA subresource_data = {};

	auto data = image.Raw();
	subresource_data.pSysMem = data;
	subresource_data.SysMemPitch = sizeof(*data) * image.GetWidth();

	subresource_data.SysMemSlicePitch = 0;

	graphics_device->CreateTexture2D(&desc, &subresource_data, &sprite.TEXTURE);
	graphics_device->CreateShaderResourceView(sprite.TEXTURE.Get(), nullptr, &sprite.TEXTURE_VIEW);

	return sprite;
}

AnimatedSprite CoreEngine::CreateSprite(const std::vector<Image>& frames, std::chrono::milliseconds duration, std::optional<unsigned int> repeat_count)
{
	std::vector<ImageSprite> sprites;
	sprites.reserve(frames.size());

	for (const auto& frame : frames)
	{
		sprites.push_back(CreateSprite(frame));
	}

	return AnimatedSprite(sprites, duration , repeat_count);
}

MemoryRenderer CoreEngine::CreateRenderer(Image& image)
{
	return MemoryRenderer(graphics_device.Get(), image);
}

WindowRenderer CoreEngine::CreateRenderer(CustomWindow& window)
{
	return WindowRenderer(graphics_device.Get(), window.window_handle , window.GetWidth() , window.GetHeight());
}

void CoreEngine::SetComponent(ID3D11ShaderResourceView* texture_view)
{
	device_context->PSSetShaderResources(0u, 1u, &texture_view);
}

void CoreEngine::Draw()
{
	device_context->DrawIndexed(6, 0u, 0u);
}