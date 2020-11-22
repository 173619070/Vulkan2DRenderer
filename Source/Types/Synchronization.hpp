#pragma once

#include "../Core/SourceCommon.h"

namespace vk2d {
namespace _internal {



/// @brief		This is used to synchronize work.
class Fence
{
public:
	Fence()											= default;
	Fence( const vk2d::_internal::Fence & other )	= delete;
	Fence( vk2d::_internal::Fence && other )		= default;

	/// @brief		Unblocks waiting on this fence, any thread calling
	///				vk2d::_interna::Wait() is allowed to continue after calling this.
	void											Set();

	/// @brief		Tests without blocking if this fence is set.
	/// @return		true if set, false if not set.
	bool											IsSet();

	/// @brief		Blocks calling thread until this fence is set by another thread.
	/// @param[in]	timeout
	///				Maximum time to wait before returning false.
	/// @return		true if successfully waited, false if timeout.
	bool											Wait(
		std::chrono::nanoseconds					timeout		= std::chrono::nanoseconds::max() );

	/// @brief		Blocks calling thread until this fence is set by another thread.
	/// @param[in]	timeout
	///				Time point to wait for before returning false.
	/// @return		true if successfully waited, false if timeout.
	bool											Wait(
		std::chrono::steady_clock::time_point		timeout );

private:
	std::mutex										condition_variable_mutex;
	std::condition_variable							condition;
	std::atomic_bool								is_set					= {};
};



/// @brief		This is used to synchronize an object. Similar in function to std::future.
/// @tparam		T
///				Type of object or value contained in this class and we want to have fenced access to.
template<typename T>
class FencedObject
{
public:

	/// @brief		Sets the contained value and allows getting the value.
	/// @param		value
	///				Value that we wish to copy from to internal memory.
	/// @return		Contained value reference.
	void							Set(
		const T					&	value )
	{
		this->value = value;
		fence.Set();
	}

	/// @brief		Sets the contained value and allows getting the value.
	/// @param		value
	///				Value that we wish to copy from to internal memory.
	/// @return		Contained value reference.
	void							Set(
		T						&&	value )
	{
		this->value = std::move( value );
		fence.Set();
	}

	/// @brief		Gets access to contained value, waits for signal first.
	/// @return		Contained value reference.
	T							&	Get()
	{
		fence.Wait();
		return this->value;
	}

	/// @brief		Sets the contained value and allows getting the value.
	/// @param		value
	///				Value that we wish to copy from to internal memory.
	/// @return		Contained value reference.
	T							&	operator=( const T & value )
	{
		Set( value );
	}

	/// @brief		Sets the contained value and allows getting the value.
	/// @param		value
	///				Value that we wish to copy from to internal memory.
	/// @return		Contained value reference.
	T							&	operator=( T && value )
	{
		Set( std::move( value ) );
	}

	/// @brief		Gets access to contained value, waits for signal first.
	/// @return		Contained value reference.
	T							&	operator->()
	{
		return Get();
	}

	/// @brief		Gets access to contained value, waits for signal first.
	/// @return		Contained value reference.
	T							&	operator*()
	{
		return Get();
	}

private:
	Fence							fence					= {};
	T								value;
};



} // _internal
} // vk2d
