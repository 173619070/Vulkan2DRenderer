#pragma once

#include <core/SourceCommon.hpp>
#include <interface/resources/material/MaterialResource.hpp>
#include <interface/resources/material/ShaderMemberInfo.hpp>

#include <interface/resources/resource_base/ResourceImplBase.hpp>

#include <system/ThreadPrivateResources.hpp>
#include <vulkan/shaders/ShaderHandle.hpp>



namespace vk2d {
namespace vk2d_internal {



class MaterialResourceImpl :
	public ResourceImplBase
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MaterialResourceImpl(
		MaterialResource								&	my_interface,
		ResourceManagerImpl								&	resource_manager,
		uint32_t											loader_thread,
		ResourceBase									*	parent_resource,
		std::span<ShaderMemberInfo>							vertex_members,
		const MaterialCreateInfo						&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~MaterialResourceImpl();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool													MTLoad(
		ThreadPrivateResource							*	thread_resource
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													MTUnload(
		ThreadPrivateResource							*	thread_resource
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceStatus											GetStatus();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceStatus											WaitUntilLoaded(
		std::chrono::nanoseconds							timeout
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceStatus											WaitUntilLoaded(
		std::chrono::steady_clock::time_point				timeout
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool													IsGood() const;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													SchedulePostLoadCleanup();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MaterialResource									&	my_interface;
	ResourceManagerImpl									&	resource_manager;
	ThreadMaterialLoaderResource						*	loader_thread_resource						= {};

	std::vector<ShaderMemberInfo>							vertex_members;

	vulkan::ShaderHandle									vertex_shader;
	vulkan::ShaderHandle									fragment_shader;

	MaterialCreateInfo										create_info_copy							= {};

	bool													is_good										= {};
};



} // vk2d_internal
} // vk2d
