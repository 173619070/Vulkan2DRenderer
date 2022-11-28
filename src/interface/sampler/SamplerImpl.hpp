#pragma once

#include <core/SourceCommon.hpp>

#include <vulkan/descriptor_set/DescriptorSet.hpp>
#include <interface/sampler/Sampler.hpp>

#include <vulkan/utils/VulkanMemoryManagement.hpp>

namespace vk2d {
namespace vk2d_internal {



class SamplerImpl_DEPRICATED {
public:
	SamplerImpl_DEPRICATED(
		Sampler_DEPRICATED					&	sampler,
		InstanceImpl						&	instance,
		const SamplerCreateInfo				&	create_info );

	~SamplerImpl_DEPRICATED();

	VkSampler									GetVulkanSampler() const;
	VkBuffer									GetVulkanBufferForSamplerData() const;
	glm::uvec2									GetBorderColorEnable() const;
	bool										IsAnyBorderColorEnabled() const;

	bool										IsGood() const;



	struct BufferData {
		alignas( 16 )	Colorf					borderColor			= {};	// Border color
		alignas( 8 )	glm::uvec2				borderColorEnable	= {};	// Border color enable
	};

private:
	Sampler_DEPRICATED						&	my_interface;
	InstanceImpl							&	instance;
	VkDevice									vk_device			= {};

	VkSampler									vk_sampler			= {};
	vulkan::CompleteBufferResource				sampler_data		= {};

	glm::uvec2									border_color_enable	= {};

	bool										is_good				= {};
};



} // vk2d_internal

} // vk2d
