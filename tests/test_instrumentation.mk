ftrace.test: $(OBJDIR)ftrace$(PINTOOL_SUFFIX) $(OBJDIR)sse_sample_app$(EXE_SUFFIX)
	$(PIN) -t $(OBJDIR)ftrace$(PINTOOL_SUFFIX) -o $(OBJDIR)ftrace.out -- $(OBJDIR)sse_sample_app$(EXE_SUFFIX) > $(OBJDIR)ftrace_stdout.out
	$(DIFF) -I "Total number of instructions: [0-9]*" $(OBJDIR)ftrace.out ftrace.reference
	$(DIFF) $(OBJDIR)ftrace_stdout.out ftrace_stdout.reference
	$(RM) $(OBJDIR)ftrace.out $(OBJDIR)ftrace_stdout.out

ftrace_no_specified_output_file.test: $(OBJDIR)ftrace$(PINTOOL_SUFFIX) $(OBJDIR)sse_sample_app$(EXE_SUFFIX)
	$(PIN) -t $(OBJDIR)ftrace$(PINTOOL_SUFFIX) -- $(OBJDIR)sse_sample_app$(EXE_SUFFIX) > $(OBJDIR)ftrace_no_output_file_stdout.out 2> $(OBJDIR)ftrace_no_output_file_stderr.out
	$(DIFF) $(OBJDIR)ftrace_no_output_file_stdout.out ftrace_stdout.reference
	$(DIFF) $(OBJDIR)ftrace_no_output_file_stderr.out ftrace_no_instrument.reference
	$(RM) $(OBJDIR)ftrace_no_output_file_stdout.out $(OBJDIR)ftrace_no_output_file_stderr.out

ftrace_no_instrument.test: $(OBJDIR)ftrace$(PINTOOL_SUFFIX) $(OBJDIR)sse_sample_app$(EXE_SUFFIX)
	$(PIN) -t $(OBJDIR)ftrace$(PINTOOL_SUFFIX) -instrument 0 -- $(OBJDIR)sse_sample_app$(EXE_SUFFIX) > $(OBJDIR)ftrace_no_instrument_stdout.out 2> $(OBJDIR)ftrace_no_instrument_stderr.out
	$(DIFF) $(OBJDIR)ftrace_no_instrument_stdout.out ftrace_stdout.reference
	$(DIFF) $(OBJDIR)ftrace_no_instrument_stderr.out ftrace_no_instrument.reference
	$(RM) $(OBJDIR)ftrace_no_instrument_stderr.out $(OBJDIR)ftrace_no_instrument_stdout.out
