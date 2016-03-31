include makefile.settings

# Standard targets

all:

install:

install-html:
install-dvi:
install-pdf:
install-ps:

uninstall:

install-strip:

clean:
	@touch obj/__rm__
	$(RM) obj/*

distclean:

mostlyclean:

maintainer-clean:

TAGS:

info:

html:
dvi:
pdf:
ps:

dist:

check:

installcheck:

installdir:



.SECONDARY:

obj/%.o : obj/%.t
	$(CXX) -o $@ -c '$(patsubst obj/%.t,%.cpp,$<)' $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS)

obj/%.t : %.cc obj/%.d
	$(CXX) -o $@ $< $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS)
	$@

obj/%.d: %.cc
	$(CXX) -o $@ $<  $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -MM -MF $@ -MT '$(patsubst %.cc,obj/%.d,$<)' $<
