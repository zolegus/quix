#QUIX
Multi process quick fix engine using no barriers or other sync primitives to take advantage of new intel cpus with high core counts 

## Philosophy
The overriding development philosophy for the quix engine is that many small, simple and highly focused processes lead to a more stable and extensible system than a monolithic beast which (in this developers experence) ends up subject to a higher rate of code entrophy as the codebase evolves.  The goal of this project is not so much to create a financial engine but to test the above view in a real world requirement.  The underlying framework that will be developed should be applicable in many problem domains.

Multihreading is an important illusion.  But it is still an illusion.  On an physical CPU with 4 cores you can only have 4 things executing in parallel.  It is true that threads block and having another thread you can schedule while the blocked thread is waiting leads to increased work throughput.  But this does not decrease latency and it is to decrease latency that this framework is being developed.  Also as physical CPU's become cheaper and cheaper with more and more cores the economics of making sure every last available cycle of a core is put to use shift towards doing the work itself with more efficency. 

## Architecture

###Node
At the heart of the quix archiecture is the idea of one core runs one node. One node equates to a one single threaded process.  

Doing this has several advantages for a low latency system. 
* Compilers have many more opportunites to optimize code when it is single threaded. 
* The node can give us almost deterministic performance metrics as for the most part the node owns all the core. 
* Sharing of code cachelines is kept to a minimum as we need not have multiple cores running the same code. 
* Same can be said of data cachelines as data is quite often is directly related to the code that requires it. 
* On modern Intel processors we may not need to use a memory barrier between physical cores (SDMv3 8.2.3.7)
[comment]: (On modern intel processors we are guarented that one physical core will see the writes of another physical core in program write order.  This means no need for any syncronisation of any kind between the nodes.  In other words we get the transports required memory barrier for free (Intel® 64 and IA-32 Architectures Software Developer’s Manual, Volume 3 - 8.2.3.7). [Note: some non-temporal move instructions and string operations might not be subject to this.  Need to clarify])

A node comes in three personalities. 
* An **injector** node aquires an empty event, gives it some initial state and then commits it into the event transport. 
* A **processor** node reaquires an event from a node it follows, performs some highly focused task and commits it back into the event transport.
* A **terminator** node reaquires an event, can do some more work just as a processor will do but releases the event for re-cycling. 
 
A node has some restrictions on how it iteracts with its external environment. 
* A nodes only input is an event aquired/reaquired from the event transport. 
* A nodes only output is an event commited/released to the event transport. 
* All changes to a nodes state (including a file) must not be shared with another node. 
* Any state changes another node sees in deterministic order are not subject to the previous restriction. 
 
This leads to several advantages that are more difficult to implement in a more monolithic system
* Component testing can be done for each node in total isolation.  Including performance testing.
* System testing can be done by replacing injector and terminator nodes with nodes that support the testing process.  For example you could replace the tcp_recv and tcp_send nodes with file_read and file_write nodes to replay production data thru the latest codebase in order to understand the changes that will happen as a result of the new code base.
* System recovery (even on another physical box) can be done by replaying the events back thru the system.

The node is responsible for binding the transport to the behaviour.

###Transport

Each node has the same transport bound to it to ensure each node can talk to its followers.

Transport interface comprises of 6 functions
* **aquire_test**: Test if an event is ready to be aquired
* **aquire**: Aquire an event.  Blocks until an event become available.
* **commit**: Commit an event to the transport system to be pick up by followers.
* **reaquire_test**: Test if an event is ready to be reaquired.
* **reaquire** Reaquire and event.  Blocks until an event becomes available.
* **release**: Release an event to the transport system for recycling.
 
Currently there are 4 transports implemented
* **uni**: A simple pointer handoff transport.  Like an event queue of one.
* **dual**: A simple pointer handoff transport with tick-tock.  Like an event queue of two.
* **lwd**:  Inspired by the discruptor pattern but made much more lightweight.  One injector only.
* **mcd**:  Extension of the lwd to allow muliple channels.  One injector per channel only.
 
Under load I got the following timings on a quad core Haswell Xeon.  
Results are for 1 billion events sent from an injector to a terminator (one hop).
*  **uni**: 100s approx.  Expect will drop in an unloaded environment.
*  **dual**: 60s approx.  Expect will drop in an unloaded environment.
*  **lwd**: 15s approx.  Best results but unsuitable for complex topologies.
*  **mcd**: 30s approx.  Expect will rises with more channels
  
Quix will be implemented using the mcd transport.  However transports are swappable so future performance testing might show uni or dual to have better results.  This is based on the view that simpler leads to lower latencies (mostly) and uni/dual are simpler than mcd.

The mcd transport allows for complicated topologies by having nodes follow muliple channels such as:
```
 1> [N1:recv] 1> [N2:decoder] 1>        1> [N7:request_enrich ] 1>          1> [N3:encoder] 1> [N4:send] 1>
Client                     [N5:ClientManager]                [N6:ExchangeManager]                     Exchange
 <2 [N4:send] <2 [N3:encoder] <2        <2 [N8:response_enrich] <2          <2 [N2:decoder] <2 [N1:recv] <2
```
In the above ExchangeManager is following both N7:request_enrich on channel 1 and N2:decoder on channel 2.

##Behaviour

Each node has a behaviour bound to it.  At its core a behaviour is nothing more than a functor that takes the event as an argument and returns it to the caller when done.  The collection of behaviours bound to the active nodes defines what the system can do.

Currently the follow behaviours are implemented.
*  **event_publish**: Used to inject empty events into the transport for performance testing.
*  **event_republish**: Used to forward events from/into the transport for performance testing.
*  **event_consume**: Used to terminate empty events received from the transport for performance testing.
*  **file_read**: Read file and inject events into the transport.  One line per event.
*  **file_write**: Write received events to file.  One line per event.
*  **tcp_recv**: Receive a descriptor on an incomming connection then receive events from received descriptor.
*  **tcp_send**: Receive a descriptor on an incomming connection then send events to received descriptor.
  
The following behaviours are started but yet to be completed.
*  **fix_decoder**: Decode a fix message into a form more friendly to the following node.
*  **fix_encoder**: Encode a fix message from the form that is ore friendly to the nodes being followed.
  
The following behaviours are not yet implemented.
*  **client_manager**: Event enrichment related to the client.
*  **destination_manager**: Event enrichment related to the destination.
*  **order_manager**: Event enrichment related to the order.
*  **product_manager**: Event enrichment related to the product.
