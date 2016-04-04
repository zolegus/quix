#QUIX
Multi process quick fix engine using no barriers or other sync primitives to take advantage of new intel cpus with high core counts 

## Philosophy
The overriding development philosophy for the quix engine is that many small, simple and highly focused processes lead to a more stable and extensible system than a monolithic beast which (in this developers experence) ends up subject to a higher rate of code entrophy as the codebase evolves.  The goal of this project is not so much to create a financial engine but to test the above view in a real world requirement.  The underlying framework that will be developed should be applicable in many problem domains.

Multihreading is an important illusion.  But it is still an illusion.  On an physical CPU with 4 cores you can only have 4 things executing in parallel.  It is true that threads block and having another thread you can schedule while the blocked thread is waiting leads to increased work throughput.  But this does not decrease latency and it is to decrease latency that this framework is being developed.  Also as physical CPU's become cheaper and cheaper with more and more cores the economics of making sure every last available cycle of a core is put to use shift towards doing the work itself with more efficency. 

## Architecture
At the heart of the quix archiecture is the idea of one core runs one node. One node equates to a one single threaded process.  

Doing this has several advantages for a low latency system. 

* Compilers have many more opportunites to optimize code when it is single threaded. 
* The node can give us almost deterministic performance metrics as for the most part our node owns all the core. 
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

