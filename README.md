# ProducerConsumer
A multi-threaded producer-consumer program that use a bounded buffer. The buffer will have 16 slots total. The program will have the semaphores *full* and *empty* to keep track of the numbers of full and empty slots available to the consumers and producers. Other than that, the program will have a mutex to coordinate access to the buffer.

The number of producers, the number of consumers, and the number of items each producer produces will be specified by their binary log as command-line parameters. Thus, the following command should generate 4 producers, 16 consumers, and 32 items produced by each producer:

./producer-consumer 2 4 5

The items produced by the producer threads will be integers, and calculate using the following expression:

thread_number * num_produced + counter

where *thread_number* is an integer passed to each thread ranging from 0 to num_threads - 1, *num_produced* is the number of items produced by each producer, and *counter* is a local variable in each thread that gets initialized to 0 and incremented every time a new item is produced. The consumer threads will then consume these items by simply printing them.
