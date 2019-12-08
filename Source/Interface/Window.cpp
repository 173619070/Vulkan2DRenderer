
#include "../Header/Core/SourceCommon.h"

#include "../../Include/Interface/Window.h"
#include "../Header/Impl/WindowImpl.h"
#include "../../Include/Interface/Renderer.h"
#include "../Header/Impl/RendererImpl.h"
#include "../Header/Core/MeshBuffer.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <array>



namespace vk2d {



VK2D_API					Window::Window(
	_internal::RendererImpl		*	renderer_parent,
	WindowCreateInfo			&	window_create_info
)
{
	impl	= std::make_unique<vk2d::_internal::WindowImpl>(
		renderer_parent,
		window_create_info
		);
	if( !impl ) {
		if( renderer_parent->GetReportFunction() ) {
			renderer_parent->GetReportFunction()( ReportSeverity::CRITICAL_ERROR, "Out of host ram!" );
		}
		return;
	}
	if( !impl->is_good ) {
		impl = nullptr;
		return;
	}
	is_good							= true;
}



VK2D_API Window::~Window()
{
	impl	= nullptr;
}



VK2D_API bool VK2D_APIENTRY Window::BeginRender()
{
	if( impl ) {
		return impl->BeginRender();
	}
	return {};
}



VK2D_API bool VK2D_APIENTRY Window::EndRender()
{
	if( impl ) {
		return impl->EndRender();
	}
	return {};
}



VK2D_API void VK2D_APIENTRY Window::DrawTriangleList(
	const std::vector<Vertex>				&	vertices,
	const std::vector<VertexIndex_3>		&	indices,
	bool										filled,
	TextureResource							*	texture
)
{
	if( impl ) {
		impl->DrawTriangleList(
			vertices,
			indices,
			filled,
			texture
		);
	}
}

VK2D_API void VK2D_APIENTRY Window::DrawLineList(
	const std::vector<Vertex>				&	vertices,
	const std::vector<VertexIndex_2>		&	indices,
	TextureResource							*	texture
)
{
	if( impl ) {
		impl->DrawLineList(
			vertices,
			indices,
			texture
		);
	}
}

VK2D_API void VK2D_APIENTRY Window::DrawPointList(
	const std::vector<Vertex>				&	vertices,
	TextureResource							*	texture
)
{
	if( impl ) {
		impl->DrawPointList(
			vertices,
			texture
		);
	}
}

VK2D_API void VK2D_APIENTRY Window::DrawLine(
	Vector2d						point_1,
	Vector2d						point_2,
	Color							color
)
{
	if( impl ) {
		impl->DrawLine(
			point_1,
			point_2,
			color
		);
	}
}

VK2D_API void VK2D_APIENTRY Window::DrawBox(
	Vector2d						top_left,
	Vector2d						bottom_right,
	bool							filled,
	Color							color
)
{
	if( impl ) {
		impl->DrawBox(
			top_left,
			bottom_right,
			filled,
			color
		);
	}
}

VK2D_API void VK2D_APIENTRY Window::DrawCircle(
	Vector2d						top_left,
	Vector2d						bottom_right,
	bool							filled,
	float							edge_count,
	Color							color
)
{
	if( impl ) {
		impl->DrawCircle(
			top_left,
			bottom_right,
			filled,
			edge_count,
			color
		);
	}
}

VK2D_API void VK2D_APIENTRY Window::DrawPie(
	Vector2d						top_left,
	Vector2d						bottom_right,
	float							begin_angle_radians,
	float							end_angle_radians,
	bool							filled,
	float							edge_count,
	Color							color
)
{
	if( impl ) {
		impl->DrawPie(
			top_left,
			bottom_right,
			begin_angle_radians,
			end_angle_radians,
			filled,
			edge_count,
			color
		);
	}
}

VK2D_API void VK2D_APIENTRY Window::DrawPieBox(
	Vector2d						top_left,
	Vector2d						bottom_right,
	float							begin_angle_radians,
	float							coverage,
	bool							filled,
	Color							color
)
{
	if( impl ) {
		impl->DrawPieBox(
			top_left,
			bottom_right,
			begin_angle_radians,
			coverage,
			filled,
			color
		);
	}
}

VK2D_API void VK2D_APIENTRY Window::DrawTexture(
	Vector2d					top_left,
	Vector2d					bottom_right,
	vk2d::TextureResource	*	texture,
	Color						color
)
{
	if( impl ) {
		impl->DrawTexture(
			top_left,
			bottom_right,
			texture,
			color
		);
	}
}

VK2D_API void VK2D_APIENTRY Window::DrawMesh(
	const vk2d::Mesh	&	mesh
)
{
	if( impl ) {
		impl->DrawMesh(
			mesh
		);
	}
}







Cursor::Cursor(
	const std::filesystem::path		&	image_path,
	int32_t								hot_spot_x,
	int32_t								hot_spot_y
)
{
	impl		= std::make_unique<vk2d::_internal::CursorImpl>(
		image_path,
		hot_spot_x,
		hot_spot_y
	);
	if( impl && impl->IsGood() ) {
		is_good			= true;
	} else {
		is_good			= false;
		impl			= nullptr;
	}
}

Cursor::Cursor(
	uint32_t							image_size_x,
	uint32_t							image_size_y,
	const std::vector<vk2d::Color>	&	image_data,
	int32_t								hot_spot_x,
	int32_t								hot_spot_y
)
{
	impl		= std::make_unique<vk2d::_internal::CursorImpl>(
		image_size_x,
		image_size_y,
		image_data,
		hot_spot_x,
		hot_spot_y
	);
	if( impl && impl->IsGood() ) {
		is_good			= true;
	} else {
		is_good			= false;
		impl			= nullptr;
	}
}

Cursor::Cursor(
	vk2d::Cursor	&	other
)
{
	impl		= std::make_unique<vk2d::_internal::CursorImpl>(
		other.impl->GetExtent()[ 0 ],
		other.impl->GetExtent()[ 1 ],
		other.impl->GetPixelData(),
		other.impl->GetHotSpot()[ 0 ],
		other.impl->GetHotSpot()[ 1 ]
	);
	if( impl && impl->IsGood() ) {
		is_good			= true;
	} else {
		is_good			= false;
		impl			= nullptr;
	}
}

Cursor::~Cursor()
{}

VK2D_API vk2d::Cursor &VK2D_APIENTRY Cursor::operator=(
	vk2d::Cursor	&	other )
{
	impl		= std::make_unique<vk2d::_internal::CursorImpl>(
		other.impl->GetExtent()[ 0 ],
		other.impl->GetExtent()[ 1 ],
		other.impl->GetPixelData(),
		other.impl->GetHotSpot()[ 0 ],
		other.impl->GetHotSpot()[ 1 ]
		);
	if( impl && impl->IsGood() ) {
		is_good			= true;
	} else {
		is_good			= false;
		impl			= nullptr;
	}
}

VK2D_API std::array<uint32_t, 2> VK2D_APIENTRY Cursor::GetExtent()
{
	if( is_good ) {
		return impl->GetExtent();
	}
	return {};
}

VK2D_API std::array<int32_t, 2> VK2D_APIENTRY Cursor::GetHotSpot()
{
	if( is_good ) {
		return impl->GetHotSpot();
	}
	return {};
}

VK2D_API const std::vector<vk2d::Color> & VK2D_APIENTRY Cursor::GetPixelData()
{
	if( is_good ) {
		return impl->GetPixelData();
	}
	return {};
}







} // vk2d
