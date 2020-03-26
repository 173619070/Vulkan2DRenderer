#pragma once

#include "../Core/SourceCommon.h"

#include "../../Include/Interface/RenderTargetTexture.h"

#include "../Core/VulkanMemoryManagement.h"
#include "../Core/DescriptorSet.h"
#include "../Core/MeshBuffer.h"
#include "RenderTargetCommonImpl.hpp"

#include <vector>
#include <memory>
#include <map>


namespace vk2d {

namespace _internal {

class InstanceImpl;
class MeshBuffer;



class RenderTargetTextureImpl
{
public:
	RenderTargetTextureImpl(
		vk2d::_internal::InstanceImpl				*	instance,
		const vk2d::RenderTargetTextureCreateInfo	&	create_info );

	~RenderTargetTextureImpl();

	void												SetSize(
		vk2d::Vector2u									new_size );

	vk2d::Vector2u										GetSize();

	// Begins the render operations. You must call this before using any drawing commands.
	// For best performance you should calculate game logic first, when you're ready to draw
	// call this function just before your first draw command.
	bool												BeginRender();

	// Ends the rendering operations. You must call this after you're done drawing.
	// This will display the results on screen.
	bool												EndRender();

	void												DrawTriangleList(
		const std::vector<vk2d::VertexIndex_3>		&	indices,
		const std::vector<vk2d::Vertex>				&	vertices,
		const std::vector<float>					&	texture_channels,
		bool											filled						= true,
		vk2d::TextureResource						*	texture						= nullptr,
		vk2d::Sampler								*	sampler						= nullptr );

	void												DrawLineList(
		const std::vector<vk2d::VertexIndex_2>		&	indices,
		const std::vector<vk2d::Vertex>				&	vertices,
		const std::vector<float>					&	texture_channels,
		vk2d::TextureResource						*	texture						= nullptr,
		vk2d::Sampler								*	sampler						= nullptr );

	void												DrawPointList(
		const std::vector<vk2d::Vertex>				&	vertices,
		const std::vector<float>					&	texture_channels,
		vk2d::TextureResource						*	texture						= nullptr,
		vk2d::Sampler								*	sampler						= nullptr );

	void												DrawTexture(
		vk2d::Vector2f									top_left,
		vk2d::Vector2f									bottom_right,
		vk2d::TextureResource						*	texture,
		vk2d::Colorf									color						= { 1.0f, 1.0f, 1.0f, 1.0f } );

	void												DrawMesh(
		const vk2d::Mesh							&	mesh );

	bool												IsGood();

private:
	bool												CreateCommandBuffers();
	void												DestroyCommandBuffers();

	bool												CreateFrameDataBuffers();
	void												DestroyFrameDataBuffers();

	bool												CreateSurfaces(
		vk2d::Vector2u									new_size );

	void												DestroySurfaces();

	void												CmdBindPipelineIfDifferent(
		VkCommandBuffer									command_buffer,
		const vk2d::_internal::PipelineSettings		&	pipeline_settings );

	void												CmdBindTextureSamplerIfDifferent(
		VkCommandBuffer									command_buffer,
		vk2d::Sampler								*	sampler,
		vk2d::TextureResource						*	texture );

	void												CmdSetLineWidthIfDifferent(
		VkCommandBuffer									command_buffer,
		float											line_width );

	bool												CmdUpdateFrameData(
		VkCommandBuffer									command_buffer );

	struct Surface
	{
		vk2d::_internal::CompleteImageResource			render_image						= {};
		vk2d::_internal::CompleteImageResource			resolve_image						= {};
		vk2d::_internal::CompleteImageResource			blur_buffer_image					= {};
		VkFramebuffer									vk_framebuffer						= {};
	};

	vk2d::_internal::InstanceImpl					*	instance							= {};
	vk2d::RenderTargetTextureCreateInfo					create_info_copy					= {};

	vk2d::Vector2u										size								= {};
	std::vector<VkExtent2D>								mipmap_levels						= {};

	vk2d::_internal::CompleteBufferResource				frame_data_staging_buffer			= {};
	vk2d::_internal::CompleteBufferResource				frame_data_device_buffer			= {};
	vk2d::_internal::PoolDescriptorSet					frame_data_descriptor_set			= {};

	std::unique_ptr<vk2d::_internal::MeshBuffer>		mesh_buffer;

	VkCommandPool										vk_command_pool						= {};
	std::vector<VkCommandBuffer>						vk_render_command_buffers			= {};
	std::vector<VkCommandBuffer>						vk_transfer_command_buffers			= {};
	VkRenderPass										vk_render_pass						= {};

	uint32_t											current_surface						= {};
	std::vector<vk2d::_internal::RenderTargetTextureImpl::Surface>							surfaces;

	vk2d::_internal::PipelineSettings					previous_pipeline_settings			= {};
	vk2d::TextureResource							*	previous_texture					= {};
	vk2d::Sampler									*	previous_sampler					= {};
	float												previous_line_width					= {};

	std::map<vk2d::Sampler*, std::map<vk2d::TextureResource*, vk2d::_internal::SamplerTextureDescriptorPoolData>>
		sampler_texture_descriptor_sets														= {};

	bool												is_good								= {};
};



} // _internal

} // vk2d
