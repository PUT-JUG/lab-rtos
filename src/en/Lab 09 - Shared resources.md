# Shared resources, semaphores

Concurrent execution of many threads that utilize common, shared resources may lead to the
unexpected results. Have in mind that threads can be preempted at any time, which can easily
lead to a situation where data being modified by one thread can be wrongly interpreted or even
overwritten by another thread.

### Concurrent resource access

Make a new program with global integer variable. Create 10000 threads executing a simple
function that waits 1 milisecond and then increments the common global variable by 100.


*Hint*: Holding the execution of thread is easily available with function:
`std::this_thread::sleep_for(std::chrono::duration sleep_duration)`. Object that represents
the sleep time, `std::chrono::duration`, can be created using subtypes such as:
`std::chrono::milliseconds` or `std::chrono::milliseconds`, i.e.:

```cpp
// pausing current thread for 500 milliseconds
std::this_thread::sleep_for(std::chrono::milliseconds(500));
```

Wait to the finish of the execution of threads and check the result of addition. Run the
program several times. Is the result repeatable and correct? Think, which elementary operations
the processor has to perform in order to increment the variable. Is there a possibility, that such
sequence can be interrupted by another thread?

## Sempahores, mutexes, atomic operations

In order to avoid such situations, a *critical sections* are introduced to the program where concurrent access to shared resources occur. The cirical section is a part of the code which cannot be executed by more than one process at the time and this is where the access to shared resource is executed. Entering the critical sections can be controlled using semaphores - specialized variables that control the thread execution. The semaphore can be a counter, which describes the number of available resources. Every new thread that claims the resources, decreases the value of sempahore. When the count reaches zero, every next thread has to wait until any other processing thread finishes execution and releases the resources by incrementing the value of semaphore.

The specialized type of semaphore is a binary semaphore - *mutex*. Mutex can be assigned
only with two values - 0 or 1. Therefore, it is used when only one process can execute given
piece of code. Before entering the critical section, thread should check whether another process
has taken control over mutex and wait for its release if needed. If the mutex is free, thread
should lock it in order to prevent concurrent access to the restricted resource. Critical sections
should be as short as possible to prevent the multi-threaded program from slowing down significantly.
The operation of checking, locking and unlocking mutexes is a operation that cannot
be interrupted by another processes - an *atomic operation*. This is why the mutex handling
should be performed on the operational system level and taken into consideration by the system
scheduler.

## Mutex in C++

Mutexes in C++ are available thanks to the `std::mutex` class, accessible after including the
`<mutex>` header. 

You can execute three methods on the mutex type object:

* `lock()`: locks mutex, if it is already locked, waits for release
* `trylock()`: tries to lock mutex, returns true if the lock succeeded
* `unlock()`: unlocks the mutex

### Exercise 1 - Simple mutex

Improve the exemplary variable-incrementing program with mutex. Think, which part of a program is a critical section and introduce mutex to control access to the global variable.

## The producer-consumer problem

producer-consumer problem arises when there are asynchronously generated and processed data in the system. In general, there are two kinds of concurrent processes (producers and consumers) that share a common resource - a buffer storing data units. Producers shouldn’t add new data to the buffer when its full; on the other hand, consumers can’t process the data when the buffer is empty.


### Exercise 2 - Producer-consumer

Write a program that will show the operation of the producer and consumer on the example of products - random strings.

The producer and the consumer should work in two threads, which are executed in an inifinite loop:

* Producer: cyclically generates random 10-character strings and tries to put them in the buffer. If the buffer is full, the generated "product" is dropped and the error  is written to the console with a message. Then, the thread waits for a random time (specified by min-max parameters)

* Consumer: checks if there are any elements in the buffer, if so - retrieves them and writes to the file, each in a separate line, then waits for the time specified by the parameter.

The program should have the following parameters, defined by constants in the code:

* minimum period of procuders's operation
* maximum period of procuders's operation
* The consumer's period
* The maximum depth of the queue

As a buffer, you can use the queue from the standard library (`std::queue`), with appropriate security measures. Watch the program run in different conditions, changing the latency/depth of the queue and causing the buffer to overflow.

*Useful features:*

Generating a string of characters:

```cpp
std::string temp;
temp.resize(10);
for (int i = 0; i < 10; i++) {
    temp[i] = rand()%('z'-'a'+1)+'a';
}
```

Write a program in two versions:

(a) FIFO 1-1 queue
Launch one producer and one consumer, operating on two threads and exchanging data through a shared buffer. 

(b) Multiple agent FIFO queue
Extend the program to include the possibility of launching any number of producers and consumers specified by the user. Each consumer should save the data in a file with a name containing thread ID.



***
Author: *Adam Bondyra, Jakub Tomczyński*

Last modified: {JUG:MODIFICATION_DATE}
