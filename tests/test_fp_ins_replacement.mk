ftrace_replace_fp_ins_simple.test: $(OBJDIR)sse_sample_app$(EXE_SUFFIX)
	$(MAKE) REPLACE_FP_FN=replace_fp_ins_simple $(OBJDIR)ftrace$(PINTOOL_SUFFIX)
	$(PIN) -t $(OBJDIR)ftrace$(PINTOOL_SUFFIX) -o $(OBJDIR)ftrace_replace_fp_ins_simple.out -- $(OBJDIR)sse_sample_app$(EXE_SUFFIX) > $(OBJDIR)ftrace_replace_fp_ins_simple_stdout.out
	$(DIFF) -I "Total number of instructions: [0-9]*" $(OBJDIR)ftrace_replace_fp_ins_simple.out ftrace_replace_fp_ins_simple.reference
	$(DIFF) $(OBJDIR)ftrace_replace_fp_ins_simple_stdout.out ftrace_replace_fp_ins_simple_stdout.reference
	$(RM) $(OBJDIR)ftrace_replace_fp_ins_simple.out $(OBJDIR)ftrace_replace_fp_ins_simple_stdout.out $(OBJDIR)ftrace$(PINTOOL_SUFFIX) $(OBJDIR)ftrace$(OBJ_SUFFIX)

ftrace_replace_fp_ins_complex.test: $(OBJDIR)sse_sample_app$(EXE_SUFFIX)
	$(MAKE) REPLACE_FP_FN=replace_fp_ins_complex REPLACE_FP_FILE=replace_fp_ins_complex.cpp $(OBJDIR)ftrace$(PINTOOL_SUFFIX)
	$(PIN) -t $(OBJDIR)ftrace$(PINTOOL_SUFFIX) -o $(OBJDIR)ftrace_replace_fp_ins_complex.out -- $(OBJDIR)sse_sample_app$(EXE_SUFFIX) > $(OBJDIR)ftrace_replace_fp_ins_complex_stdout.out
	$(DIFF) -I "Total number of instructions: [0-9]*" $(OBJDIR)ftrace_replace_fp_ins_complex.out ftrace_replace_fp_ins_complex.reference
	$(DIFF) $(OBJDIR)ftrace_replace_fp_ins_complex_stdout.out ftrace_replace_fp_ins_complex_stdout.reference
	$(RM) $(OBJDIR)ftrace_replace_fp_ins_complex.out $(OBJDIR)ftrace$(PINTOOL_SUFFIX) $(OBJDIR)ftrace$(OBJ_SUFFIX) $(OBJDIR)ftrace_replace_fp_ins_complex_stdout.out

ftrace_no_replace.test: $(OBJDIR)sse_sample_app$(EXE_SUFFIX)
	$(MAKE) REPLACE_FP_FN=replace_fp_ins_simple $(OBJDIR)ftrace$(PINTOOL_SUFFIX)
	$(PIN) -t $(OBJDIR)ftrace$(PINTOOL_SUFFIX) -fp-replace 0 -o $(OBJDIR)ftrace_no_replace.out -- $(OBJDIR)sse_sample_app$(EXE_SUFFIX) > $(OBJDIR)ftrace_no_replace_stdout.out
	$(DIFF) -I "Total number of instructions: [0-9]*" $(OBJDIR)ftrace_no_replace.out ftrace.reference
	$(DIFF) $(OBJDIR)ftrace_no_replace_stdout.out ftrace_stdout.reference
	$(RM) $(OBJDIR)ftrace_no_replace.out $(OBJDIR)ftrace_no_replace_stdout.out $(OBJDIR)ftrace$(PINTOOL_SUFFIX) $(OBJDIR)ftrace$(OBJ_SUFFIX)
