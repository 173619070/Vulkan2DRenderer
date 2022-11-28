
#include <core/SourceCommon.hpp>

#include "WindowImpl.hpp"
#include <mesh/generators/MeshGenerators.hpp>
#include <mesh/modifiers/MeshModifiers.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API vk2d::Window::Window(
	vk2d_internal::InstanceImpl	&	instance,
	const WindowCreateInfo		&	window_create_info
)
{
	impl = std::make_unique<vk2d_internal::WindowImpl>(
		*this,
		instance,
		window_create_info
	);
	if( !impl || !impl->IsGood() ) {
		instance.Report( ReportSeverity::CRITICAL_ERROR, "Internal error: Cannot create window implementation!" );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API vk2d::Window::~Window()
{
	impl = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::CloseWindow()
{
	impl->CloseWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API bool vk2d::Window::ShouldClose()
{
	return impl->ShouldClose();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::TakeScreenshotToFile(
	const std::filesystem::path		&	save_path,
	bool								include_alpha
)
{
	impl->TakeScreenshotToFile(
		save_path,
		include_alpha
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::TakeScreenshotToData(
	bool		include_alpha
)
{
	impl->TakeScreenshotToData( include_alpha );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::Focus()
{
	impl->Focus();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetOpacity(
	float			opacity
)
{
	impl->SetOpacity( opacity );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API float vk2d::Window::GetOpacity()
{
	return impl->GetOpacity();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::Hide(
	bool			hidden
)
{
	impl->Hide( hidden );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API bool vk2d::Window::IsHidden()
{
	return impl->IsHidden();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DisableEvents(
	bool				disable_events
)
{
	impl->DisableEvents( disable_events );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API bool vk2d::Window::AreEventsDisabled()
{
	return impl->AreEventsDisabled();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetFullscreen(
	Monitor				*	monitor,
	uint32_t				frequency )
{
	impl->SetFullscreen( monitor, frequency );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API bool vk2d::Window::IsFullscreen()
{
	return impl->IsFullscreen();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API glm::dvec2 vk2d::Window::GetCursorPosition()
{
	return impl->GetCursorPosition();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetCursorPosition(
	glm::dvec2			new_position
)
{
	impl->SetCursorPosition( new_position );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetCursor(
	Cursor		*	cursor
)
{
	impl->SetCursor( cursor );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API std::string vk2d::Window::GetClipboardString()
{
	return impl->GetClipboardString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetClipboardString( const std::string & str )
{
	impl->SetClipboardString( str );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetTitle( const std::string & title )
{
	impl->SetTitle( title );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API std::string vk2d::Window::GetTitle()
{
	return impl->GetTitle();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetIcon(
	const std::vector<std::filesystem::path>		&	image_paths )
{
	impl->SetIcon( image_paths );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetPosition(
	glm::ivec2			new_position
)
{
	impl->SetPosition( new_position );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API glm::ivec2 vk2d::Window::GetPosition()
{
	return impl->GetPosition();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetSize(
	glm::uvec2		new_size
)
{
	impl->SetSize( new_size );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API glm::uvec2 vk2d::Window::GetSize()
{
	return impl->GetSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::Iconify(
	bool		iconified
)
{
	impl->Iconify( iconified );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API bool vk2d::Window::IsIconified()
{
	return impl->IsIconified();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetMaximized(
	bool		maximized
)
{
	impl->SetMaximized( maximized );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API bool vk2d::Window::GetMaximized()
{
	return impl->GetMaximized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetCursorState(
	CursorState		new_state
)
{
	impl->SetCursorState( new_state );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API vk2d::CursorState vk2d::Window::GetCursorState()
{
	return impl->GetCursorState();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API bool vk2d::Window::BeginRender()
{
	return impl->BeginRender();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API bool vk2d::Window::EndRender()
{
	return impl->EndRender();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::SetRenderCoordinateSpace(
	RenderCoordinateSpace coordinate_space
)
{
	impl->SetRenderCoordinateSpace( coordinate_space );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DrawPoint(
	glm::vec2		location,
	Colorf			color,
	float			size
)
{
	auto mesh = mesh_generators::GeneratePointMeshFromList(
		{ location }
	);
	mesh_modifiers::SetVerticesColor( mesh, color );
	mesh_modifiers::SetVerticesPointSize( mesh, size );
	DrawMesh( mesh, std::vector { glm::mat4( 1.0f ) } );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DrawLine(
	glm::vec2		point_1,
	glm::vec2		point_2,
	Colorf			color,
	float			line_width
)
{
	auto mesh = mesh_generators::GenerateLineMeshFromList(
		{ point_1, point_2 },
		{ { 0, 1 } }
	);
	mesh_modifiers::SetVerticesColor( mesh, color );
	mesh.SetLineWidth( line_width );
	DrawMesh( mesh, std::vector { glm::mat4( 1.0f ) } );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DrawRectangle(
	Rect2f			area,
	bool			filled,
	Colorf			color
)
{
	auto mesh = mesh_generators::GenerateRectangleMesh(
		area,
		filled
	);
	mesh_modifiers::SetVerticesColor( mesh, color );
	DrawMesh( mesh, std::vector { glm::mat4( 1.0f ) } );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DrawEllipse(
	Rect2f			area,
	bool			filled,
	float			edge_count,
	Colorf			color
)
{
	auto mesh = mesh_generators::GenerateEllipseMesh(
		area,
		filled,
		edge_count
	);
	mesh_modifiers::SetVerticesColor( mesh, color );
	DrawMesh( mesh, std::vector { glm::mat4( 1.0f ) } );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DrawEllipsePie(
	Rect2f			area,
	float			begin_angle_radians,
	float			coverage,
	bool			filled,
	float			edge_count,
	Colorf			color
)
{
	auto mesh = mesh_generators::GenerateEllipsePieMesh(
		area,
		begin_angle_radians,
		coverage,
		filled,
		edge_count
	);
	mesh_modifiers::SetVerticesColor( mesh, color );
	DrawMesh( mesh, std::vector { glm::mat4( 1.0f ) } );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DrawRectanglePie(
	Rect2f			area,
	float			begin_angle_radians,
	float			coverage,
	bool			filled,
	Colorf			color
)
{
	auto mesh = mesh_generators::GenerateRectanglePieMesh(
		area,
		begin_angle_radians,
		coverage,
		filled
	);
	mesh_modifiers::SetVerticesColor( mesh, color );
	DrawMesh( mesh, std::vector { glm::mat4( 1.0f ) } );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DrawTexture(
	glm::vec2		top_left,
	Texture		*	texture,
	Colorf			color
)
{
	if( texture ) {
		auto texture_size	= texture->GetSize();
		auto bottom_right	= top_left + glm::vec2( float( texture_size.x ), float( texture_size.y ) );
		texture->GetSize();
		auto mesh = mesh_generators::GenerateRectangleMesh(
			{ top_left, bottom_right }
		);
		mesh.SetTexture( texture );
		mesh_modifiers::SetVerticesColor( mesh, color );
		DrawMesh( mesh, std::vector { glm::mat4( 1.0f ) } );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API bool vk2d::Window::IsGood() const
{
	if( !impl ) return false;
	return impl->IsGood();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DrawPointList_DEPRICATED(
	const vk2d_internal::RawVertexData	&	raw_vertex_data,
	std::span<const glm::mat4>				transformations,
	Texture								*	texture,
	Sampler_DEPRICATED								*	sampler
)
{
	impl->DrawPointList_DEPRICATED(
		raw_vertex_data,
		transformations,
		texture,
		sampler
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DrawLineList_DEPRICATED(
	std::span<const uint32_t>				indices,
	const vk2d_internal::RawVertexData	&	raw_vertex_data,
	std::span<const glm::mat4>				transformations,
	Texture								*	texture,
	Sampler_DEPRICATED								*	sampler,
	float									line_width
)
{
	impl->DrawLineList_DEPRICATED(
		indices,
		raw_vertex_data,
		transformations,
		texture,
		sampler,
		line_width
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API void vk2d::Window::DrawTriangleList_DEPRICATED(
	std::span<const uint32_t>				indices,
	const vk2d_internal::RawVertexData	&	raw_vertex_data,
	std::span<const glm::mat4>				transformations,
	bool									filled,
	Texture								*	texture,
	Sampler_DEPRICATED								*	sampler
)
{
	impl->DrawTriangleList_DEPRICATED(
		indices,
		raw_vertex_data,
		transformations,
		filled,
		texture,
		sampler
	);
}
