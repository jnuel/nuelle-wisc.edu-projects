import heapq   
import copy

state = [1,2,3,4,5,0,7,8,6]

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
            print (each_list[x], each_list[x+1])


def solve(state):
    pq = [] 
    md = 0
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
            
    # Add initial state to priority queue open
    heapq.heappush(pq, (md, state, (0, md, -1)))
    closed = {}
    OPEN = {}
    OPEN[state] = 0
    solution = {}
    dist = md
    moves = 0
    
    while OPEN:
        # Get the current state from OPEN, place on closed
        cur_state = heapq.heappop(pq)
        del OPEN[cur_state[1]]
        closed[cur_state[1]] = cur_state[2][0]
        # If the manhattan dist of the cur_state is 0, solved!
        if cur_state[2][1] == 0:
            break
        moves = moves + 1
        next_succs = gen_succ(cur_state[1])
        for each_list in next_succs:
                for x in range(0,len(each_list),2):
                    h_new = each_list[x+1]
                    g_new = moves
                    f_new = (each_list[x+1] + moves
                    if closed.get(each_list[x]) is None:
                             if OPEN.get(each_list[x]) is None:
                        heapq.heappush(pq, (f_new, each_list[x], (g_new, h_new, (cur_state[2][2])+1)))
                    else:
                        if each_list[x] in OPEN:
                             if OPEN.get(each_list[x]) > g_new:
                                 OPEN.update({each_list[x] : g_new})
                        elif each_list[x] in closed:
                             del closed[each_list[x]]
                             OPEN[each_list[x]] = g_new
                        
           
    
#     for key, value in solution.items():
#         print("solution", key, value)
         
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
        
solve(state)