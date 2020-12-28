#!/usr/bin/env python
# coding: utf-8

# In[6]:



    # Finding the manhattan distance
    def manhattan_distance(data_point1, data_point2):
        x1 = float(data_point1.get('TMAX')) + float(data_point1.get('PRCP')) + float(data_point1.get('TMIN'))
        x2 = float(data_point2.get('TMAX')) + float(data_point2.get('PRCP')) + float(data_point2.get('TMIN'))
        return abs(x1-x2)
      
    # Reading the data from a file
    def read_dataset(filename):
        big_dicti = []
        dicti = {}
        count = 0
        key = ''
        
        with open(filename, 'r') as infile: 
            for line in infile: 
                value = line.split() 
                #print(value)
                dicti = {}
                dicti['DATE'] = value[0]
                dicti['TMAX'] = value[1]
                dicti['PRCP'] = value[2]
                dicti['TMIN'] = value[3]
                dicti['RAIN'] = value[4]
                big_dicti.append(dicti)
                
        return big_dicti
     
    # Returns whether or not it will rain based off the neighbors
    def majority_vote(nearest_neighbors):
        rain = 0
        not_rain = 0
        
        for neighbor in nearest_neighbors:
            if neighbor.get('RAIN') == 'TRUE':
                rain = rain + 1
            else:
                not_rain = not_rain + 1
        if rain >= not_rain:
            return 'TRUE'
        else:
            return 'FALSE'
    
    # The 
    def k_nearest_neighbors(filename, test_point, k, year_interval):
        dataset = read_dataset(filename)
        date = test_point.get('DATE').split('-')
        year = date[0]
        #print(year)
    
        # Find the valid neighbors that are within the year interval
        neighbors = []
        for data in dataset:
            possible_date = data.get('DATE').split('-')
            possible_year = possible_date[0]
            if abs(float(possible_year) - float(year)) <= year_interval:
                neighbors.append(data)
        
        # Find the closest k valid neighbors    
        closest = []
        for neighbor in neighbors:
            neighbor['manh'] = manhattan_distance(neighbor, test_point)
            #print(neighbor)
        
        # Sorting the neighbors based off of their calculated manhattan distance
        sort = []
        sort = sorted(neighbors, key = lambda i: i['manh'])
        
        # Organizing the at most k neighbors from the above sorted list.
        found = []
        count = 0
        while len(found) < k:
            found.append(sort[count])
            count = count + 1
        
        # Finding the majority vote based off of those k sorted neighbors
        return majority_vote(found)


# In[ ]:




