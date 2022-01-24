# TSP

implemented algorithms:
- brute force <br>
- branch and bound least cost <br>
- simmulated annealing <br>
- tabu search<br>
- genetic algorithm <br>

running: 
```console
./tsp arg1 arg2 arg3
```

arg1 = instance name with extension f.e m6.atsp <br>

<br>

arg2 = method_name: <br>

- bf || bruteforce  runs brute_force algorithm 
- bnb || branchandbound  runs branch_and_bound algorithm
- sa || simulatedannealing  runs simulated_annealing algorithm
- ts || tabusearch  runs tabu_search algorithm
- ga || genetic runs genetic algorithm


**as default running genetic_algorithm**


<br>
arg3 = initial_solution <br>
-  n || neighbours || neighbour runs nearest neighborhood initial solution for ts and sa algorithms <br>
-  everything else runs nearest neighborhood initial solution for ts and sa algorithms <br>

**only relevant for sa and ts algorigthm, as default running random initial solution**

