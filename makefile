BINS := 
##  Base nodes for empty message performance testing
BINS := bin/base_uni_injector_message_publish
BINS += bin/base_uni_processor_message_republish
BINS += bin/base_uni_terminator_message_consume
BINS += bin/base_dual_injector_message_publish
BINS += bin/base_dual_processor_message_republish
BINS += bin/base_dual_terminator_message_consume
BINS += bin/base_lwd_injector_message_publish
BINS += bin/base_lwd_processor_message_republish
BINS += bin/base_lwd_terminator_message_consume
BINS += bin/base_mcd_injector_message_publish
BINS += bin/base_mcd_processor_message_republish
BINS += bin/base_mcd_terminator_message_consume
##  Working nodes for functional testing
BINS += bin/quix_mcd_injector_file_read
BINS += bin/quix_mcd_injector_tcp_recv
BINS += bin/quix_mcd_processor_file_write
BINS += bin/quix_mcd_processor_tcp_send
BINS += bin/quix_mcd_processor_fix_decode
BINS += bin/quix_mcd_processor_fix_encode
#INS += bin/quix_mcd_processor_manager_client
#INS += bin/quix_mcd_processor_manager_product
#INS += bin/quix_mcd_processor_manager_order
#INS += bin/quix_mcd_processor_manager_destination

SRCS := $(shell ls src/*/*.cc)
DEPS := $(patsubst src/%.cc,obj/%.d,$(SRCS))
TSTS := $(patsubst src/%.cc,obj/%.t,$(SRCS))
OBJS := $(patsubst src/%.cc,obj/%.o,$(SRCS))
MKFILES := makefile inc/settings.mk


include inc/settings.mk
include inc/common.mk
include inc/rules-cpp.mk


##
##  Base nodes for empty message performance testing
##

bin/base_uni_injector_message_publish : src/base_uni_injector_message_publish.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_uni_processor_message_republish : src/base_uni_processor_message_republish.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_uni_terminator_message_consume : src/base_uni_terminator_message_consume.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_dual_injector_message_publish : src/base_dual_injector_message_publish.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_dual_processor_message_republish : src/base_dual_processor_message_republish.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_dual_terminator_message_consume : src/base_dual_terminator_message_consume.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_lwd_injector_message_publish : src/base_lwd_injector_message_publish.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_lwd_processor_message_republish : src/base_lwd_processor_message_republish.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_lwd_terminator_message_consume : src/base_lwd_terminator_message_consume.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_mcd_injector_message_publish : src/base_mcd_injector_message_publish.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_mcd_processor_message_republish : src/base_mcd_processor_message_republish.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/base_mcd_terminator_message_consume : src/base_mcd_terminator_message_consume.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

##
##  Working nodes for functional testing
##

bin/quix_mcd_injector_file_read : src/quix_mcd_injector_file_read.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/quix_mcd_injector_tcp_recv : src/quix_mcd_injector_tcp_recv.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/quix_mcd_processor_file_write : src/quix_mcd_processor_file_write.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/quix_mcd_processor_tcp_send : src/quix_mcd_processor_tcp_send.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/quix_mcd_processor_fix_decode : src/quix_mcd_processor_fix_decode.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/quix_mcd_processor_fix_encode : src/quix_mcd_processor_fix_encode.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/quix_mcd_processor_manager_client : src/quix_mcd_processor_manager_client.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/quix_mcd_processor_manager_product : src/quix_mcd_processor_manager_product.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/quix_mcd_processor_manager_order : src/quix_mcd_processor_manager_order.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

bin/quix_mcd_processor_manager_destination : src/quix_mcd_processor_manager_destination.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 


-include $(DEPS)
