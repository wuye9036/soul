#ifndef SOUL_SYNTAX_TREE_IDENTIFIER_H
#define SOUL_SYNTAX_TREE_IDENTIFIER_H

#include <soul/include/syntax_tree/syntax_tree_fwd.h>
#include <soul/include/syntax_tree/node.h>
#include <string>

namespace soul{ namespace common{
	struct token_t;
} }

BEGIN_NS_SOUL_SYNTAX_TREE();

using soul::common::token_t;

struct identifier: public node{
	identifier( boost::shared_ptr<token_t> const& tok );
	std::string name;
};

END_NS_SOUL_SYNTAX_TREE();

#endif