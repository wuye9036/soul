#include <soul/include/syntax_tree/identifier.h>
#include <soul/include/common/token.h>
#include <soul/include/syntax_tree/visitor.h>
BEGIN_NS_SOUL_SYNTAX_TREE();

using namespace boost;

identifier::identifier( boost::shared_ptr<token_t> const& tok )
	: node( node_ids::identifier, tok, tok ), name(tok->str){}

END_NS_SOUL_SYNTAX_TREE();