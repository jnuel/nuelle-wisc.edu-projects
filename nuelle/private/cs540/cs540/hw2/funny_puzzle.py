#!/usr/bin/env python
# coding: utf-8

# In[1]:


import heapq   
import copy

# Finding the manhattan distance
def manhattan(i, data_point2):
    width = 3
    y1 = i % width;   
    x1 = i // width;
    #print("dp1", data_point1, "dp2", data_point2 )
#     x1 = data_point1[0]
#     y1 = data_point1[1]
    x2 = data_point2[0]
    y2 = data_point2[1]
    final = abs(x1-x2) + abs(y1-y2)
    #print(final)
    return final
#     return 0

def print_succ(state):
    returned = gen_succ(state)
    for each_list in returned:
        for x in range(0,len(each_list),2):
            print (each_list[x], " h=", each_list[x+1], sep="")


def solve(state):
    pq = [] 
    md = 0 # manhattan distance variable
    
    # Find the manhattan distance of the current state
    for num in range(9):
        if state[num] == 1:
            md = md + manhattan(num, [0,0])
        elif state[num] == 2:
            md = md + manhattan(num, [0,1])
        elif state[num] == 3:
            md = md + manhattan(num, [0,2])
        elif state[num] == 4:
            md = md + manhattan(num, [1,0])
        elif state[num] == 5:
            md = md + manhattan(num, [1,1])
        elif state[num] == 6:
            md = md + manhattan(num, [1,2])
        elif state[num] == 7:
            md = md + manhattan(num, [2,0])
        elif state[num] == 8:
            md = md + manhattan(num, [2,1])
            
    # Add initial state to priority queue(open)          [0] [1]    2-[0 1  2]
    heapq.heappush(pq, (md, state, (0, md, -1))) # (pq, (f, state, (g, h, parent)))     [0]  [1]  [2]   [3]
    closed = [] # array of the closed/visited nodes, save the parent and the g cost: (state, g, parent, h)
    dist = md
    moves = 0
    
    while len(pq) > 0:
        # Get the current state from OPEN, place on closed
        cur_state = heapq.heappop(pq)
        
        # check to see if the cur state is the goal
        if cur_state[2][1] == 0:
            path = []
            index = cur_state[2][2]
            if index != -1:                                                    
                while index != -1:
                    path.append(cur_state)  
                    cur_state = closed[index]
                    index = closed[index][2]
                    
                path.append(cur_state)
                path = path[::-1]
                for each in path[:-1]:
                    print(each[0], " h=",each[3], " moves: ", each[1], sep="")
                print(path[len(path)-1][1], " h=", path[len(path)-1][2][1]," moves: ",path[len(path)-1][2][0], sep="")
            else:
                print(cur_state[1], " h=", cur_state[2][1]," moves: ",cur_state[2][0], sep="")
            break
        visited = already_visited(closed, cur_state[1]) #return -1 if haven't visited, index in closed if not
        if visited != -1: # we've already visited, check if we have a faster route, g is lower change it in closed. 
            g_old = closed[visited][1]
            g_new = cur_state[2][0]
            new_parent = cur_state[2][2]
            # if it has been visited, see if we have a faster route, g is lower, have visited return the index of where it is in closed
            # if it is lower, then replace the parent of the one that is in the closed set with the current parent
            if g_new < g_old:
                del closed[visited]
                closed.append([cur_state[1], g_new, cur_state[2][2], cur_state[2][1]])
        else: # we haven't visited, add the current one to the closed list
            closed.append([cur_state[1], cur_state[2][0], cur_state[2][2], cur_state[2][1]])
            # if it hasn't been visited before,
            # increment moves
            moves = cur_state[2][0] + 1
        
            # add all of the children to open, 
            next_succs = gen_succ(cur_state[1])
            for each_list in next_succs:
                for x in range(0,len(each_list),2):
                    h_new = each_list[x+1]
                    g_new = moves
                    f_new = h_new + g_new
                    # giving the children the index of the parent in the closed list, using the size of closed -1 
                    parent = len(closed) - 1
                    heapq.heappush(pq, (f_new, each_list[x], (g_new, h_new, parent)))

# Helper function to find if a node has been added to closed, returns the index if so.    
def already_visited(alist, node):
    index = 0
    for each in alist:
        if each[0] == node: 
            return index # return where it is in the closed
        index += 1
    return -1 # else return -1, saying that it isn't visited before
         
def gen_succ(state):
    #Set up the swaps possible
    possible_swaps = [
        [1,3],
        [0,2,4],
        [1,5],
        [0,4,6],
        [1,3,5,7],
        [2,4,8],
        [3,7],
        [4,6,8],
        [5,7]
    ]
    
    position = 0
    for i in range(9):
        if state[i] == 0:
            position = i
            
    #print("found position is ", position)

    # Find the successors
    lists = find_succ(state, position, possible_swaps[position])
    
    # Find the manhattan distance for each list    
    md = 0
    succs = []
    
    lists = sorted(lists)
    for each in lists:
        for num in range(9):
            #print("num is " , num)
            if each[num] == 1:
                md = md + manhattan(num, [0,0])
            elif each[num] == 2:
                md = md + manhattan(num, [0,1])
            elif each[num] == 3:
                md = md + manhattan(num, [0,2])
            elif each[num] == 4:
                md = md + manhattan(num, [1,0])
            elif each[num] == 5:
                md = md + manhattan(num, [1,1])
            elif each[num] == 6:
                md = md + manhattan(num, [1,2])
            elif each[num] == 7:
                md = md + manhattan(num, [2,0])
            elif each[num] == 8:
                md = md + manhattan(num, [2,1])

        succs.append((each, md))
        md = 0
    return succs

    
def find_succ(arr, position, possible_swaps):
    toreturn = []
                
    for x in possible_swaps: 
        #print("all x are" ,x)
        acopy = copy.deepcopy(arr)
        save = acopy[x] # save the value that we are going to swap with
        #print(save, "= save and x is", x)
        acopy[position] = save
        acopy[x] = 0
        toreturn.append(acopy)
        
        
    return toreturn


# In[ ]:




