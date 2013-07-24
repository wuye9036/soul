#ifndef SOUL_SYNTAX_TREE_PARSE_API_H
#define SOUL_SYNTAX_TREE_PARSE_API_H

#include <soul/include/syntax_tree/syntax_tree_fwd.h>

#include <eflib/include/platform/boost_begin.h>
#include <boost/shared_ptr.hpp>
#include <eflib/include/platform/boost_end.h>

#include <string>

namespace soul{
	namespace common{
		class lex_context;
		class code_source;
		class diag_chat;
	}
}

BEGIN_NS_SOUL_SYNTAX_TREE();

struct program;

boost::shared_ptr<program> parse(
	const std::string& code_text,
	boost::shared_ptr<soul::common::lex_context> ctxt,
	soul::common::diag_chat* diags
	);

boost::shared_ptr<program> parse(
	soul::common::code_source* src,
	boost::shared_ptr<soul::common::lex_context> ctxt,
	soul::common::diag_chat* diags
	);

END_NS_SOUL_SYNTAX_TREE();

#endif