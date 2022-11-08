# Narrow-bridge-problem
## In this git, the problem of a narrow bridge in Pintos OS will be considered and solved.
This task is used to synchronize processes.
The bridge has 2 directions, and a maximum of two cars can move across the bridge at one time. It is not difficult to guess that the cars imitate flows and the bridge is the critical section. Also cars have priority 0 and 1 and cars that have higher priority must cross the bridge first. To solve this problem, variables such as semaphore were used.
