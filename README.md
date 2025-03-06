# Website Crawler
Using C++ and Windows Sockets, this multi-threaded application crawls through inputted URLs


## Code Architecture

There are 3 main classes that interact with each other: **Crawler**, **Socket**, and **Url**.

The **Url** class parses a given `char*` into its respective parts. This includes its host name, request, path, along with the entire original url.

The **Socket** class is responsible for maintaining the `SOCKET` used to communicate with the internet. This includes constructing, connecting, sending, receiving, and closing the socket. The **Socket** class maintains a **Url** object to keep track of the desired link it wants to connect to. In addition, the **Socket** class is responsible of keeping track of duplicate host names and ips.

The **Crawler** class is the wrapper to the multithreading aspect. It contains the `Crawler::Run` function that each thread runs. In addition, it keeps track of the overall statistics, and prints them out regularly.

## Lessons Learned

There were two main lessons I learned: socket lifecycles and multithreading.

A client socket lifecycle is as follows: 

1. socket()
2. connect()
3. recv() + send()
4. close()

At any point, these could fail, so it is very important to make your code very fault resistant. In addition, I learned about how to convert a link into a readable format for the socket to consume. This includes DNS Lookup, and formatting a get / head request correctly

Multithreading was another particular challenge. I learned about how to both start threads, and safely close the threads. I gained a better understanding on mutexes to keep critical sections safe. In addition, I learned about events that could be set up to notify particular threads to quit
