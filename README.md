# Narrow-bridge-problem
## In this git, the problem of a narrow bridge in Pintos OS will be considered and solved.
This task is used to synchronize processes.
The bridge has 2 directions, and a maximum of two cars can move across the bridge at one time. It is not difficult to guess that the cars imitate flows and the bridge is the critical section. Also cars have priority 0 and 1 and cars that have higher priority must cross the bridge first. To solve this problem, variables such as semaphore were used.

To solve the problem, such a synchronization primitive as a semaphore was chosen, since it has two simple operations “up” and “down”, so when cars are prohibited from entering the bridge, the down operation is performed, in the case when cars are allowed to enter, the up operation is performed . 4 semaphores were created for each type of car: 2 for ambulances that move left and right and 2 for ordinary cars.
Also, for each type of car, a variable was created that is responsible for the number of cars. After each up operation, the number of cars decreased, respectively, after each down operation, it increased.
The cars_on_bridge variable was created, responsible for the number of cars on the bridge, after each up operation it increased.
Initially, all cars are prohibited from entering the bridge, as ambulances must be the first to cross the bridge. Next, the presence of ambulances is checked and if they are, then they pass the bridge first. The solution thus developed ensures that ambulances cross the bridge out of turn.
