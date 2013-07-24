#ifndef SOUL_PARSER_DIAGS_H
#define SOUL_PARSER_DIAGS_H

#include <soul/include/parser/parser_forward.h>
#include <soul/include/common/diag_item.h>

BEGIN_NS_SOUL_PARSER();

// Boost.Wave Errors
extern soul::common::diag_template boost_wave_exception_warning;
extern soul::common::diag_template boost_wave_exception_error;
extern soul::common::diag_template boost_wave_exception_fatal_error;

extern soul::common::diag_template cannot_open_include_file;
extern soul::common::diag_template cannot_open_input_file;
extern soul::common::diag_template unrecognized_token;
extern soul::common::diag_template unknown_tokenize_error;
extern soul::common::diag_template end_of_file;
extern soul::common::diag_template unmatched_token;
extern soul::common::diag_template unmatched_expected_token;

END_NS_SOUL_PARSER();

#endif