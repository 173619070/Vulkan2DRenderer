#pragma once

#include <core/SourceCommon.h>

#include <types/MeshPrimitives.hpp>

#include <vulkan/utils/VulkanMemoryManagement.hpp>
#include <system/DescriptorSet.h>

#include <interface/WindowImpl.h>
#include <interface/InstanceImpl.h>



namespace vk2d {

namespace vk2d_internal {

class MeshBuffer;

template<typename T>
class MeshBufferBlock;

using IndexBufferBlocks									= std::vector<std::unique_ptr<MeshBufferBlock<uint32_t>>>;
using VertexBufferBlocks								= std::vector<std::unique_ptr<MeshBufferBlock<Vertex>>>;
using TextureChannelBufferBlocks						= std::vector<std::unique_ptr<MeshBufferBlock<float>>>;
using TransformationBufferBlocks						= std::vector<std::unique_ptr<MeshBufferBlock<glm::mat4>>>;

enum class MeshBufferDescriptorSetType : uint32_t {
	NONE,
	UNIFORM,
	STORAGE,
};



// TODO:
// MeshBuffer needs to be updated to have more functionality.
// New goals are:
// - Keep data on the GPU if possible. Similar to static and dynamic meshes where dynamic meshes are updated every frame but
//   static meshes are kept in VRAM between draws until they're manually freed.
// - Allow custom Vertex data. Which must find it's way into the custom user made shaders.
//
// Another thing that must be implemented is custom mesh data. Since we're implementing custom shader with ability to pass data
// to the shader, we must account for different sized data at runtime.
//
// I think this requires a total overhaul of MeshBuffer. Vertex and index blocks will not change, they're going to be reserved
// regardless of other data. Texture channel weight data and transformation blocks may go away by default.
//
// Right now MeshBuffer is a bit of a mess anyhow. Should tidy it up at the very least. Currently there are 4 distinct buffers
// for indices, vertices, texture channel weights and transformations.
//
// Indices, vertices, texture channel weights, transformation block all have different data rates, Vertices just define the
// corners, indices are per number of corners to draw, texture channel weights is number of texture layers * number of vertices
// and transformations are per instance.
// 
// Consider interleaving indices, vertices... and the whole mesh into a single MeshBufferBlock and add offsets to different data
// sections, basically this would make the entire mesh strictly contained within a single range within a single buffer, this
// would maximize the space usage.
// Vertex could also be given the same template parameter pack as the Mesh, this way the Vertex will actually contain the
// custom data we want to send to the shader, though this also means we need to adjust what the Vertex looks like inside the
// shader.
// This method requires making sure alignment for each vertex parameter is properly set and won't cause problems in different
// GPU architectures. Also alignment must be checked between data sections, eg, between index and vertex buffers.
//
// On the other hand, it may be more efficient to keep indices in one list, vertices on another and so on, exactly like
// MeshBuffer does right now. The only difference then would be a variable size Vertex list per mesh.
// Consider having static mesh in VRAM, if we want to send per-draw data to the mesh, if we increase the number of draws, we may
// need to migrate the whole mesh around to fit the per-draw data at the end of the mesh data.
//
// We could also introduce a new buffer for all custom data and interleave it just like in the Vertex buffer. This would require
// a separate buffer binding however, which isn't a problem necessarily but may not be as efficient as adjusting Vertex size,
// but would potentially be easier to implement... Not sure about this though.
//
// Currently I'm leaning on having the MeshBuffer work mostly as it is right now, except allow custom size MeshBufferBlock.
// For custom data, either interleave custom data into Vertex class, or create a new MeshBufferBlock for custom vertex data.
// Introduce static and dynamic storage locations. Dynamic works same as current MeshBuffer, static works more like a memory
// pool of sorts, new uploads are tracked and recorded to command buffers once. Freeing is done manually it is not automatically
// tracked, old data is ignored but memory is set as availble in the pool side.
// Per-draw data should be cleared after every draw and resent every time, completely dynamic as the MeshBuffer is right now.
//
// Currently VK2D has 2 shader interfaces, one for more simple renders and another for more complicated ones, it would be nice
// to have a single interface instead of 2. See if it would be possible. I think it is allowed to have an unbound set in shader
// if it is never used, this should allow us to have only one shader interface. Alternatively we could just bind it to a
// fallback data buffer. Combining the shader interfaces into one may have a small performance hit as we're now using the larger
// interface for everything, mostly because the vertex and index buffers are also bound to fragment shaders.
//
// I think exposing vertex and index buffers for custom fragment shaders is okay, I don't think it's worth the effort to ask the
// user about it. Implementing this would require new vulkan pipeline layout, vulkan pipeline, shaders, descriptor set layout
// and descriptor set so it's a lot of work.
//


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief		MeshBuffer represents a collection of all mesh data that is about to be rendered.
///
///				MeshBuffer consists of dynamically and automatically expanding storage, it has 2 internal buffers, one in RAM
///				and another in VRAM, mesh buffers may be copied over to the VRAM by submitting a copy command to Vulkan command
///				buffer.
class MeshBuffer {
	template<typename T>
	friend class MeshBufferBlock;
public:
	struct MeshBlockLocationInfo {
		bool									success								= {};

		MeshBufferBlock<uint32_t>			*	index_block							= {};
		MeshBufferBlock<Vertex>				*	vertex_block						= {};
		MeshBufferBlock<float>				*	texture_channel_weight_block		= {};
		MeshBufferBlock<glm::mat4>			*	transformation_block				= {};

		uint32_t								index_size							= {};	// size of data.
		VkDeviceSize							index_byte_size						= {};	// size of data in bytes.
		uint32_t								index_offset						= {};	// offset into buffer.
		VkDeviceSize							index_byte_offset					= {};	// offset into buffer in bytes.

		uint32_t								vertex_size							= {};	// size of data.
		VkDeviceSize							vertex_byte_size					= {};	// size of data in bytes.
		uint32_t								vertex_offset						= {};	// offset into buffer.
		VkDeviceSize							vertex_byte_offset					= {};	// offset into buffer in bytes.

		uint32_t								texture_channel_weight_size			= {};	// size of data.
		VkDeviceSize							texture_channel_weight_byte_size	= {};	// size of data in bytes.
		uint32_t								texture_channel_weight_offset		= {};	// offset into buffer.
		VkDeviceSize							texture_channel_weight_byte_offset	= {};	// offset into buffer in bytes.

		uint32_t								transformation_size					= {};	// size of data.
		VkDeviceSize							transformation_byte_size			= {};	// size of data in bytes.
		uint32_t								transformation_offset				= {};	// offset into buffer.
		VkDeviceSize							transformation_byte_offset			= {};	// offset into buffer in bytes.
	};

	struct PushResult {
		MeshBuffer::MeshBlockLocationInfo		location_info;
		bool									success;
		inline explicit operator bool()
		{
			return	success;
		}
	};

	MeshBuffer(
		InstanceImpl						&	instance
	);

	// Pushes mesh into render list, dynamically allocates new buffers
	// if needed, binds the new buffers to command buffer if needed
	// and adds vertex and index data to host visible buffer.
	// Returns mesh offsets of whatever buffer object this mesh was
	// put into, needed when recording a Vulkan draw command.
	MeshBuffer::PushResult						CmdPushMesh(
		VkCommandBuffer							command_buffer,
		const std::vector<uint32_t>			&	new_indices,
		const std::vector<Vertex>			&	new_vertices,
		const std::vector<float>			&	new_texture_channel_weights,
		const std::vector<glm::mat4>		&	new_transformations
	);

	bool										CmdUploadMeshDataToGPU(
		VkCommandBuffer							command_buffer
	);

	// Gets the total amount of individual meshes that have been pushed so far.
	uint32_t									GetPushedMeshCount();

	// This gets the total amount of vertices already pushed in
	uint32_t									GetTotalVertexCount();

	// This gets the total amount of indices already pushed in
	uint32_t									GetTotalIndexCount();

	// This gets the total amount of texture channels already pushed in
	uint32_t									GetTotalTextureChannelCount();

	// This gets the total amount of transformations already pushed in
	uint32_t									GetTotalTransformationCount();

private:
	MeshBuffer::MeshBlockLocationInfo			ReserveSpaceForMesh(
		uint32_t								index_count,
		uint32_t								vertex_count,
		uint32_t								texture_channel_weight_count,
		uint32_t								transformation_count
	);

	// Find an index buffer with enough space to hold the data, if none found
	// this function will allocate a new buffer that will have enough space.
	// Returns nullptr on failure.
	MeshBufferBlock<uint32_t>				*	FindIndexBufferWithEnoughSpace(
		uint32_t								count
	);

	// Find an index buffer with enough space to hold the data, if none found
	// this function will allocate a new buffer that will have enough space.
	// Returns nullptr on failure.
	MeshBufferBlock<Vertex>					*	FindVertexBufferWithEnoughSpace(
		uint32_t								count
	);

	// Find an index buffer with enough space to hold the data, if none found
	// this function will allocate a new buffer that will have enough space.
	// Returns nullptr on failure.
	MeshBufferBlock<float>					*	FindTextureChannelBufferWithEnoughSpace(
		uint32_t								count
	);

	// Find a transformation buffer with enough space to hold the data, if none found
	// this function will allocate a new buffer that will have enough space.
	// Returns nullptr on failure.
	MeshBufferBlock<glm::mat4>				*	FindTransformationBufferWithEnoughSpace(
		uint32_t								count
	);

	// Creates a new buffer block and stores it internally,
	// returns a pointer to it if successful or nullptr on failure.
	MeshBufferBlock<uint32_t>				*	AllocateIndexBufferBlockAndStore(
		VkDeviceSize							byte_size
	);

	// Creates a new buffer block and stores it internally,
	// returns a pointer to it if successful or nullptr on failure.
	MeshBufferBlock<Vertex>					*	AllocateVertexBufferBlockAndStore(
		VkDeviceSize							byte_size
	);

	// Creates a new buffer block and stores it internally,
	// returns a pointer to it if successful or nullptr on failure.
	MeshBufferBlock<float>					*	AllocateTextureChannelBufferBlockAndStore(
		VkDeviceSize							byte_size
	);

	// Creates a new buffer block and stores it internally,
	// returns a pointer to it if successful or nullptr on failure.
	MeshBufferBlock<glm::mat4>				*	AllocateTransformationBufferBlockAndStore(
		VkDeviceSize							byte_size
	);

	// Removes a buffer block with matching pointer from internal storage.
	void										FreeBufferBlockFromStorage(
		MeshBufferBlock<uint32_t>			*	buffer_block
	);

	// Removes a buffer block with matching pointer from internal storage.
	void										FreeBufferBlockFromStorage(
		MeshBufferBlock<Vertex>				*	buffer_block
	);

	// Removes a buffer block with matching pointer from internal storage.
	void										FreeBufferBlockFromStorage(
		MeshBufferBlock<float>				*	buffer_block
	);

	// Removes a buffer block with matching pointer from internal storage.
	void										FreeBufferBlockFromStorage(
		MeshBufferBlock<glm::mat4>			*	buffer_block
	);

	InstanceImpl							&	instance;

	bool										first_draw									= {};

	uint32_t									pushed_mesh_count							= {};
	uint32_t									pushed_index_count							= {};
	uint32_t									pushed_vertex_count							= {};
	uint32_t									pushed_texture_channel_weight_count			= {};
	uint32_t									pushed_transformation_count					= {};

	MeshBufferBlock<uint32_t>				*	bound_index_buffer_block					= {};
	MeshBufferBlock<Vertex>					*	bound_vertex_buffer_block					= {};
	MeshBufferBlock<float>					*	bound_texture_channel_weight_buffer_block	= {};
	MeshBufferBlock<glm::mat4>				*	bound_transformation_buffer_block			= {};

	IndexBufferBlocks							index_buffer_blocks							= {};
	VertexBufferBlocks							vertex_buffer_blocks						= {};
	TextureChannelBufferBlocks					texture_channel_weight_buffer_blocks		= {};
	TransformationBufferBlocks					transformation_buffer_blocks				= {};

	IndexBufferBlocks::iterator					current_index_buffer_block					= {};
	VertexBufferBlocks::iterator				current_vertex_buffer_block					= {};
	TextureChannelBufferBlocks::iterator		current_texture_channel_weight_buffer_block	= {};
	TransformationBufferBlocks					current_transformation_buffer_block			= {};
};







template<typename T>
class MeshBufferBlock {
	friend class MeshBuffer;

public:
	MeshBufferBlock(
		MeshBuffer							&	parent,
		VkDeviceSize							buffer_byte_size,
		VkBufferUsageFlags						buffer_usage_flags,
		MeshBufferDescriptorSetType				descriptor_set_type
	) :
		parent( parent )
	{
		assert( buffer_byte_size );
		assert( buffer_usage_flags );

		auto & instance				= parent.instance;
		auto memory_pool			= instance.GetVulkanDevice().GetDeviceMemoryPool();

		total_byte_size				= CalculateAlignmentForBuffer(
			buffer_byte_size,
			instance.GetVulkanDevice().GetVulkanPhysicalDeviceProperties().limits
		);

		host_data.reserve( total_byte_size / sizeof( T ) + 1 );

		// Create staging buffer
		{
			VkBufferCreateInfo buffer_create_info {};
			buffer_create_info.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buffer_create_info.pNext					= nullptr;
			buffer_create_info.flags					= 0;
			buffer_create_info.size						= total_byte_size;
			buffer_create_info.usage					= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			buffer_create_info.sharingMode				= VK_SHARING_MODE_EXCLUSIVE;
			buffer_create_info.queueFamilyIndexCount	= 0;
			buffer_create_info.pQueueFamilyIndices		= nullptr;
			staging_buffer = memory_pool->CreateCompleteBufferResource(
				&buffer_create_info,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			if( staging_buffer != VK_SUCCESS ) {
				instance.Report( ReportSeverity::CRITICAL_ERROR, "Internal error: Cannot create MeshBufferBlock, cannot create staging buffer!" );
				return;
			}
		}

		// Create device buffer
		{
			VkBufferCreateInfo buffer_create_info {};
			buffer_create_info.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buffer_create_info.pNext					= nullptr;
			buffer_create_info.flags					= 0;
			buffer_create_info.size						= total_byte_size;
			buffer_create_info.usage					= VK_BUFFER_USAGE_TRANSFER_DST_BIT | buffer_usage_flags;
			buffer_create_info.sharingMode				= VK_SHARING_MODE_EXCLUSIVE;
			buffer_create_info.queueFamilyIndexCount	= 0;
			buffer_create_info.pQueueFamilyIndices		= nullptr;
			device_buffer = memory_pool->CreateCompleteBufferResource(
				&buffer_create_info,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);
			if( device_buffer != VK_SUCCESS ) {
				instance.Report( ReportSeverity::CRITICAL_ERROR, "Internal error: Cannot create MeshBufferBlock, cannot create device buffer!" );
				return;
			}
		}

		// Create descriptor set
		{
			auto AllocateAndUpdateDescriptorSet = [this, &instance](
				const DescriptorSetLayout	&	descriptor_set_layout,
				VkDescriptorType				descriptor_type
			) -> PoolDescriptorSet
			{
				// WARNING: MeshBufferBlock::descriptor_set allocation and freeing needs to be thread specific if we ever start doing multithreaded rendering.
				auto ret = instance.AllocateDescriptorSet(
					descriptor_set_layout
				);

				VkDescriptorBufferInfo descriptor_write_buffer_info {};
				descriptor_write_buffer_info.buffer = device_buffer.buffer;
				descriptor_write_buffer_info.offset = 0;
				descriptor_write_buffer_info.range = device_buffer.memory.GetSize();
				std::array<VkWriteDescriptorSet, 1> descriptor_write {};
				descriptor_write[ 0 ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptor_write[ 0 ].pNext = nullptr;
				descriptor_write[ 0 ].dstSet = ret.descriptorSet;
				descriptor_write[ 0 ].dstBinding = 0;
				descriptor_write[ 0 ].dstArrayElement = 0;
				descriptor_write[ 0 ].descriptorCount = 1;
				descriptor_write[ 0 ].descriptorType = descriptor_type;
				descriptor_write[ 0 ].pImageInfo = nullptr;
				descriptor_write[ 0 ].pBufferInfo = &descriptor_write_buffer_info;
				descriptor_write[ 0 ].pTexelBufferView = nullptr;
				vkUpdateDescriptorSets(
					instance.GetVulkanDevice(),
					uint32_t( descriptor_write.size() ), descriptor_write.data(),
					0, nullptr
				);

				return ret;
			};

			switch( descriptor_set_type ) {
			case MeshBufferDescriptorSetType::NONE:
				break;
			case MeshBufferDescriptorSetType::UNIFORM:
			{
				descriptor_set = AllocateAndUpdateDescriptorSet(
					instance.GetGraphicsUniformBufferDescriptorSetLayout(),
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
				);
			}
				break;
			case MeshBufferDescriptorSetType::STORAGE:
			{
				descriptor_set = AllocateAndUpdateDescriptorSet(
					instance.GetGraphicsStorageBufferDescriptorSetLayout(),
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
				);
			}
				break;
			default:
				assert( 0 && "Should never reach this! Check how 'descriptor_set_type' was set." );
				break;
			}
		}
		is_good = true;
	}

	~MeshBufferBlock()
	{
		auto memory_pool = parent.instance.GetVulkanDevice().GetDeviceMemoryPool();

		// WARNING: MeshBufferBlock::descriptor_set allocation and freeing needs to be thread specific if we ever start doing multithreaded rendering.
		parent.instance.FreeDescriptorSet( descriptor_set );
		memory_pool->FreeCompleteResource( device_buffer );
		memory_pool->FreeCompleteResource( staging_buffer );
	}

	bool 										CopyVectorsToStagingBuffers()
	{
		auto mapped_memory	= staging_buffer.memory.Map<T>();
		if( !mapped_memory ) {
			parent.instance.Report( ReportSeverity::CRITICAL_ERROR, "Internal error: Cannot copy mesh buffer block to  map staging buffer memory" );
			return false;
		} else {
			std::memcpy( mapped_memory, host_data.data(), used_byte_size );
			staging_buffer.memory.Unmap();

			host_data.clear();

			return true;
		}
	}

	// Checks if something fits into this MeshBufferBlock.
	// Parameter count is not in byte size, if this MeshBufferBlock is type
	// float and parameter count is 1 then space for 4 bytes is checked for.
	bool										CheckDataFits(
		uint32_t								count
	)
	{
		VkDeviceSize	reserve_size		= count * sizeof( T );
		if( used_byte_size + reserve_size <= total_byte_size ) {
			return true;
		} else {
			return false;
		}
	}

	// If data fits, this will reserve space for it in the buffer.
	// Returns the location in bytes to the beginning of the reserved space.
	// Parameter count is not in byte size, if this MeshBufferBlock is type
	// float and parameter count is 1 then space for 4 bytes is reserved.
	VkDeviceSize								ReserveSpace(
		uint32_t								count
	)
	{
		VkDeviceSize reserve_size	= count * sizeof( T );
		assert( used_byte_size + reserve_size <= total_byte_size );
		auto ret					= used_byte_size;
		used_byte_size				+= reserve_size;
		return ret;
	}

	bool										IsGood()
	{
		return is_good;
	}

private:
	MeshBuffer								&	parent;

	// TODO: Get rid of host_data vector.
	// Currently when submitting a mesh, it's contents are copied here, then they're copied to Vulkan RAM buffer, then
	// finally into VRAM via copy command in command buffer. This is 3 copies just to submit a mesh. This is needed because
	// currently the Vulkan RAM buffer may be read from by the GPU while we're writing to it. This could be mitigated by
	// making Vulkan RAM buffer double buffered, so that writes won't interfere with the GPU upload.
	// Make sure we even need a double buffering here. Window::BeginRender() should synchronize the frame and the GPU, but
	// I don't remember how I designed it, it may be that some operations will not synchronize until Window::EndRender() is
	// called.
	std::vector<T>								host_data					= {};

	VkDeviceSize								total_byte_size				= {};	// Total size of buffer in bytes.
	VkDeviceSize								used_byte_size				= {};	// Used size of uint data in bytes.

	CompleteBufferResource						staging_buffer				= {};
	CompleteBufferResource						device_buffer				= {};
	PoolDescriptorSet							descriptor_set				= {};

	bool										is_good						= {};
};



} // vk2d_internal

} // vk2d
