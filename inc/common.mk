.SECONDARY:

.PHONY: all
all: $(BINS)


.PHONY: clean
clean:
	@touch $(BINS) $(OBJS) $(TSTS) $(DEPS)
	$(RM) $(BINS) $(OBJS) $(TSTS) $(DEPS) $(shell find . -name '*~')


