#pragma once

#include "../Core/Common.h"

#include <memory>

namespace vk2d {

namespace _internal {
class RendererImpl;
class SamplerImpl;
} // _internal

class Renderer;



enum class SamplerFilter {
	NEAREST,
	LINEAR,
	CUBIC,
};

enum class SamplerMipmapMode {
	NEAREST,
	LINEAR,
};

enum class SamplerAddressMode {
	REPEAT,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER,
	MIRROR_CLAMP_TO_EDGE,
};

enum class SamplerBorderColor {
	BLACK_TRANSPARENT,
	BLACK_OPAQUE,
	WHITE_OPAQUE,
};

struct SamplerCreateInfo {
	vk2d::SamplerFilter					minification_filter				= vk2d::SamplerFilter::LINEAR;
	vk2d::SamplerFilter					magnification_filter			= vk2d::SamplerFilter::LINEAR;
	vk2d::SamplerMipmapMode				mipmap_mode						= vk2d::SamplerMipmapMode::LINEAR;
	vk2d::SamplerAddressMode			address_mode_u					= vk2d::SamplerAddressMode::REPEAT;
	vk2d::SamplerAddressMode			address_mode_v					= vk2d::SamplerAddressMode::REPEAT;
	vk2d::SamplerBorderColor			border_color					= vk2d::SamplerBorderColor::BLACK_OPAQUE;
	bool								mipmap_enable					= true;
	float								mipmap_max_anisotropy			= 16.0f;
	float								mipmap_level_of_detail_bias		= 0.0f;
	float								mipmap_min_level_of_detail		= 0.0f;
	float								mipmap_max_level_of_detail		= 128.0f;
};



class Sampler {
	friend class RendererImpl;

	VK2D_API																			Sampler(
		vk2d::_internal::RendererImpl				*	renderer_parent,
		const vk2d::SamplerCreateInfo				&	create_info );

public:
	VK2D_API																			~Sampler();

	VK2D_API bool										VK2D_APIENTRY					IsGood();

private:
	std::unique_ptr<vk2d::_internal::SamplerImpl>		impl							= {};
	bool												is_good							= {};
};



} // vk2d
