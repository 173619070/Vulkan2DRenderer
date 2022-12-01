#pragma once

#include <core/Common.hpp>



namespace vk2d {



// TODO: Fix this.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename ResourceManagerT,
	typename ResourceT
>
concept ResourceManagerCompatible = requires( ResourceManagerT * resource_manager, ResourceT * resource_ptr )
{
	resource_manager->DestroyResource( resource_ptr );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename ResourceT,
	typename ResourceManagerT
>
class ResourceHandleBase
{
protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceHandleBase(
		ResourceManagerT			*	resource_manager,
		ResourceT					*	resource_ptr
	) :
		resource_manager( resource_manager ),
		resource_ptr( resource_ptr )
	{}

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceHandleBase() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceHandleBase(
		const ResourceHandleBase	&	other
	)
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceHandleBase(
		ResourceHandleBase			&&	other
	)
	{
		MoveOther( std::move( other ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~ResourceHandleBase()
	{
		Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceHandleBase				&	operator=(
		const ResourceHandleBase	&	other
	)
	{
		if( other == *this ) return *this;

		Clear();
		CopyOther( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceHandleBase				&	operator=(
		ResourceHandleBase			&&	other
	)
	{
		if( other == *this ) return *this;

		Clear();
		MoveOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool								operator==(
		const ResourceHandleBase	&	other
	) const
	{
		return resource_manager == other.resource_manager && resource_ptr == other.resource_ptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool								operator!=(
		const ResourceHandleBase	&	other
	) const
	{
		return !( *this == other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceT						*	operator->()
	{
		return resource_ptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceT						&	operator*()
	{
		// TODO, throw if nullptr. Need to implement exceptions for this.
		return *resource_ptr;
	}

	// TODO: Remove this once reference counting has been implemented.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void								Clear()
	{
		assert(
			( resource_manager == nullptr && resource_ptr == nullptr ) ||
			( resource_manager != nullptr && resource_ptr != nullptr )
		);
		if( resource_manager && resource_ptr )
		{
			DecrementReferenceCount();
			resource_manager->DestroyResource( resource_ptr );
			resource_manager	= nullptr;
			resource_ptr		= nullptr;
		}
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void								CopyOther(
		const ResourceHandleBase	&	other
	)
	{
		resource_manager	= other.resource_manager;
		resource_ptr		= other.resource_ptr;
		IncrementReferenceCount();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void								MoveOther(
		ResourceHandleBase			&&	other
	)
	{
		std::swap( resource_manager, other.resource_manager );
		std::swap( resource_ptr, other.resource_ptr );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void								IncrementReferenceCount()
	{
		// TODO
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void								DecrementReferenceCount()
	{
		// TODO
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ResourceManagerT				*	resource_manager				= {};
	ResourceT						*	resource_ptr					= {};
};



} // vk2d
