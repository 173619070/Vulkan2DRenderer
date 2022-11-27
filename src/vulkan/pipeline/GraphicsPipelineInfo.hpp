#pragma once

#include <core/SourceCommon.hpp>

#include <vulkan/shaders/ShaderInterface.hpp>

#include <utility/Hasher.hpp>
#include <utility/EnumHelpers.hpp>



namespace vk2d {
namespace vulkan {



class GraphicsPipelineInfo {
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline 									GraphicsPipelineInfo() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline 									GraphicsPipelineInfo(
		VkPipelineLayout					vulkan_pipeline_layout,
		VkRenderPass						vulkan_render_pass,
		VkPrimitiveTopology					primitive_topology,
		VkPolygonMode						polygon_mode,
		GraphicsShaderList					graphics_shader_list,
		VkSampleCountFlags					sample_count,
		VkBool32							enable_blending
	) :
		vulkan_pipeline_layout( vulkan_pipeline_layout ),
		vulkan_render_pass( vulkan_render_pass ),
		primitive_topology( primitive_topology ),
		polygon_mode( polygon_mode ),
		graphics_shader_list( graphics_shader_list ),
		sample_count( sample_count ),
		enable_blending( enable_blending )
	{
		hash = CalculateHash();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline 									GraphicsPipelineInfo(
		const GraphicsPipelineInfo		&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline GraphicsPipelineInfo			&	operator=(
		const GraphicsPipelineInfo		&	other
	) = default;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool								operator<(
		const GraphicsPipelineInfo		&	other
	) const
	{
		return hash < other.GetHash();
	}

	inline VkPipelineLayout					GetVulkanPipelineLayout() const
	{
		return vulkan_pipeline_layout;
	}

	inline VkRenderPass						GetVulkanRenderPass() const
	{
		return vulkan_render_pass;
	}

	inline VkPrimitiveTopology				GetPrimitiveTopology() const
	{
		return primitive_topology;
	}

	inline VkPolygonMode					GetPolygonMode() const
	{
		return polygon_mode;
	}

	inline const GraphicsShaderList		&	GetGraphicsShaderList_DEPRICATED() const
	{
		return graphics_shader_list;
	}

	inline VkSampleCountFlags				GetSampleCount() const
	{
		return sample_count;
	}

	inline VkBool32							GetEnableBlending() const
	{
		return enable_blending;
	}

	inline size_t							GetHash() const
	{
		return hash;
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr size_t					CalculateHash() const
	{
		Hasher hasher;
		hasher.Hash( reinterpret_cast<size_t>( vulkan_pipeline_layout ) );
		hasher.Hash( reinterpret_cast<size_t>( vulkan_render_pass ) );
		hasher.Hash( EnumToUnderlying( primitive_topology ) );
		hasher.Hash( EnumToUnderlying( polygon_mode ) );
		hasher.Hash( graphics_shader_list.GetHash() );
		hasher.Hash( std::underlying_type_t<VkSampleCountFlagBits>( sample_count ) );
		hasher.Hash( enable_blending );
		return hasher.GetHash();
	}

	VkPipelineLayout						vulkan_pipeline_layout		= {};
	VkRenderPass							vulkan_render_pass			= {};
	VkPrimitiveTopology						primitive_topology			= {};
	VkPolygonMode							polygon_mode				= {};
	GraphicsShaderList						graphics_shader_list		= {};
	VkSampleCountFlags						sample_count				= {};
	VkBool32								enable_blending				= {};

	size_t									hash;
};



} // vulkan
} // vk2d
