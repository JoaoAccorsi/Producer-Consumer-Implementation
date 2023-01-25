# Producer-Consumer-Implementation

> Code developed for the Bachelor's Degree in Computer Science:  
> - University: Universidade do Vale do Rio dos Sinos (Unisinos)  
> - Subject: Analysis and Application of Operating Systems  
> - Semester: 2022/02  

C program to simulate the producer consumer problem.

The program has three producers who produces one cargo transport each: planes, ships and trucks. 

So on, once it got produced, each product is put in a sorting belt, which in the end, has two consumers.

Then, the consumers got the products from the sorting belt and classify the cargo transports according its maximum weight.

If it is higher than 200.000kg it is high weight. Or else, low weight.

Mutex is used to avoid the two consumers to access the sorting belt at the same time, and semaphores to control if the sorting belt has cargo to be classified or not. 

The final diagram is as below:

![Diagram](https://user-images.githubusercontent.com/60155867/214568316-43731db0-55a8-4d78-abb7-224a33cc3504.png)

> - Compilate: gcc trabGA.c queue.c -o trabGA.out -lpthread -lrt
> - Run: ./trabGA.out
