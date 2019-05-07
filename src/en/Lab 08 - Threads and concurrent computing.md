# Concurrent programming - creating threads, paralleling calculations

Concurrent programming is a set of programming techniques aimed at concurrent (parallel) processing, thanks to the simultaneous existence of multiple threads or processes that operate on shared data.

Concurrent processing is essential for real-time systems that need to manage multiple processes simultaneously, or for servers that need to handle a large number of customer queries. In these applications, it is unacceptable for one task to paralyze the entire system.

In multiprocessor systems or with multi-core processors, concurrent processing also allows for large processing efficiency gains, as long as the task can be divided into smaller portions, which can be performed simultaneously. Each thread can then be executed on a separate core.

Available resources are managed by the operating system and the system allocates processing time to particular threads on available cores, using system-specific scheduling algorithms. When there are more threads than cores, they are preempted cyclically to avoid process starvation.

The course uses the C++ language and the mechanisms provided by the standard library for the C+++11 extensions, which are available in most modern compilers, e.g. Visual Studio from the 2013 version. Some environments/compilers require additional options to enable the C++11 standard (e.g. the `std=c++11` flag for the `gcc` compiler).

Similar functionality can be achieved by using i.e. *pthreads* libraries (on systems compatible with *POSIX*, e.g. Linux), or dedicated solutions provided by individual systems (e.g. threads in the *Windows API*). There are also several languages designed for concurrent processing, e.g. Go, Erlang, Ada.


## Fist multi-threaded program

To use threads, include the header `<thread>`. A single thread is represented by an object in the class `std::thread`. The class constructor accepts the pointer to the function to be executed in the thread and optionally the arguments to be passed on to the function to be executed:

```cpp
// creates a first thread that executes a "foo" function
std::thread first(foo);
// creates a second thread executing "bar" function, with arguments
std::thread second(bar, 0, 0.2);
```

The value returned by functions executed in the thread (here: `foo` and `bar`) is ignored. In order to return results from functions executed in the thread, we must use a global variable, a field in the class or passing variables through the pointer. Passing variables to the thread by reference is prohibited (unless we use the wrapper `std::ref()` or `std::cref()`).

In order to wait for the thread to end, we call the join() method on it:

```cpp
first.join(); // waits for the thread to be completed
```

An example of a program that runs two threads may therefore look like this:

```cpp
#include <iostream>
#include <thread>

void foo() {
    for (int i = 0; i < 5; i++) {
        std::cout << "hello foo" << std::endl;
    }
}

void bar(int x, float y) {
    for (int i = 0; i < 5; i++) {
        std::cout << "hello bar " << x << " " << y << std::endl;
    }
}

int main(int argc, const char * argv[]) {
    std::thread first(foo);
    std::thread second(bar, 0, 0.2);

    std::cerr << "threads started" << std::endl;

    // wait for threads to finish:
    first.join();
    second.join();

    std::cerr << "threads finished" << std::endl;

    return 0;
}
```

Run given example and analyze console output.

## Useful functions

### Measuring the execution time

In order to determine whether out concurrent solutions profit in efficiency, we can use the time measurement functions provided in the library `<chrono>`. An example of using these mechanisms to measure the time elapsed between two places in the code looks as follows:

```cpp
// ...we declare objects that store the execution start and finish time...
std::chrono::time_point<std::chrono::system_clock> start, end;
// We save the current time as the starting point...
start = std::chrono::system_clock::now();

/*
a code whose performance we're testing
*/

// we save the current time as a moment of completion
end = std::chrono::system_clock::now();
// Calculate the differences 
std::chrono::duration<double> elapsed_time = end - start;
// Writing out the time in seconds
std::cerr << "time: "<<< elapsed_time.count() << std::endl;
```

Remember that running a program compiled in *debug* version may give different performance results than *release* version.


### Checking the number of available cores

The number of concurrent hardware threads available in the system can be checked using the function `std::thread::hardware_concurrency()`. In case of processors with Hyper-threading technology, each physical core is seen in the system as two logical cores, so for example, for a 2-core processor, the function can return the value of 4.

**Note: If the operating system does not offer the possibility to check the number of concurrent threads, the function will return 0. A correctly written program should take this eventuality into account.

Remember that creating threads and changing the context between them (preemption) is a time consuming operation, so creating too many threads (more than those available in hardware), or threads that run too short, may be unprofitable.

## Exercises

### 1. Collection of threads

Modify the first example to run the number of threads specified by the user input. The threaded function is to display a message containing the unique *ID* of the thread in the console. The *ID* can be acquired from the thread level using the `std::this_thread::get_id()` function. Think about how you can conveniently store information about your thread collection and wait for every thread to finish.

**Note** that the `std::thread` object is not copyable (it is not *CopyConstructible* or *CopyAssignable*), which means that you cannot trivijustally create a temporary object first and then place it in the collection using the `push_back` method.)

Therefore, we can immediately place the thread in the collection:

```cpp
collection.push_back(std::thread(foo));
```

or force the transfer using the `std::move` tool and `emplace` methods:

```cpp
std::thread temporary(foo);
collection.emplace_back(std::move(temporary);
```

### 2. Searching for prime numbers

First numbers are often found in the proximity of the *2<sup>n</sup> - 1* collection, where n ∈ N.

Write a program that uses this property to search for prime numbers. In order to do this, draw a set of initial values that meet the above relation. Remember about the limits of the range of numbers that can be stored in the selected variable type. Then, for each of the generated numbers, search the set of natural numbers in a descending direction until you meet the prime number. Save the found number in the vector provided.
Think about how to divide the work into multiple threads and how to pass a portion of the set of numbers to be searched for to each thread. Compare performance results based on the number of threads and the set size of the set. To get the same drawn numbers every time you start, you can initiate a random number generator with a constant value, e.g. `srand(0)`.



***
Author: *Jakub Tomczyński, Adam Bondyra*

Data ostatniej modyfikacji: {JUG:MODIFICATION_DATE}
