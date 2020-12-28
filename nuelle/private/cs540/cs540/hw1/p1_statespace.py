#!/usr/bin/env python
# coding: utf-8

# In[4]:



    #Each of these functions should take as an argument the current state of the two water jugs as a list of two integers as well as the maximum capacities of each jug as a list of two integers.
    
    jugs = [6, 4]
    max = [10, 10]
    
    #fill(state max which) — returns a copy of state which fills the jug corresponding to the index in which (0 or 1) to its maximum capacity. Do not modify state.
    def fill(state, max, which):
        copy = state.copy()
        copy[which] = max[which]
        return copy
    
    #empty(state max which) — returns a copy of state which empties the jug corresponding to the index in which (0 or 1). Do not modify state.
    def empty(state, max, which):
        to_empty = state.copy()
        to_empty[which] = 0
        return to_empty
    
    #xfer(state max source dest) — returns a copy of state which pours the contents of the jug at index source into the jug at index dest until source is empty or dest is full. Do not modify state. xfer is shorthand for transfer.
    def xfer(state, max, source, dest):
        xfer_copy = state.copy()
        if xfer_copy[source] + xfer_copy[dest] > max[dest]:
            how_much = max[dest] - xfer_copy[dest]
            xfer_copy[dest] = max[dest]
            xfer_copy[source] = xfer_copy[source] - how_much
        else:
            # < or = to
            xfer_copy[dest] = xfer_copy[source] + xfer_copy[dest]
            xfer_copy[source] = 0
        return xfer_copy
          
    #succ(state max) — prints the list of unique successor states of the current state in any order. This function will generate the unique successor states of the current state by applying fill empty xfer operations on the current state. (Note: You have to apply an operation at every step for generating a successor state.)
    def succ(state, max):
        succs = []
        succs.append(empty(jugs, max, 0))
        if (empty(jugs, max, 1) not in succs):
            succs.append(empty(jugs, max, 1))
        if (empty(jugs, max, 0) not in succs):
            succs.append(empty(jugs, max, 0))
        if (xfer(jugs, max, 0, 1) not in succs):
            succs.append(xfer(jugs, max, 0, 1))
        if (xfer(jugs, max, 1, 0) not in succs):
            succs.append(xfer(jugs, max, 1, 0))
        if (fill(jugs, max, 0) not in succs):
            succs.append(fill(jugs, max, 0))
        if (fill(jugs, max, 1) not in succs):
            succs.append(fill(jugs, max, 1))
        print(succs)


# In[ ]:




