
#include "../Header/Core/SourceCommon.h"

#include "../../Include/Interface/RenderTargetTexture.h"
#include "../Header/Impl/RenderTargetTextureImpl.h"



VK2D_API vk2d::RenderTargetTexture::RenderTargetTexture(
	vk2d::_internal::InstanceImpl						*	instance,
	const vk2d::RenderTargetTextureCreateInfo			&	create_info
)
{
	impl = std::make_unique<vk2d::_internal::RenderTargetTextureImpl>(
		instance,
		create_info
	);

	if( impl && impl->IsGood() ) {
		is_good	= true;
	} else {
		is_good	= false;
		impl	= nullptr;
	}
}


VK2D_API vk2d::RenderTargetTexture::~RenderTargetTexture()
{}

VK2D_API void VK2D_APIENTRY vk2d::RenderTargetTexture::SetSize(
	vk2d::Vector2u		new_size
)
{
	impl->SetSize(
		new_size
	);
}

VK2D_API vk2d::Vector2u VK2D_APIENTRY vk2d::RenderTargetTexture::GetSize()
{
	return impl->GetSize();
}

VK2D_API bool VK2D_APIENTRY vk2d::RenderTargetTexture::BeginRender()
{
	return impl->BeginRender();
}

VK2D_API bool VK2D_APIENTRY vk2d::RenderTargetTexture::EndRender()
{
	return impl->EndRender();
}

/*
VK2D_API void VK2D_APIENTRY vk2d::RenderTargetTexture::DrawTriangleList(
	const std::vector<vk2d::VertexIndex_3>	&	indices,
	const std::vector<vk2d::Vertex>			&	vertices,
	const std::vector<float>				&	texture_channels,
	bool										filled,
	vk2d::TextureResource					*	texture,
	vk2d::Sampler							*	sampler
)
{
	impl->DrawTriangleList(
		indices,
		vertices,
		texture_channels,
		filled,
		texture,
		sampler
	);
}

VK2D_API void VK2D_APIENTRY vk2d::RenderTargetTexture::DrawLineList(
	const std::vector<vk2d::VertexIndex_2>	&	indices,
	const std::vector<vk2d::Vertex>			&	vertices,
	const std::vector<float>				&	texture_channels,
	vk2d::TextureResource					*	texture,
	vk2d::Sampler							*	sampler
)
{
	impl->DrawLineList(
		indices,
		vertices,
		texture_channels,
		texture,
		sampler
	);
}

VK2D_API void VK2D_APIENTRY vk2d::RenderTargetTexture::DrawPointList(
	const std::vector<vk2d::Vertex>		&	vertices,
	const std::vector<float>			&	texture_channels,
	vk2d::TextureResource				*	texture,
	vk2d::Sampler						*	sampler
)
{
	impl->DrawPointList(
		vertices,
		texture_channels,
		texture,
		sampler
	);
}

VK2D_API void VK2D_APIENTRY vk2d::RenderTargetTexture::DrawTexture(
	vk2d::Vector2f				top_left,
	vk2d::Vector2f				bottom_right,
	vk2d::TextureResource	*	texture,
	vk2d::Colorf				color
)
{
	impl->DrawTexture(
		top_left,
		bottom_right,
		texture,
		color
	);
}

VK2D_API void VK2D_APIENTRY vk2d::RenderTargetTexture::DrawMesh(
	const vk2d::Mesh		&	mesh
)
{
	impl->DrawMesh(
		mesh
	);
}
*/

VK2D_API bool VK2D_APIENTRY vk2d::RenderTargetTexture::IsGood()
{
	return is_good;
}
