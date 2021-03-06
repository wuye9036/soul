
#ifndef SASL_ENUMS_BUILTIN_TYPES_H
#define SASL_ENUMS_BUILTIN_TYPES_H

#include "../enums/enum_base.h" 

struct builtin_types :
	public enum_base< builtin_types, uint32_t >
	, public bitwise_op< builtin_types >, public equal_op< builtin_types >, public value_op< builtin_types, uint32_t >
{
	friend struct enum_hasher;
private:
	builtin_types( const storage_type& val, const std::string& name );
	builtin_types( const storage_type& val ): base_type(val){}
public:
	static void force_initialize();
	
	builtin_types( const this_type& rhs )
		:base_type(rhs.val_)
	{}
	
	this_type& operator = ( const this_type& rhs){
		val_ = rhs.val_;
		return *this;
	}

	const static this_type _unsigned;
	const static this_type _sint32;
	const static this_type _c_int;
	const static this_type _sint16;
	const static this_type _generic_type_field_shift;
	const static this_type _scalar_type_mask;
	const static this_type _sign_mask;
	const static this_type _dim1_mask;
	const static this_type _boolean;
	const static this_type _generic_type_mask;
	const static this_type _sint8;
	const static this_type _scalar;
	const static this_type _sign_field_shift;
	const static this_type _sampler;
	const static this_type _float;
	const static this_type _dim0_field_shift;
	const static this_type _void;
	const static this_type _uint16;
	const static this_type _dimension_mask;
	const static this_type _dim1_field_shift;
	const static this_type _dimension_field_shift;
	const static this_type _double;
	const static this_type _matrix;
	const static this_type _sint64;
	const static this_type _real;
	const static this_type _scalar_field_shift;
	const static this_type _uint8;
	const static this_type _signed;
	const static this_type _vector;
	const static this_type none;
	const static this_type _uint32;
	const static this_type _precision_field_shift;
	const static this_type _uint64;
	const static this_type _dim0_mask;
	const static this_type _integer;


	static std::string to_name( const this_type& enum_val );
	static this_type from_name( const std::string& name );
	std::string name() const;

};

#endif
