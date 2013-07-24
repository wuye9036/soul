#include <soul/include/syntax_tree/declaration.h>
#include <soul/include/syntax_tree/expression.h>
#include <soul/include/syntax_tree/node_creation.h>
#include <soul/include/syntax_tree/statement.h>
#include <soul/include/syntax_tree/program.h>

BEGIN_NS_SOUL_SYNTAX_TREE();

using soul::common::token_t;

void instantiate(){
	create_node<program>( token_t::null(), token_t::null() );
	create_node<variable_declaration>( token_t::null(), token_t::null() );
	create_node<parameter_full>( token_t::null(), token_t::null() );
	create_node<expression_list>( token_t::null(), token_t::null() );
	create_node<labeled_statement>( token_t::null(), token_t::null() );
}

END_NS_SOUL_SYNTAX_TREE();