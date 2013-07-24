#include <soul/include/syntax_tree/program.h>

#include <soul/enums/node_ids.h>
#include <soul/include/syntax_tree/visitor.h>

BEGIN_NS_SOUL_SYNTAX_TREE();

program::program( const std::string& name)
	: node( node_ids::program, boost::shared_ptr<token_t>(), boost::shared_ptr<token_t>() ), name(name)
{
}

program::program( boost::shared_ptr<token_t> const& tok_beg, boost::shared_ptr<token_t> const& tok_end )
	: node( node_ids::program, tok_beg, tok_end )
{
}

SOUL_SYNTAX_NODE_ACCEPT_METHOD_IMPL( program );

END_NS_SOUL_SYNTAX_TREE();