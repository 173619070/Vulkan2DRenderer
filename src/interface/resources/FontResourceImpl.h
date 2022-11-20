#pragma once

#include <core/SourceCommon.h>

#include <interface/resources/FontResource.h>

#include <containers/Rect2.hpp>
#include <containers/Color.hpp>

#include <interface/resources/ResourceImplBase.h>
#include <interface/resources/TextureResource.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace vk2d {

class FontResource;

namespace vk2d_internal {

class ResourceManagerImpl;
class ThreadPrivateResource;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FontResourceImpl:
	public ResourceImplBase
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FontResourceImpl(
		FontResource								&	my_interface,
		ResourceManagerImpl							&	resource_manager,
		uint32_t										loader_thread,
		ResourceBase								*	parent_resource,
		const std::filesystem::path					&	file_path,
		uint32_t										glyph_texel_size,
		bool											use_alpha,
		uint32_t										fallback_character,
		uint32_t										glyph_atlas_padding
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~FontResourceImpl();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceStatus										GetStatus();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceStatus										WaitUntilLoaded(
		std::chrono::nanoseconds						timeout
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceStatus										WaitUntilLoaded(
		std::chrono::steady_clock::time_point			timeout
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool												MTLoad(
		ThreadPrivateResource						*	thread_resource
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void												MTUnload(
		ThreadPrivateResource						*	thread_resource
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Rect2f												CalculateRenderedSize(
		std::string_view								text,
		size_t											font_face							= 0,
		float											kerning								= 0.0f,
		glm::vec2										scale								= glm::vec2( 1.0f, 1.0f ),
		bool											vertical							= false,
		bool											wait_for_resource_load				= true
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	size_t												GetFontFaceCount();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool												FaceExists(
		size_t											font_face
	) const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TextureResource									*	GetTextureResource();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const GlyphInfo									&	GetGlyphInfo(
		char32_t										character,
		size_t											font_face					= 0
	) const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool												IsGood() const;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct AtlasTexture {
		std::vector<Color8>								data								= {};
		uint32_t										index								= {};
		uint32_t										previous_row_height					= {};
		uint32_t										current_row_height					= {};
		uint32_t										current_write_location				= {};
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct AtlasLocation {
		AtlasTexture								*	atlas_ptr							= {};
		uint32_t										atlas_index							= {};
		Rect2u											location							= {};
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct FaceInfo {
		FT_Face											face								= {};
		std::vector<GlyphInfo>							glyph_infos							= {};
		std::map<char32_t, uint32_t>					charmap								= {};	// link character to a GlyphInfo vector
		uint32_t										fallback_glyph_index				= {};
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	AtlasTexture									*	CreateNewAtlasTexture();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	AtlasLocation										ReserveSpaceForGlyphFromAtlasTextures(
		FT_GlyphSlot									glyph,
		uint32_t										glyph_atlas_padding
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void												CopyGlyphTextureToAtlasLocation(
		AtlasLocation									atlas_location,
		const std::vector<Color8>					&	converted_texture_data
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Does everything.
	AtlasLocation										AttachGlyphToAtlas(
		FT_GlyphSlot									glyph,
		uint32_t										glyph_atlas_padding,
		const std::vector<Color8>					&	converted_texture_data
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FontResource									&	my_interface;

	bool												use_alpha							= {};
	uint32_t											glyph_texel_size					= {};
	uint32_t											glyph_atlas_padding					= {};
	uint32_t											fallback_character					= {};

	uint32_t											atlas_size							= {};

	AtlasTexture									*	current_atlas_texture				= {};
	std::vector<std::unique_ptr<AtlasTexture>>			atlas_textures						= {};
	std::vector<FaceInfo>								face_infos							= {};

	TextureResource									*	texture_resource					= {};

	bool												is_good								= {};
};

} // vk2d_internal
} // vk2d
