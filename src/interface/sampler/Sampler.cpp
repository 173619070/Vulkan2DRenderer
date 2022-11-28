
#include <core/SourceCommon.hpp>

#include "../instance/InstanceImpl.hpp"
#include "SamplerImpl.hpp"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API vk2d::Sampler_DEPRICATED::Sampler_DEPRICATED(
	vk2d_internal::InstanceImpl		&	instance,
	const SamplerCreateInfo			&	create_info
)
{
	impl = std::make_unique<vk2d_internal::SamplerImpl_DEPRICATED>(
		*this,
		instance,
		create_info
	);

	if( !impl || !impl->IsGood() ) {
		impl = nullptr;
		instance.Report( ReportSeverity::NON_CRITICAL_ERROR, "Internal error: Cannot create sampler implementation!" );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API vk2d::Sampler_DEPRICATED::~Sampler_DEPRICATED()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VK2D_API bool vk2d::Sampler_DEPRICATED::IsGood() const
{
	return !!impl && impl->IsGood();
}
