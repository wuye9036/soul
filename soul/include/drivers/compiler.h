#ifndef SOUL_DRIVERS_COMPILER_H
#define SOUL_DRIVERS_COMPILER_H

#include <soul/include/drivers/drivers_forward.h>

#include <eflib/include/utility/shared_declaration.h>

#include <eflib/include/platform/boost_begin.h>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <eflib/include/platform/boost_end.h>

#include <vector>
#include <string>

namespace soul
{
	namespace common
	{
		EFLIB_DECLARE_CLASS_SHARED_PTR(diag_chat);
		EFLIB_DECLARE_CLASS_SHARED_PTR(code_source);
		EFLIB_DECLARE_CLASS_SHARED_PTR(lex_context);
	}
	namespace syntax_tree
	{
		EFLIB_DECLARE_STRUCT_SHARED_PTR(node);
	}
}

BEGIN_NS_SOUL_DRIVERS();

typedef boost::function<
	bool/*succeed*/ (
	std::string& /*[out]content*/, std::string& /*[out]native file name*/,
	std::string const& /*file name*/, bool /*is system header*/,
	bool /*check only*/ )
> include_handler_fn;

class compiler{
public:
	virtual void set_parameter( int argc, char** argv )				= 0;
	virtual void set_parameter( std::string const& cmd )			= 0;

	virtual void set_code_source( soul::common::code_source_ptr const& )	= 0;
	virtual void set_code       ( std::string const& code_text )	= 0;
	virtual void set_code_file  ( std::string const& code_file )	= 0;

	virtual void add_virtual_file(
		std::string const& file_name,
		std::string const& code_content,
		bool high_priority ) = 0;
	virtual void set_include_handler( include_handler_fn inc_handler ) = 0;

	virtual soul::common::diag_chat_ptr	compile()		= 0;
	virtual soul::syntax_tree::node_ptr	get_root() const= 0;

	virtual ~compiler(){}
};

END_NS_SOUL_DRIVERS();

#endif