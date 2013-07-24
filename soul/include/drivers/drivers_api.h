#ifndef SOUL_DRIVERS_DRIVERS_API_H
#define SOUL_DRIVERS_DRIVERS_API_H

#include <soul/include/drivers/drivers_forward.h>

#include <soul/include/drivers/compiler.h>

#include <eflib/include/utility/shared_declaration.h>

#include <eflib/include/platform/boost_begin.h>
#include <boost/shared_ptr.hpp>
#include <eflib/include/platform/boost_end.h>

#include <string>

#if defined(sasl_drivers_EXPORTS)
#	define SOUL_DRIVERS_API __declspec(dllexport)
#elif defined(SOUL_STATIC_DRIVERS)
#	define SOUL_DRIVERS_API
#else
#	define SOUL_DRIVERS_API __declspec(dllimport)
#endif

namespace soul
{
	namespace drivers
	{
		EFLIB_DECLARE_CLASS_SHARED_PTR(compiler);
	}
	namespace shims
	{
		EFLIB_DECLARE_CLASS_SHARED_PTR(ia_shim);
		EFLIB_DECLARE_CLASS_SHARED_PTR(interp_shim);
	}
}

extern "C"
{
	SOUL_DRIVERS_API void sasl_create_compiler		(soul::drivers::compiler_ptr&	out);
	SOUL_DRIVERS_API void sasl_create_ia_shim		(soul::shims::ia_shim_ptr&		out);
	SOUL_DRIVERS_API void sasl_create_interp_shim	(soul::shims::interp_shim_ptr&  out);
};

#endif