#ifndef SOUL_PARSER_PARSE_API_H
#define SOUL_PARSER_PARSE_API_H

#include <soul/include/parser/parser_forward.h>

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
	namespace parser{
		class attribute;
		class lexer;
		class grammars;

		bool parse(
			boost::shared_ptr<attribute>& pt_root,
			const std::string& code,
			boost::shared_ptr< ::soul::common::lex_context > ctxt,
			lexer& l, grammars& g,
			soul::common::diag_chat* diags
			);

		bool parse(
			boost::shared_ptr<attribute>& pt_root,
			soul::common::code_source* src,
			boost::shared_ptr< ::soul::common::lex_context > ctxt,
			lexer& l, grammars& g,
			soul::common::diag_chat* diags
			);
	}
}

#endif