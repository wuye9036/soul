#include <soul/include/drivers/compiler_impl.h>

#include <soul/include/drivers/compiler_diags.h>
#include <soul/include/drivers/code_sources.h>
#include <soul/include/drivers/options.h>

#include <soul/include/parser/parse_api.h>
#include <soul/include/parser/diags.h>
#include <soul/include/syntax_tree/program.h>
#include <soul/include/common/diag_chat.h>

#include <eflib/include/diagnostics/profiler.h>
#include <eflib/include/string/ustring.h>
#include <eflib/include/utility/shared_declaration.h>
#include <eflib/include/diagnostics/assert.h>

#include <eflib/include/platform/boost_begin.h>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <eflib/include/platform/boost_end.h>

#include <fstream>
#include <math.h>

namespace po = boost::program_options;

EFLIB_USING_SHARED_PTR(soul::syntax_tree, node);
EFLIB_USING_SHARED_PTR(soul::common, lex_context);
EFLIB_USING_SHARED_PTR(soul::common, diag_chat);
EFLIB_USING_SHARED_PTR(soul::common, code_source);

using eflib::fixed_string;

using boost::dynamic_pointer_cast;
using boost::shared_ptr;

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::ostream;
using std::ofstream;

BEGIN_NS_SOUL_DRIVERS();

template <typename ParserT>
bool compiler_impl::parse( ParserT& parser )
{
	try{
		opt_disp.reg_extra_parser( parser );
		opt_global.reg_extra_parser( parser );
		opt_io.reg_extra_parser( parser );
		opt_macros.reg_extra_parser( parser );
		opt_includes.reg_extra_parser(parser);

		po::parsed_options parsed
			= parser.run();

		std::vector<std::string> unrecg = po::collect_unrecognized( parsed.options, po::include_positional );

		if( !unrecg.empty() ){
			cout << "Warning: options ";
			BOOST_FOREACH( std::string const & str, unrecg ){
				cout << str << " ";
			}
			cout << "are invalid. They were ignored." << endl;
		}

		po::store( parsed, vm );
		po::notify(vm);

	} catch ( boost::program_options::invalid_command_line_syntax const & e ) {
		cout << "Fatal error occurs: " << e.what() << endl;
		return false;
	} catch ( std::exception const & e ){
		cout << "Unprocessed error: " << e.what() << endl;
	}

	return true;
}

void compiler_impl::set_parameter( int argc, char** argv )
{
	po::basic_command_line_parser<char> parser
		= po::command_line_parser(argc, argv).options( desc ).allow_unregistered();
	if( !parse(parser) )
	{
		EFLIB_ASSERT_UNIMPLEMENTED();
	}
}

void compiler_impl::set_parameter( std::string const& cmd )
{
#if defined(EFLIB_WINDOWS)
	vector<string> cmds = po::split_winmain(cmd);
#else
	vector<string> cmds = po::split_unix(cmd);
#endif

	po::basic_command_line_parser<char> parser
		= po::command_line_parser(cmds).options( desc ).allow_unregistered();

	if( !parse( parser ) ){
		EFLIB_ASSERT_UNIMPLEMENTED();
	}
}

compiler_impl::compiler_impl()
{
	opt_disp.fill_desc(desc);
	opt_global.fill_desc(desc);
	opt_io.fill_desc(desc);
	opt_macros.fill_desc(desc);
	opt_includes.fill_desc(desc);
}

shared_ptr<diag_chat> compiler_impl::compile()
{
	// Initialize env for compiling. 
	shared_ptr<diag_chat> diags = diag_chat::create();

	opt_disp.filterate(vm);
	opt_global.filterate(vm);
	opt_io.filterate(vm);
	opt_macros.filterate(vm);
	opt_includes.filterate(vm);

	if( opt_disp.show_help ){
		diags->report(text_only)->p(desc);
		return diags;
	}

	if( opt_disp.show_version ){
		diags->report(text_only)->p(opt_disp.version_info);
		return diags;
	}

	if( opt_global.detail == options_global::none ){
		diags->report(unknown_detail_level)->p(opt_global.detail_str);
	}

	// Process inputs and outputs.
	std::string fname = opt_io.input_file;
	shared_ptr<code_source> code_src;
	shared_ptr<lex_context> lex_ctxt;
	// Set code source.
	if( !fname.empty() )
	{
		shared_ptr<compiler_code_source> file_code_source( new compiler_code_source() );
		
		if ( !file_code_source->set_file(fname) ){
			diags->report( soul::parser::cannot_open_input_file )->p(fname);
			return diags;
		} 

		diags->report(compiling_input)->p(fname);
		code_src = file_code_source;
		lex_ctxt = file_code_source;
	}
	else if( user_code_src )
	{
		code_src = user_code_src;
		lex_ctxt = user_lex_ctxt;
	}
	else
	{
		diags->report(input_file_is_missing);
		return diags;
	}

	// Set include and virtual include.

	compiler_code_source* driver_sc = dynamic_cast<compiler_code_source*>( code_src.get() );

	if( driver_sc )
	{
		driver_sc->set_diag_chat( diags.get() );

		if( user_inc_handler )
		{
			driver_sc->set_include_handler( user_inc_handler );
		}
		else
		{
			for( virtual_file_dict::iterator it = virtual_files.begin(); it != virtual_files.end(); ++it)
			{
				driver_sc->add_virtual_file( it->first, it->second.first, it->second.second );
			}

			// Apply macros from API.
			for(size_t i = 0; i < macros.size(); ++i){
				macro_states ms = macros[i].second;
				switch (ms)
				{
				case ms_normal:
					driver_sc->add_macro( macros[i].first, false );
					break;
				case ms_predef:
					driver_sc->add_macro( macros[i].first, true );
					break;
				case ms_remove:
					driver_sc->remove_macro( macros[i].first );
					break;
				}
			}

			// Apply macros from parameters.
			driver_sc->add_macro( opt_macros.defines, false );
			driver_sc->add_macro( opt_macros.predefs, true );
			driver_sc->remove_macro( opt_macros.undefs );

			// Apply search path from API
			driver_sc->add_include_path( inc_paths );
			driver_sc->add_sys_include_path( sys_paths );

			// Apply search path from parameters.
			driver_sc->add_include_path( opt_includes.includes );
			driver_sc->add_sys_include_path( opt_includes.sysincls );
		}
	}

	eflib::profiler		prof;
	diag_chat_ptr		semantic_diags;

	{
		// Compiling with profiling

		eflib::profiling_scope prof_scope(&prof, "driver impl compiling");
		
		{
			eflib::profiling_scope prof_scope(&prof, "parse @ compiler_impl");
			mroot = soul::syntax_tree::parse( code_src.get(), lex_ctxt, diags.get() );
			if( !mroot ){ return diags; }
		}
	}

	eflib::print_profiler(&prof, 3);

	return diags;
}

node_ptr compiler_impl::get_root() const{
	return mroot;
}

po::variables_map const & compiler_impl::variables() const
{
	return vm;
}

options_display_info const & compiler_impl::display_info() const
{
	return opt_disp;
}

options_io const & compiler_impl::io_info() const
{
	return opt_io;
}

void compiler_impl::set_code( std::string const& code )
{
	shared_ptr<compiler_code_source> src( new compiler_code_source() );
	src->set_code( code );
	user_code_src = src;
	user_lex_ctxt = src;
}

void compiler_impl::set_code_source( shared_ptr<code_source> const& src )
{
	user_code_src = src;
}

void compiler_impl::set_code_file( std::string const& code_file )
{
	opt_io.input_file = code_file;
}

void compiler_impl::set_lex_context( shared_ptr<lex_context> const& lex_ctxt )
{
	user_lex_ctxt = lex_ctxt;
}

void compiler_impl::add_virtual_file( string const& file_name, string const& code_content, bool high_priority )
{
	virtual_files[file_name] = make_pair( code_content, high_priority );
}

void compiler_impl::set_include_handler( include_handler_fn inc_handler )
{
	user_inc_handler = inc_handler;
}

void compiler_impl::add_sysinclude_path( std::string const& sys_path )
{
	sys_paths.push_back(sys_path);
}

void compiler_impl::add_include_path( std::string const& inc_path )
{
	inc_paths.push_back(inc_path);
}

void compiler_impl::clear_sysinclude_paths()
{
	sys_paths.clear();
}

void compiler_impl::add_macro( std::string const& macro, bool predef )
{
	macros.push_back( make_pair(macro, predef?ms_predef:ms_normal) );
}

void compiler_impl::clear_macros()
{
	macros.clear();
}

void compiler_impl::remove_macro( std::string const& macro )
{
	macros.push_back( make_pair(macro, ms_remove) );
}


void null_compiler::set_parameter( int /*argc*/, char** /*argv*/ )
{

}

null_compiler::null_compiler()
{

}

END_NS_SOUL_DRIVERS();