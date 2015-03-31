ftrace_no_specified_output_file.test: $(OBJDIR) $(OBJDIR)sse_sample_app$(EXE_SUFFIX)
	$(MAKE) $(FTRACE_CMD_VARS) $(OBJDIR)ftrace$(PINTOOL_SUFFIX)
	$(PIN) -t $(OBJDIR)ftrace$(PINTOOL_SUFFIX) -- $(OBJDIR)sse_sample_app$(EXE_SUFFIX) &> $(OBJDIR)$(@:.test=.out)
	$(DIFF) $(OBJDIR)$(@:.test=.out) tests/ftrace.stdout.reference
	$(RM) $(OBJDIR)$(@:.test=.out)

ftrace_no_instrument.test: FTRACE_CMDLINE_FLAGS := -instrument 0
ftrace_no_instrument.test: EXPECTED_STDOUT := tests/ftrace.stdout.reference
ftrace_no_instrument.test: $(OBJDIR)ftrace_no_instrument.out

$(OBJDIR)ftrace_no_instrument.out: $(OBJDIR)
	touch $@
