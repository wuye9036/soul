#include <soul/include/parser/error_handlers.h>
#include <soul/include/parser/diags.h>
#include <soul/include/parser/generator.h>
#include <soul/include/common/diag_chat.h>
#include <boost/bind.hpp>

using soul::common::diag_chat;
using boost::bind;

BEGIN_NS_SOUL_PARSER();

error_handler get_expected_failed_handler( std::string const& expected_str )
{
	return boost::bind( expected_failed_handler, _1, _2, _3, expected_str );
}

parse_results expected_failed_handler( diag_chat* diags, token_iterator const& org_iter, token_iterator& iter, std::string const& expected_str )
{
	token_ptr tok = *iter;
	
	diags->clear();
	if( tok->end_of_file )
	{
		diags->report( end_of_file )
			->span(tok->span)->file(tok->file_name)
			->p(expected_str);
	} 
	else 
	{
		diags->report( unmatched_expected_token )
			->span(tok->span)->file(tok->file_name)
			->p(expected_str)->p(tok->str);
	}
	

	iter = org_iter;

	return parse_results::recovered_expected_failed;
}

END_NS_SOUL_PARSER();