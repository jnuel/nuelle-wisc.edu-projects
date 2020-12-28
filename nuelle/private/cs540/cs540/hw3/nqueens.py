#!/usr/bin/env python
# coding: utf-8

# In[10]:


import heapq
import random

# -- given a state of the board, return a list of all valid successor states
def succ(state, static_x, static_y):
    succs = []
    n = len(state)
    if state[static_x] != static_y:
        return []
    else:
        for i in range(n):
            if i == static_x and state[i] == static_y:
                continue
            else:
                if state[i] + 1 < n:
                    new_state = state.copy()
                    new_state[i] = new_state[i] + 1
                    succs.append(new_state)
#                     print("appended + 1", new_state)
                if state[i] - 1 > -1:
                    new_state = state.copy()
                    new_state[i] = new_state[i] - 1
                    succs.append(new_state)
#                     print("appended - 1", new_state)
    # return them in ascending order using sorted()
    return sorted(succs)

# -- given a state of the board, return an integer score such that the goal state scores 0
def f(state):
    attacked = state.copy()
    num_attacked = 0  
    
    # check rows
    for i in range(len(state)):
        cur = state[i]
        for each in range(len(state)):
            if each != i:
#                 print("state[each]:", state[each], "each:", each, cur)
                if state[each] == cur:
#                     print("attacked")
                    attacked[i] = -1
                    break
                    
    for i in range(len(attacked)):
        if attacked[i] == -1:
            num_attacked += 1
            
#     print("num_attacked:", num_attacked, attacked)
    if num_attacked == len(state): # don't have to search any further
        return num_attacked
    else:
        num_attacked = 0
    
    # check diagonals
    for i in range(len(state)):
        if attacked[i] != -1:
            cur = state[i]
#             print("i:", i, "cur:", cur)
            count = 1
            for each_minus in range(i-1, -1, -1):
#                 print("each minus - 1:", each_minus, "state[each_minus]:", state[each_minus], cur+count, cur-count)
                if state[each_minus] == cur + count or state[each_minus] == cur - count:
#                     print("attacked")
                    attacked [i] = -1
                    break
                else: 
                    count += 1
                    
            count = 1
            for each_plus in range(i+1, len(state)):
#                 print("each plus:", each_plus,"state[each_plus]:", state[each_plus])
                if state[each_plus] == cur + count or state[each_plus] == cur - count:
#                     print("attacked")
                    attacked[i] = -1
                    break
                count += 1
                
    for i in range(len(attacked)):
        if attacked[i] == -1:
            num_attacked += 1
            
    return num_attacked
    
# -- given the current state, use succ() to generate the successors and return the selected next state
def choose_next(curr, static_x, static_y):
    if curr[static_x] != static_y:
        return None
    
    succs = succ(curr, static_x, static_y)
    succs.append(curr)
    succs = sorted(succs)
    
    pq = []
    for each in succs:
        pq.append((f(each), each))
    
#     print(*pq, sep="\n")
    pq = sorted(pq)
#     print("sorted pq", pq)
    
    return pq[0][1]
    
# -- run the hill-climbing algorithm from a given initial state, return the convergence state
def n_queens(initial_state, static_x, static_y, print_path=True):
    f_cur = f(initial_state)
    cur = initial_state
    while f_cur != 0:
        if print_path:
            print(cur, " - f=", f_cur, sep="")
        next_state = choose_next(cur, static_x, static_y)
        f_next = f(next_state) 
        if next_state == cur:
            return next_state
        elif f_next == f_cur:
            if print_path:
                print(next_state, " - f=", f_cur, sep="")
            return next_state
        else:
            cur = next_state
            f_cur = f(cur)
    if print_path:
        print(cur, " - f=", f_cur, sep="")
    return cur

# -- run the hill-climbing algorithm on an n*n board with random restarts
def n_queens_restart(n, k, static_x, static_y):

    start_time = time.time()
    random.seed(1)
    num_times = 0
    solutions = []
    
    while num_times < k:
        board = []
        for col in range(n):
            if col == static_x:
                board.append(static_y)
            else:
                board.append(random.randint(0, n-1))
        returned = n_queens(board, static_x, static_y, print_path=False)
        f_ret = f(returned)
        if f_ret == 0:
#             print("f_ret == 0!!")
            print(returned, " - f=", f_ret, sep="")
            return
        else:
            solutions.append((f_ret, returned))
#             print(returned, " - f=", f_ret, sep="")
            num_times += 1
#     print("didn't find a good one", len(solutions))
    solutions = sorted(solutions)
#     print(solutions)
    best_f = solutions[0][0]
    
    for each in solutions:
        if best_f == each[0]:
            print(each[1], " - f=", each[0],sep="")
        else:
            break


# In[ ]:





# In[ ]:




