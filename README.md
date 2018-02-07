The Lightweight Kernel is a simple kernel that allows multiple functions to be run essentially as programs. Each process
has its own process control block and stack. Each process saves and restores its state from its individual stack when a
time quantum expires for each process’s run time.

The kernel supports 5 different levels of priority: HIGHEST, HIGH, MEDIUM, LOW and IDLE. The highest priorities are run
in a round-robin fashion until they terminate so the priority below it can run. If no processes remain in the running
queues, the idle process is switched in and run until there is a higher priority process to prevent full termination of the
program in the main loop.

The kernel supports basic inter-process communication through the sending and receiving of messages. In order to
receive a message, a process is required to have bound to a message queue. Only one message queue can be bound to
any one process and a process can only have one message queue. If a process attempts to receive a message but there
are no messages available, it is sent to a blocked queue until there is a message. In order to send a message, a process is
not required to be bind to a message queue. When a process sends a message, the process that the message queue
belongs to is unblocked if it was previously blocked which allows it to the receive the message.