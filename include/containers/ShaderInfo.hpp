#pragma once

#include <core/Common.hpp>

#include <utility/Hasher.hpp>
#include <utility/EnumHelpers.hpp>

#include <string>
#include <string_view>



namespace vk2d {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class ShaderStage : uint32_t
{
	NONE					= 0,
	VERTEX,
	TESSELLATION_CONTROL,
	TESSELLATION_EVALUATION,
	GEOMETRY,
	FRAGMENT,
	COMPUTE,
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ShaderInfo
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr						ShaderInfo() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr						ShaderInfo(
		ShaderStage							shader_stage,
		std::string_view					shader_name,
		std::string_view					shader_code
	) :
		stage( shader_stage ),
		name( shader_name ),
		code( shader_code )
	{
		hash = CalculateHash();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr ShaderStage			GetStage() const
	{
		return stage;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr const std::string	&	GetName() const
	{
		return name;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr const std::string	&	GetCode() const
	{
		return code;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr size_t					GetHash() const
	{
		return hash;
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr size_t					CalculateHash()
	{
		Hasher hasher;
		hasher.Hash( EnumToUnderlying( stage ) );
		hasher.Hash( name );
		hasher.Hash( code );
		return hasher.GetHash();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ShaderStage								stage;
	std::string								name;
	std::string								code;

	size_t									hash;
};



} // vk2d
