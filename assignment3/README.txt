mpuchkov@sfu.ca, 301314335



** RUNNING **
1) make
2) ./s



** SIMULATION **
Simulation will run as long as Process(pid:0) exists.
To finish, call 'E' [Exit] on all processes until no more left.



** IMPLEMENTATION ISSUES **
1) P and V are not implemented completely and will not block
2) Will get stuck if all processes other than Process(pid:0) are blocked unless Kill other
3) Process indices are not reusable
4) If only 2 processes exist, (pid:0) will run when Q called
5) (pid:0) will run as any other process in Low Priority Queue[2] when Q called
6) Can only send one message to one process, others will be overridden
7) Calling Q too many times causes SegFault11 (LIST implementation issue)



2018-03-18