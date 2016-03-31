obj/%.o : obj/%.t
	$<
	$(CXX) -o $@ -c '$(patsubst obj/%.t,src/%.cpp,$<)' $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS)

obj/%.t : src/%.cc obj/%.d
	$(CXX) -o $@ $< $(CXXFLAGS)

obj/%.d: src/%.cc $(MKFILES)
	$(CXX) -o $@ $<  $(CXXFLAGS) -MM -MF $@ -MT '$(patsubst src/%.cc,obj/%.d,$<)' $<
