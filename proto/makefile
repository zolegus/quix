SRCS := $(shell ls src/*.c)
DEPS := $(patsubst src/%.c,obj/%.d,$(SRCS))
TSTS := $(patsubst src/%.c,obj/%.t,$(SRCS))
OBJS := $(patsubst src/%.c,obj/%.o,$(SRCS))

BINS := 

BINS += bin/uni/injector-base bin/uni/processor-base bin/uni/terminator-base

BINS += bin/dual/injector-base bin/dual/processor-base bin/dual/terminator-base

BINS += bin/lwd/injector-base bin/lwd/processor-base bin/lwd/terminator-base
BINS += bin/lwd/injector-file-read bin/lwd/processor-file-write
BINS += bin/lwd/injector-tcp-recv bin/lwd/processor-tcp-send
BINS += bin/lwd/processor-fix-decode bin/lwd/processor-fix-encode
BINS += bin/lwd/processor-order-manager

BINS += bin/mcd/injector-base bin/mcd/processor-base bin/mcd/terminator-base
BINS += bin/mcd/injector-file-read bin/mcd/processor-file-write
BINS += bin/mcd/injector-tcp-recv bin/mcd/processor-tcp-send
BINS += bin/mcd/processor-fix-decode bin/mcd/processor-fix-encode
BINS += bin/mcd/processor-order-manager

MKFILES := makefile ../inc/settings.mk ../inc/common.mk ../inc/rules-c.mk
COBJS := obj/behaviour.o obj/transport.o obj/arguments.o obj/daemon.o

include ../inc/settings.mk
include ../inc/common.mk
include ../inc/rules-c.mk

####
#### Uni Slot Messaging
####


bin/uni/injector-base: \
obj/injector.o $(COBJS) obj/transport-uni.o obj/behaviour-base-publish.o
	$(CC) -o $@ $^

bin/uni/processor-base: \
obj/processor.o $(COBJS) obj/transport-uni.o obj/behaviour-base-republish.o
	$(CC) -o $@ $^

bin/uni/terminator-base: \
obj/terminator.o $(COBJS) obj/transport-uni.o obj/behaviour-base-consumer.o
	$(CC) -o $@ $^

####
#### Dual Slot Messaging
####

bin/dual/injector-base: \
obj/injector.o $(COBJS) obj/transport-dual.o obj/behaviour-base-publish.o
	$(CC) -o $@ $^

bin/dual/processor-base: \
obj/processor.o $(COBJS) obj/transport-dual.o obj/behaviour-base-republish.o
	$(CC) -o $@ $^

bin/dual/terminator-base: \
obj/terminator.o $(COBJS) obj/transport-dual.o obj/behaviour-base-consumer.o
	$(CC) -o $@ $^

####
#### Light Weight Disruptor Messaging
####

bin/lwd/injector-base: \
obj/injector.o $(COBJS) obj/transport-lwd.o obj/behaviour-base-publish.o
	$(CC) -o $@ $^

bin/lwd/processor-base: \
obj/processor.o $(COBJS) obj/transport-lwd.o obj/behaviour-base-republish.o
	$(CC) -o $@ $^

bin/lwd/terminator-base: \
obj/terminator.o $(COBJS) obj/transport-lwd.o obj/behaviour-base-consumer.o
	$(CC) -o $@ $^

bin/lwd/injector-file-read: \
obj/injector.o $(COBJS) obj/transport-lwd.o obj/behaviour-file-read.o
	$(CC) -o $@ $^

bin/lwd/processor-file-write: \
 obj/processor.o $(COBJS) obj/transport-lwd.o obj/behaviour-file-write.o
	$(CC) -o $@ $^

bin/lwd/injector-tcp-recv: \
obj/injector.o $(COBJS) obj/transport-lwd.o obj/behaviour-tcp-recv.o
	$(CC) -o $@ $^

bin/lwd/processor-tcp-send: \
 obj/processor.o $(COBJS) obj/transport-lwd.o obj/behaviour-tcp-send.o
	$(CC) -o $@ $^

bin/lwd/processor-fix-decode: \
obj/processor.o $(COBJS) obj/transport-lwd.o obj/behaviour-fix-decode.o
	$(CC) -o $@ $^

bin/lwd/processor-fix-encode: \
obj/processor.o $(COBJS) obj/transport-lwd.o obj/behaviour-fix-encode.o
	$(CC) -o $@ $^

bin/lwd/processor-order-manager: \
obj/processor.o $(COBJS) obj/transport-lwd.o obj/behaviour-order-manager.o
	$(CC) -o $@ $^

####
#### Multi-Channel Disruptor Messaging
####

bin/mcd/injector-base: \
obj/injector.o $(COBJS) obj/transport-mcd.o obj/behaviour-base-publish.o
	$(CC) -o $@ $^

bin/mcd/processor-base: \
obj/processor.o $(COBJS) obj/transport-mcd.o obj/behaviour-base-republish.o
	$(CC) -o $@ $^

bin/mcd/terminator-base: \
obj/terminator.o $(COBJS) obj/transport-mcd.o obj/behaviour-base-consumer.o
	$(CC) -o $@ $^

bin/mcd/injector-file-read: \
obj/injector.o $(COBJS) obj/transport-mcd.o obj/behaviour-file-read.o
	$(CC) -o $@ $^

bin/mcd/processor-file-write: \
 obj/processor.o $(COBJS) obj/transport-mcd.o obj/behaviour-file-write.o
	$(CC) -o $@ $^

bin/mcd/injector-tcp-recv: \
obj/injector.o $(COBJS) obj/transport-mcd.o obj/behaviour-tcp-recv.o
	$(CC) -o $@ $^

bin/mcd/processor-tcp-send: \
 obj/processor.o $(COBJS) obj/transport-mcd.o obj/behaviour-tcp-send.o
	$(CC) -o $@ $^

bin/mcd/processor-fix-decode: \
obj/processor.o $(COBJS) obj/transport-mcd.o obj/behaviour-fix-decode.o
	$(CC) -o $@ $^

bin/mcd/processor-fix-encode: \
obj/processor.o $(COBJS) obj/transport-mcd.o obj/behaviour-fix-encode.o
	$(CC) -o $@ $^

bin/mcd/processor-order-manager: \
obj/processor.o $(COBJS) obj/transport-mcd.o obj/behaviour-order-manager.o
	$(CC) -o $@ $^

####
#### End Of Makefile
####

