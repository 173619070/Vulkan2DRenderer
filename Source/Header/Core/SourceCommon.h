#pragma once

#define VK2D_LIBRARY_EXPORT				1

#include "../../BuildOptions.h"
#include "../../../Include/Core/Common.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <mutex>

#include <vulkan/vulkan.h>

namespace vk2d {

namespace _internal {



enum class CommandBufferCheckpointType : uint32_t {
	BEGIN_COMMAND_BUFFER,
	END_COMMAND_BUFFER,
	BEGIN_RENDER_PASS,
	END_RENDER_PASS,
	BIND_PIPELINE,
	BIND_DESCRIPTOR_SET,
	BIND_INDEX_BUFFER,
	BIND_VERTEX_BUFFER,
	BEGIN_DRAW,
	END_DRAW,
	GENERIC,
};

struct CommandBufferCheckpointData {
	CommandBufferCheckpointData(
		std::string						name,
		CommandBufferCheckpointType		type,
		CommandBufferCheckpointData	*	previous
	) :
		name( name ),
		type( type ),
		previous( previous )
	{}

	std::string							name			= {};
	CommandBufferCheckpointType			type			= {};
	CommandBufferCheckpointData		*	previous		= {};
};

#if VK2D_BUILD_OPTION_VULKAN_COMMAND_BUFFER_CHECKMARKS && VK2D_BUILD_OPTION_VULKAN_VALIDATION && VK2D_DEBUG_ENABLE

void									SetCommandBufferCheckpointQueue(
	VkDevice							device,
	VkQueue								queue,
	std::mutex						*	queue_mutex );

void									CmdInsertCommandBufferCheckpoint(
	VkCommandBuffer						command_buffer,
	std::string							name,
	CommandBufferCheckpointType			type );

std::vector<VkCheckpointDataNV>			GetCommandBufferCheckpoints();

std::string								CommandBufferCheckpointTypeToString( CommandBufferCheckpointType type );

#else

inline void								CmdInsertCommandBufferCheckpoint(
	VkCommandBuffer						command_buffer,
	std::string							name,
	CommandBufferCheckpointType			type )
{};

#endif



std::string				VkResultToString( VkResult result );
std::string				VkPipelineStageFlagBitsToString( VkPipelineStageFlagBits flags );



} // _internal

} // vk2d
