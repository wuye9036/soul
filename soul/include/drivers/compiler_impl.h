#ifndef SOUL_DRIVER_DRIVER_IMPL_H
#define SOUL_DRIVER_DRIVER_IMPL_H

#include <soul/include/drivers/drivers_forward.h>
#include <soul/include/drivers/compiler.h>
#include <soul/include/drivers/options.h>
#include <eflib/include/string/ustring.h>

#include <eflib/include/platform/boost_begin.h>
#include <boost/unordered_map.hpp>
#include <eflib/include/platform/boost_end.h>

BEGIN_NS_SOUL_DRIVERS();

class null_compiler: public compiler
{
public:
	null_compiler();

	virtual void set_parameter( int argc, char** argv );
	virtual void set_parameter( std::string const& /*cmd*/ ){}

	virtual void set_code       ( std::string const& /*code_text*/ ){}
	virtual void set_code_file  ( std::string const& /*file_name*/ ){}
	virtual void set_code_source( soul::common::code_source_ptr const& ){}
	virtual void set_lex_context( soul::common::lex_context_ptr const& ){}

	/// Only support by default code_source.
	virtual void add_virtual_file(
		std::string const& /*file_name*/,
		std::string const& /*code_content*/,
		bool /*high_priority*/ ){}
	/// Only support by default code_source.
	virtual void set_include_handler( include_handler_fn /*inc_handler*/ ){}
	/// Only support by default code source.
	virtual void add_include_path( std::string const& /*inc_path*/ ){}
	/// Only support by default code source.
	virtual void add_sysinclude_path( std::string const& /*sys_path*/ ){}
	/// Only support by default code source.
	virtual void clear_sysinclude_paths(){}
	/// Only support by default code source.
	virtual void add_macro( std::string const& /*macro*/, bool /*predef*/ ){}
	/// Only support by default code source.
	virtual void remove_macro( std::string const& /*macro*/ ){}
	/// Only support by default code source.
	virtual void clear_macros(){}

	virtual soul::common::diag_chat_ptr compile()
	{
		return soul::common::diag_chat_ptr();
	}

	virtual soul::syntax_tree::node_ptr			get_root() const
	{
		return soul::syntax_tree::node_ptr();
	}
};

class compiler_impl: public compiler{
public:
	compiler_impl();

	// All setting functions must be called before calling compile().
	virtual void set_parameter( int argc, char** argv );
	virtual void set_parameter( std::string const& cmd );

	virtual void set_code       ( std::string const& code_text );
	virtual void set_code_file  ( std::string const& code_file );
	virtual void set_code_source( boost::shared_ptr<soul::common::code_source> const& );
	virtual void set_lex_context( boost::shared_ptr<soul::common::lex_context> const& );

	/// Only support by default code_source.
	virtual void add_virtual_file( std::string const& file_name, std::string const& code_content, bool high_priority );
	/// Only support by default code_source.
	virtual void set_include_handler( include_handler_fn inc_handler );
	/// Only support by default code source.
	virtual void add_include_path( std::string const& inc_path );
	/// Only support by default code source.
	virtual void add_sysinclude_path( std::string const& sys_path );
	/// Only support by default code source.
	virtual void clear_sysinclude_paths();
	/// Only support by default code source.
	virtual void add_macro( std::string const& macro, bool predef );
	/// Only support by default code source.
	virtual void remove_macro( std::string const& macro );
	/// Only support by default code source.
	virtual void clear_macros();

	virtual soul::common::diag_chat_ptr			compile();
	virtual soul::syntax_tree::node_ptr			get_root() const;

	boost::program_options::variables_map const &	variables() const;
	options_display_info const &					display_info() const;
	options_io const &								io_info() const;

private:
	compiler_impl( compiler_impl const& );
	compiler_impl& operator = ( compiler_impl const& );

	template <typename ParserT> bool parse( ParserT& parser );

	soul::syntax_tree::node_ptr				mroot;

	// Options
	options_global			opt_global;
	options_display_info	opt_disp;
	options_io				opt_io;
	option_macros			opt_macros;
	options_includes		opt_includes;

	po::options_description	desc;
	po::variables_map		vm;

	// Overridden options
	boost::shared_ptr<soul::common::code_source>	user_code_src;
	boost::shared_ptr<soul::common::lex_context>	user_lex_ctxt;

	typedef boost::unordered_map< std::string,
		std::pair<std::string, bool> > virtual_file_dict;
	enum macro_states
	{
		ms_normal,
		ms_predef,
		ms_remove
	};
	std::vector<std::string>	sys_paths, inc_paths;
	std::vector< std::pair<std::string, macro_states> > macros;
	include_handler_fn			user_inc_handler;
	virtual_file_dict			virtual_files;
};

END_NS_SOUL_DRIVERS();

#endif