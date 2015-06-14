##############################################################
#
# Test targets
#
##############################################################

###### Place all generic definitions here ######

# This defines tests which run tools of the same name.  This is simply for convenience to avoid
# defining the test name twice (once in TOOL_ROOTS and again in TEST_ROOTS).
# Tests defined here should not be defined in TOOL_ROOTS and TEST_ROOTS.
TEST_TOOL_ROOTS := ftrace

# This defines the tests to be run that were not already defined in TEST_TOOL_ROOTS.
TEST_ROOTS := ftrace_normal_fp_implementation ftrace_no_specified_output_file ftrace_no_instrument ftrace_replace_fp_ins_simple ftrace_replace_fp_ins_complex ftrace_replacement_type_simple ftrace_replacement_type_nested

# This defines a list of tests that should run in the "short" sanity. Tests in this list must also
# appear either in the TEST_TOOL_ROOTS or the TEST_ROOTS list.
# If the entire directory should be tested in sanity, assign TEST_TOOL_ROOTS and TEST_ROOTS to the
# SANITY_SUBSET variable in the tests section below (see example in makefile.rules.tmpl).
SANITY_SUBSET :=

# This defines the tools which will be run during the the tests, and were not already defined in
# TEST_TOOL_ROOTS.
TOOL_ROOTS :=

# This defines the static analysis tools which will be run during the the tests. They should not
# be defined in TEST_TOOL_ROOTS. If a test with the same name exists, it should be defined in
# TEST_ROOTS.
# Note: Static analysis tools are in fact executables linked with the Pin Static Analysis Library.
# This library provides a subset of the Pin APIs which allows the tool to perform static analysis
# of an application or dll. Pin itself is not used when this tool runs.
SA_TOOL_ROOTS :=

# This defines all the applications that will be run during the tests.
APP_ROOTS := sse_sample_app

# This defines any additional object files that need to be compiled.
OBJECT_ROOTS :=

# This defines any additional dlls (shared objects), other than the pintools, that need to be compiled.
DLL_ROOTS :=

# This defines any static libraries (archives), that need to be built.
LIB_ROOTS :=

# default test rule
%.test: REPLACE_FP_FN       := fp_replacement_callback
%.test: REPLACE_FP_FILE     := tests/test_callbacks.cpp
%.test: START_CALLBACK      := start_callback
%.test: START_CALLBACK_FILE := tests/test_callbacks.cpp
%.test: EXIT_CALLBACK       := exit_callback
%.test: EXIT_CALLBACK_FILE  := tests/test_callbacks.cpp
%.test: EXPECTED_OUTPUT     ?= tests/$(@:.test=.reference)
%.test: EXPECTED_STDOUT     ?= tests/$(@:.test=.stdout.reference)
%.test: $(OBJDIR) $(OBJDIR)sse_sample_app$(EXE_SUFFIX)
	$(MAKE) $(FTRACE_CMD_VARS) $(OBJDIR)ftrace$(PINTOOL_SUFFIX)
	$(PIN) -t $(OBJDIR)ftrace$(PINTOOL_SUFFIX) $(FTRACE_CMDLINE_FLAGS) -o $(OBJDIR)$(@:.test=.out) -- $(OBJDIR)sse_sample_app$(EXE_SUFFIX) > $(OBJDIR)$(@:.test=.stdout.out)
	$(DIFF) -I "Total number of instructions: [0-9]*" $(OBJDIR)$(@:.test=.out) $(EXPECTED_OUTPUT)
	$(DIFF) $(OBJDIR)$(@:.test=.stdout.out) $(EXPECTED_STDOUT)
	-$(RM) $(OBJDIR)$(@:.test=.out) $(OBJDIR)$(@:.test=.stdout.out)
	-$(RM) $(OBJDIR)function_level_replacement_type_enum.h $(OBJDIR)function_level_replacement_type_mapping.h

# specific tests for different aspects of the tool
ftrace_normal_fp_implementation.test: REPLACE_FP_FN      :=
ftrace_normal_fp_implementation.test: REPLACE_FP_FILE    :=
ftrace_normal_fp_implementation.test: EXIT_CALLBACK      :=
ftrace_normal_fp_implementation.test: EXIT_CALLBACK_FILE :=
ftrace_normal_fp_implementation.test: EXPECTED_OUTPUT    := tests/ftrace_no_output.reference
ftrace_normal_fp_implementation.test: EXPECTED_STDOUT    := tests/ftrace.stdout.reference

ftrace_no_specified_output_file.test: $(OBJDIR) $(OBJDIR)sse_sample_app$(EXE_SUFFIX)
	$(MAKE) $(FTRACE_CMD_VARS) $(OBJDIR)ftrace$(PINTOOL_SUFFIX)
	$(PIN) -t $(OBJDIR)ftrace$(PINTOOL_SUFFIX) -- $(OBJDIR)sse_sample_app$(EXE_SUFFIX) &> $(OBJDIR)$(@:.test=.out)
	$(DIFF) $(OBJDIR)$(@:.test=.out) tests/ftrace.stdout.reference
	$(RM) $(OBJDIR)$(@:.test=.out)

ftrace_no_instrument.test: FTRACE_CMDLINE_FLAGS := -instrument 0
ftrace_no_instrument.test: EXPECTED_OUTPUT      := tests/ftrace_no_output.reference
ftrace_no_instrument.test: EXPECTED_STDOUT      := tests/ftrace.stdout.reference
ftrace_no_instrument.test: $(OBJDIR)ftrace_no_instrument.out

$(OBJDIR)ftrace_no_instrument.out: $(OBJDIR)
	touch $@

ftrace_replace_fp_ins_simple.test: REPLACE_FP_FN   := replace_fp_ins_simple
ftrace_replace_fp_ins_simple.test: REPLACE_FP_FILE := replace_fp_ins_simple.cpp

ftrace_replace_fp_ins_complex.test: REPLACE_FP_FN   := replace_fp_ins_complex
ftrace_replace_fp_ins_complex.test: REPLACE_FP_FILE := replace_fp_ins_complex.cpp

ftrace_replacement_type_simple.test: REPLACE_FP_FN := replace_fp_ins_different_types
ftrace_replacement_type_simple.test: REPLACE_FP_FILE := replace_fp_ins_different_types.cpp
ftrace_replacement_type_simple.test: FUNCTION_LEVEL_REPLACEMENT_TYPE_MAPPING_FILE := $(OBJDIR)simple_replacement_type_mapping.txt
ftrace_replacement_type_simple.test: EXTRA_SOURCE_FILES := replace_fp_ins_complex.cpp replace_fp_ins_simple.cpp
ftrace_replacement_type_simple.test: $(OBJDIR)simple_replacement_type_mapping.txt
ftrace_replacement_type_simple.test: $(OBJDIR)function_level_replacement_type_enum.h $(OBJDIR)function_level_replacement_type_mapping.h

$(OBJDIR)simple_replacement_type_mapping.txt: $(OBJDIR)
	@printf "replacement_simple helper2\n" > $@
	@printf "replacement_complex     \t  helper1\n" >> $@
	@printf "foo   bar\n" >> $@

ftrace_replacement_type_nested.test: REPLACE_FP_FN := replace_fp_ins_different_types
ftrace_replacement_type_nested.test: REPLACE_FP_FILE := replace_fp_ins_different_types.cpp
ftrace_replacement_type_nested.test: FUNCTION_LEVEL_REPLACEMENT_TYPE_MAPPING_FILE := $(OBJDIR)nested_replacement_type_mapping.txt
ftrace_replacement_type_nested.test: EXTRA_SOURCE_FILES := replace_fp_ins_complex.cpp replace_fp_ins_simple.cpp
ftrace_replacement_type_nested.test: $(OBJDIR)nested_replacement_type_mapping.txt
ftrace_replacement_type_nested.test: $(OBJDIR)function_level_replacement_type_enum.h $(OBJDIR)function_level_replacement_type_mapping.h

$(OBJDIR)nested_replacement_type_mapping.txt: $(OBJDIR)
	@printf "replacement_simple helper1, helper2\n" > $@
	@printf "replacement_complex nested_helper,foo,bar, \t  baz" >> $@
