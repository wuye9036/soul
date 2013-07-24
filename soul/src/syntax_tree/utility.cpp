#include <soul/include/syntax_tree/utility.h>

#include <soul/include/syntax_tree/declaration.h>
#include <soul/include/syntax_tree/expression.h>
#include <soul/include/syntax_tree/node_creation.h>
#include <soul/include/syntax_tree/program.h>
#include <soul/include/syntax_tree/statement.h>
#include <soul/include/syntax_tree/visitor.h>

#include <eflib/include/diagnostics/assert.h>
#include <eflib/include/utility/enable_if.h>

#include <eflib/include/platform/boost_begin.h>
#include <boost/foreach.hpp>
#include <boost/preprocessor.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <eflib/include/platform/boost_end.h>

#include <vector>

using std::vector;

using boost::any;
using boost::any_cast;
using boost::is_base_of;
using boost::dynamic_pointer_cast;
using boost::shared_ptr;

BEGIN_NS_SOUL_SYNTAX_TREE();

#define SAFE_ACCEPT( node_handle ) if( node_handle ) { (node_handle)->accept(this, data); }
class follow_up_visitor : public syntax_tree_visitor{
public:
	follow_up_visitor( boost::function<void( node&, ::boost::any* )> applied ): applied( applied ){}

	// expression
	SOUL_VISIT_DCL( unary_expression ) {
		SAFE_ACCEPT( v.expr );
		applied( v, data );
	}
	SOUL_VISIT_DCL( cast_expression ) {
		SAFE_ACCEPT( v.casted_type );
		SAFE_ACCEPT( v.expr );
		applied( v, data );
	}
	SOUL_VISIT_DCL( binary_expression ){
		SAFE_ACCEPT( v.left_expr );
		SAFE_ACCEPT( v.right_expr );
		applied( v, data );
	}
	SOUL_VISIT_DCL( expression_list ) {
		visit( v.exprs, data );
		applied( v, data );
	}
	SOUL_VISIT_DCL( cond_expression ) {
		SAFE_ACCEPT( v.cond_expr );
		SAFE_ACCEPT( v.yes_expr );
		SAFE_ACCEPT( v.no_expr );
		applied( v, data );
	}
	SOUL_VISIT_DCL( index_expression ) {
		SAFE_ACCEPT( v.expr );
		SAFE_ACCEPT( v.index_expr );
		applied( v, data );
	}
	SOUL_VISIT_DCL( call_expression ){
		SAFE_ACCEPT( v.expr );
		visit( v.args, data );
		applied( v, data );
	}
	SOUL_VISIT_DCL( member_expression ) {
		SAFE_ACCEPT( v.expr );
		applied( v, data );
	}
	SOUL_VISIT_DCL( constant_expression ) {
		applied( v, data );
	}
	SOUL_VISIT_DCL( variable_expression ) {
		applied( v, data );
	}

	// declaration & type specifier
	SOUL_VISIT_INLINE_DEF_UNIMPL( initializer );

	SOUL_VISIT_DCL( expression_initializer ) {
		SAFE_ACCEPT( v.init_expr );
		applied( v, data );
	}
	SOUL_VISIT_DCL( member_initializer ) {
		visit( v.sub_inits, data );
		applied( v, data );
	}
	SOUL_VISIT_INLINE_DEF_UNIMPL( declaration );
	SOUL_VISIT_DCL( variable_declaration ){
		SAFE_ACCEPT( v.type_info );
		visit( v.declarators, data );
		applied( v, data );
	}
	SOUL_VISIT_DCL( declarator ){
		SAFE_ACCEPT( v.init );
		applied( v, data );
	}
	SOUL_VISIT_DCL( type_definition ){
		SAFE_ACCEPT( v.type_info );
		applied( v, data );
	}
	SOUL_VISIT_INLINE_DEF_UNIMPL( tynode );
	SOUL_VISIT_DCL( builtin_type ){
		applied( v, data );
	}
	SOUL_VISIT_DCL( array_type ){
		SAFE_ACCEPT( v.elem_type );
		visit( v.array_lens, data );
		applied( v, data );
	}
	SOUL_VISIT_DCL( struct_type ){
		visit( v.decls, data );
		applied( v, data );
	}
	SOUL_VISIT_DCL( alias_type ){
		visit( v, data );
	}
	SOUL_VISIT_DCL(function_type) {
		SAFE_ACCEPT(v.result_type);
		visit(v.param_types, data);
		applied( v, data );
	}
	SOUL_VISIT_DCL(parameter){
		SAFE_ACCEPT(v.init);
		applied(v, data);
	}
	SOUL_VISIT_DCL( function_def ) {
		SAFE_ACCEPT(v.type);
		visit(v.params, data);
		SAFE_ACCEPT(v.body);
		applied(v, data);
	}
	SOUL_VISIT_DCL( parameter_full ){
		SAFE_ACCEPT( v.init );
		SAFE_ACCEPT( v.param_type );
		applied( v, data );
	}
	SOUL_VISIT_DCL( function_full_def ) {
		SAFE_ACCEPT( v.retval_type );
		visit( v.params, data );
		SAFE_ACCEPT( v.body );
		applied( v, data );
	}

	// statement
	SOUL_VISIT_INLINE_DEF_UNIMPL( statement );

	SOUL_VISIT_DCL( declaration_statement ){
		visit(v.decls, data);
		applied( v, data );
	}
	SOUL_VISIT_DCL( if_statement ){
		SAFE_ACCEPT( v.cond );
		SAFE_ACCEPT( v.yes_stmt );
		SAFE_ACCEPT( v.no_stmt );
		applied( v, data );
	}
	SOUL_VISIT_DCL( while_statement ){
		SAFE_ACCEPT( v.cond );
		SAFE_ACCEPT( v.body );
		applied( v, data );
	}
	SOUL_VISIT_DCL( dowhile_statement ){
		SAFE_ACCEPT( v.body );
		SAFE_ACCEPT( v.cond );
		applied( v, data );
	}
	SOUL_VISIT_DCL( for_statement ){
		SAFE_ACCEPT( v.init );
		SAFE_ACCEPT( v.cond );
		SAFE_ACCEPT( v.iter );
		SAFE_ACCEPT( v.body );
		applied( v, data );
	}
	SOUL_VISIT_DCL( case_label ){
		SAFE_ACCEPT( v.expr );
		applied( v, data );
	}
	SOUL_VISIT_DCL( ident_label ){
		applied( v, data );
	}
	SOUL_VISIT_DCL( switch_statement ){
		SAFE_ACCEPT( v.cond );
		SAFE_ACCEPT( v.stmts );
		applied( v, data );
	}
	SOUL_VISIT_DCL( compound_statement ){
		visit( v.stmts, data );
		applied( v, data );
	}
	SOUL_VISIT_DCL( expression_statement ){
		SAFE_ACCEPT( v.expr );
		applied( v, data );
	}
	SOUL_VISIT_DCL( jump_statement ){
		SAFE_ACCEPT( v.jump_expr );
		applied( v, data );
	}
	SOUL_VISIT_DCL( labeled_statement ){
		SAFE_ACCEPT( v.stmt );
		visit( v.labels, data );
	}
	// program
	SOUL_VISIT_DCL( program ){
		visit( v.decls, data );
		applied( v, data );
	}

private:
	template <typename ContainerT>
	void visit( ContainerT& v, ::boost::any* data ){
		for( typename ContainerT::iterator it = v.begin(); it != v.end(); ++it ){
			SAFE_ACCEPT( *it );
		}
	}
	boost::function<void( node&, ::boost::any* )> applied;
};

void follow_up_traversal( boost::shared_ptr<node> root, boost::function<void( node&, ::boost::any* )> on_visit ){
	follow_up_visitor fuv( on_visit );
	if( root ){
		root->accept( &fuv, NULL );
	}
}

boost::shared_ptr<builtin_type> create_builtin_type( const builtin_types& btc )
{
	boost::shared_ptr<builtin_type> ret = create_node<builtin_type>( token_t::null(), token_t::null() );
	ret->tycode = btc;
	return ret;
}

template< typename NodeT >
void store_node_to_data( any* lhs, shared_ptr< NodeT > rhs ){
	if( rhs ){
		*lhs = dynamic_pointer_cast<node>( rhs );
	} else {
		*lhs = shared_ptr<node>();
	}
}

#define COPY_VALUE_ITEM( r, dest_src, member ) \
	BOOST_PP_TUPLE_ELEM(2, 0, dest_src)->member = BOOST_PP_TUPLE_ELEM(2, 1, dest_src).member;

#define SOUL_SWALLOW_CLONE_NODE( output, v, node_type, member_seq ) \
	::boost::shared_ptr< node_type > cloned	= create_node< node_type >( v.token_begin(), v.token_end() ); \
	BOOST_PP_SEQ_FOR_EACH( COPY_VALUE_ITEM, (cloned, v), member_seq ); \
	store_node_to_data( (output), cloned );

template<typename T> void copy_from_any( T& lhs, const boost::any& rhs ){
	lhs = any_cast<T>(rhs);
}

template<typename NodeT> void copy_from_any( shared_ptr<NodeT>& lhs, const any& rhs, EFLIB_ENABLE_IF_PRED2(is_base_of, node, NodeT) ){
	shared_ptr<node> any_v = any_cast< shared_ptr<node> >( rhs );
	if ( any_v ){
		lhs = dynamic_pointer_cast<NodeT>(any_v);
	} else {
		lhs.reset();
	}
}

#define DEEPCOPY_VALUE_ITEM( r, dest_src, member )	\
	visit( BOOST_PP_TUPLE_ELEM(2, 1, dest_src).member, &member_dup );	\
	copy_from_any( BOOST_PP_TUPLE_ELEM(2, 0, dest_src)->member, member_dup );

#define SOUL_DEEP_CLONE_NODE( dest_any_ptr, src_v_ref, node_type, member_seq )	\
	::boost::shared_ptr< node_type > cloned	= create_node< node_type >( src_v_ref.token_begin(), src_v_ref.token_end() ); \
	boost::any member_dup; \
	BOOST_PP_SEQ_FOR_EACH( DEEPCOPY_VALUE_ITEM, (cloned, src_v_ref), member_seq ); \
	store_node_to_data( (dest_any_ptr), cloned );

#define SOUL_CLONE_NODE_FUNCTION_DEF( clone_mode, node_type, member_seq )	\
	SOUL_VISIT_DCL( node_type ){	\
		EFLIB_ASSERT( data, "Data parameter must not be NULL, it is used to feedback cloned node." );	\
		SOUL_##clone_mode##_CLONE_NODE( data, v, node_type, member_seq );	\
	}

class swallow_duplicator: public syntax_tree_visitor{
public:
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, unary_expression, (op)(expr) );

	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, cast_expression, (casted_type)(expr) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, binary_expression, (op)(op_token)(left_expr)(right_expr) );
	SOUL_VISIT_INLINE_DEF_UNIMPL( expression_list );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, cond_expression, (cond_expr)(yes_expr)(no_expr) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, index_expression, (expr)(index_expr) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, call_expression, (expr)(args) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, member_expression, (expr)(member) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, constant_expression, (value_tok)(ctype) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, variable_expression, (var_name) );

	// declaration & type specifier
	SOUL_VISIT_INLINE_DEF_UNIMPL( initializer );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, expression_initializer, (init_expr) );
	SOUL_VISIT_INLINE_DEF_UNIMPL( member_initializer );
	SOUL_VISIT_INLINE_DEF_UNIMPL( declaration );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, variable_declaration,(type_info)(declarators) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, parameter_full,		(name)(param_type)(init) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, declarator,			(name)(init)(semantic)(semantic_index) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, function_def,		(name)(type)(params)(body) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, parameter,			(name)(init) );
	SOUL_VISIT_INLINE_DEF_UNIMPL( type_definition );
	SOUL_VISIT_INLINE_DEF_UNIMPL( tynode );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, builtin_type,		(tycode)(qual) (tok_beg)(tok_end) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, array_type,			(tycode)(qual) (elem_type)(array_lens) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, struct_type,			(tycode)(qual) (name)(decls) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, alias_type,			(tycode)(qual) (alias) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, function_type,		(tycode)(qual) (param_types)(result_type) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, function_full_def,	(tycode)(qual) (name)(retval_type)(params)(body) );
	// statement
	SOUL_VISIT_INLINE_DEF_UNIMPL( statement );

	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, declaration_statement,	(decls) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, if_statement,			(cond)(yes_stmt)(no_stmt) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, while_statement,			(cond)(body) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, dowhile_statement,		(cond)(body) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, for_statement,			(init)(cond)(iter)(body) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, switch_statement,		(cond)(stmts) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, compound_statement,		(stmts) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, expression_statement,	(expr) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, jump_statement,			(code)(jump_expr) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, labeled_statement,		(stmt)(labels) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, case_label,				(expr) );
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, ident_label,				(label_tok) );

	// program
	SOUL_CLONE_NODE_FUNCTION_DEF( SWALLOW, program, (decls)(name) );
};

class deep_duplicator: public syntax_tree_visitor{
public:
	SOUL_CLONE_NODE_FUNCTION_DEF( DEEP, unary_expression, (op)(expr)(tok_beg)(tok_end) );

	SOUL_VISIT_INLINE_DEF_UNIMPL( cast_expression );
	SOUL_VISIT_INLINE_DEF_UNIMPL( binary_expression );
	SOUL_VISIT_INLINE_DEF_UNIMPL( expression_list );
	SOUL_VISIT_INLINE_DEF_UNIMPL( cond_expression );
	SOUL_VISIT_INLINE_DEF_UNIMPL( index_expression );
	SOUL_VISIT_INLINE_DEF_UNIMPL( call_expression );
	SOUL_VISIT_INLINE_DEF_UNIMPL( member_expression );
	SOUL_VISIT_INLINE_DEF_UNIMPL( constant_expression );
	SOUL_VISIT_INLINE_DEF_UNIMPL( variable_expression );

	// declaration & type specifier
	SOUL_VISIT_INLINE_DEF_UNIMPL( initializer );
	SOUL_VISIT_INLINE_DEF_UNIMPL( expression_initializer );
	SOUL_VISIT_INLINE_DEF_UNIMPL( member_initializer );
	SOUL_VISIT_INLINE_DEF_UNIMPL( declaration );
	SOUL_VISIT_INLINE_DEF_UNIMPL( variable_declaration );
	SOUL_VISIT_INLINE_DEF_UNIMPL( declarator );
	SOUL_VISIT_INLINE_DEF_UNIMPL( type_definition );
	SOUL_VISIT_INLINE_DEF_UNIMPL( tynode );

	SOUL_CLONE_NODE_FUNCTION_DEF( DEEP, builtin_type, (tycode)(qual)(tok_beg)(tok_end) );

	SOUL_VISIT_INLINE_DEF_UNIMPL( array_type );
	SOUL_VISIT_INLINE_DEF_UNIMPL( struct_type );
	SOUL_VISIT_INLINE_DEF_UNIMPL( alias_type );
	SOUL_VISIT_INLINE_DEF_UNIMPL( parameter_full );
	SOUL_VISIT_INLINE_DEF_UNIMPL( function_full_def );
	SOUL_VISIT_INLINE_DEF_UNIMPL( parameter );
	SOUL_VISIT_INLINE_DEF_UNIMPL( function_def );
	SOUL_VISIT_INLINE_DEF_UNIMPL( function_type );
	// statement
	SOUL_VISIT_INLINE_DEF_UNIMPL( statement );

	SOUL_VISIT_INLINE_DEF_UNIMPL( declaration_statement );
	SOUL_VISIT_INLINE_DEF_UNIMPL( if_statement );
	SOUL_VISIT_INLINE_DEF_UNIMPL( while_statement );
	SOUL_VISIT_INLINE_DEF_UNIMPL( dowhile_statement );
	SOUL_VISIT_INLINE_DEF_UNIMPL( for_statement );
	SOUL_VISIT_INLINE_DEF_UNIMPL( case_label );
	SOUL_VISIT_INLINE_DEF_UNIMPL( ident_label );
	SOUL_VISIT_INLINE_DEF_UNIMPL( switch_statement );
	SOUL_VISIT_INLINE_DEF_UNIMPL( compound_statement );
	SOUL_VISIT_INLINE_DEF_UNIMPL( expression_statement );
	SOUL_VISIT_INLINE_DEF_UNIMPL( jump_statement );
	SOUL_VISIT_INLINE_DEF_UNIMPL( labeled_statement );

	// program
	SOUL_VISIT_INLINE_DEF_UNIMPL( program );

	// If value is "value semantic", copy it as raw data.
	template <typename ValueT> void visit( ValueT& v, boost::any* data ){
		*data = v;
	}

	// If value is "value semantic", copy it as raw data.
	template <typename NodeT> void visit( vector< shared_ptr<NodeT> > & v, boost::any* data ){
		vector< shared_ptr<NodeT> > out_v(v.size());
		BOOST_FOREACH( shared_ptr<NodeT> item, v ){
			boost::any cloned;
			visit( item, &cloned );
			out_v.push_back( dynamic_pointer_cast<NodeT>( boost::any_cast< shared_ptr<node> > (cloned) ) );
		}
		*data = out_v;
	}
};

template <typename ValueT>
ValueT process_node( ::boost::shared_ptr<node> src, syntax_tree_visitor* v ){
	EFLIB_ASSERT_AND_IF( src && v, "The input parameter is unavaliable!" ){
		return src;
	}

	::boost::any result_val;
	src->accept( v, &result_val );
	return ::boost::any_cast< ValueT >(result_val);
}

boost::shared_ptr<node> duplicate( ::boost::shared_ptr<node> src ){
	swallow_duplicator dup;
	return process_node< ::boost::shared_ptr<node> >( src, &dup );
}

boost::shared_ptr<node> deep_duplicate( ::boost::shared_ptr<node> src ){
	deep_duplicator dup;
	return process_node< ::boost::shared_ptr<node> >( src, &dup );
}

END_NS_SOUL_SYNTAX_TREE();
