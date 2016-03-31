obj/%.o: src/%.c obj/%.d
	$(CC) -o $@ -c $< $(CFLAGS)

obj/%.d: src/%.c $(MKFILES)
	$(CC) -o $@ $< $(CFLAGS) -MM -MF $@ -MT '(patsubst src/%.c,obj/%.d,$<)' $<

