#ifndef SOUL_SYNTAX_TREE_PROGRAM_H
#define SOUL_SYNTAX_TREE_PROGRAM_H

#include <soul/include/syntax_tree/syntax_tree_fwd.h>
#include <soul/include/syntax_tree/node.h>
#include <eflib/include/utility/enable_if.h>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits.hpp>
#include <vector>

BEGIN_NS_SOUL_SYNTAX_TREE();

class syntax_tree_visitor;
struct declaration;
struct token_t;

struct program: public node{

	SOUL_SYNTAX_NODE_CREATORS();

	// help for creating program syntax tree
	SOUL_SYNTAX_NODE_ACCEPT_METHOD_DECL();
	std::string name;
	std::vector< boost::shared_ptr<declaration> > decls;

protected:
	program(const std::string& name);
	program( boost::shared_ptr<token_t> const&, boost::shared_ptr<token_t> const& );
	program& operator = (const program&);
	program( const program& );
};

END_NS_SOUL_SYNTAX_TREE();

#endif