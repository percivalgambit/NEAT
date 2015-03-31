ftrace_replace_fp_ins_simple.test: REPLACE_FP_FN := replace_fp_ins_simple
ftrace_replace_fp_ins_simple.test: REPLACE_FP_FILE := replace_fp_ins_simple.cpp

ftrace_replace_fp_ins_complex.test: REPLACE_FP_FN := replace_fp_ins_complex
ftrace_replace_fp_ins_complex.test: REPLACE_FP_FILE := replace_fp_ins_complex.cpp

ftrace_no_replace.test: REPLACE_FP_FN := replace_fp_ins_simple
ftrace_no_replace.test: REPLACE_FP_FILE := replace_fp_ins_simple.cpp
ftrace_no_replace.test: FTRACE_CMDLINE_FLAGS := -fp-replace 0
