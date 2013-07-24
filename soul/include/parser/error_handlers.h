#ifndef SOUL_PARSER_ERROR_HANDLERS_H
#define SOUL_PARSER_ERROR_HANDLERS_H

#include <soul/include/parser/parser_forward.h>

#include <soul/include/parser/generator.h>

namespace soul
{
	namespace common
	{
		class diag_chat;
	}
}

BEGIN_NS_SOUL_PARSER();

typedef boost::shared_ptr< soul::common::token_t > token_ptr;
typedef std::vector< token_ptr > token_seq;
typedef token_seq::iterator token_iterator;

error_handler get_expected_failed_handler( std::string const& expected_str );
parse_results expected_failed_handler	( soul::common::diag_chat* diags, token_iterator const& org_iter, token_iterator& iter, std::string const& expected_str );

END_NS_SOUL_PARSER();

#endif