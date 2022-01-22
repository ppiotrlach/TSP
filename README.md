# TSP

implemented algorithms:
- brute force <br>
- branch and bound least cost <br>
- simmulated annealing <br>
- tabu search<br>

running: 
```console
./tsp arg1 arg2 arg3
```

arg1 = instance name with extension f.e m6.atsp <br>

<br>
<br>

arg2 = method_name: <br>

- bf || bruteforce  runs brute_force algorithm 
- bnb || branchandbound  runs branch_and_bound algorithm
- sa || simulatedannealing  runs simulated_annealing algorithm
- ts || tabusearch  runs tabu_search algorithm
<br>

**as default running tabu_search algorithm**

<br>
<br>



arg3 = initial_solution <br>
-  n || neighbours || neighbour runs nearest neighborhood initial solution for ts and sa algorithms <br>
-  everything else runs nearest neighborhood initial solution for ts and sa algorithms <br>

**as default running random initial solution**

