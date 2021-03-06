#ifndef SOUL_COMMON_DIAG_FORMATTER_H
#define SOUL_COMMON_DIAG_FORMATTER_H

#include <soul/include/common/common_fwd.h>

#include <eflib/include/string/ustring.h>

BEGIN_NS_SOUL_COMMON();

class diag_item;

enum compiler_compatibility
{
	cc_msvc,
	cc_gcc
};

eflib::fixed_string str(diag_item const*, compiler_compatibility cc = cc_msvc);

END_NS_SOUL_COMMON();

#endif