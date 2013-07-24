#pragma once

#ifndef SOUL_DRIVERS_COMPILER_DIAGS_H
#define SOUL_DRIVERS_COMPILER_DIAGS_H

#include <soul/include/drivers/drivers_forward.h>

#include <soul/include/common/diag_item.h>

BEGIN_NS_SOUL_DRIVERS();
extern soul::common::diag_template text_only;
extern soul::common::diag_template unknown_detail_level;
extern soul::common::diag_template input_file_is_missing;
extern soul::common::diag_template unknown_lang;
extern soul::common::diag_template compiling_input;
END_NS_SOUL_DRIVERS();

#endif