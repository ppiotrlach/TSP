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
arg2 = method_name: <br>
&nbsp;&nbsp;&nbsp;bf || bruteforce  runs brute_force algorithm <br>
&nbsp;&nbsp;&nbsp;bnb || branchandbound  runs branch_and_bound algorithm <br>
&nbsp;&nbsp;&nbsp;sa || simulatedannealing  runs simulated_annealing algorithm <br>
&nbsp;&nbsp;&nbsp; ts || tabusearch  runs tabu_search algorithm <br>
&nbsp;&nbsp;&nbsp; as default running tabu_search algorithm

arg3 = initial_solution <br>
&nbsp;&nbsp;&nbsp;  n || neighbours || neighbour runs nearest neighborhood initial solution for ts and sa algorithms <br>
&nbsp;&nbsp;&nbsp;  everything else runs nearest neighborhood initial solution for ts and sa algorithms <br>
&nbsp;&nbsp;&nbsp; as default running random initial solution

