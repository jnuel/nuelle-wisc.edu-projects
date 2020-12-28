import numpy as np
import random
import csv
import copy
# from decimal import Decimal

# Feel free to import other packages, if needed.
# As long as they are supported by CSL machines.


def get_dataset(filename):
    # INPUT: 
    #     filename - a string representing the path to the csv file.

    # RETURNS:
    #     An n by m+1 array, where n is # data points and m is # features.
    #     The labels y should be in the first column.
    # """
    dataset = []
    with open(filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        i = 0
        for row in reader:
            i += 1
            if i == 1:
                pass
            toadd = []
            # "IDNO","BODYFAT","DENSITY","AGE","WEIGHT","HEIGHT","ADIPOSITY","NECK","CHEST","ABDOMEN","HIP","THIGH","KNEE","ANKLE","BICEPS","FOREARM","WRIST"
            toadd.append(float(row['BODYFAT']))
            toadd.append(float(row['DENSITY']))
            toadd.append(float(row['AGE']))
            toadd.append(float(row['WEIGHT']))
            toadd.append(float(row['HEIGHT']))
            toadd.append(float(row['ADIPOSITY']))
            toadd.append(float(row['NECK']))
            toadd.append(float(row['CHEST']))
            toadd.append(float(row['ABDOMEN']))
            toadd.append(float(row['HIP']))
            toadd.append(float(row['THIGH']))
            toadd.append(float(row['KNEE']))
            toadd.append(float(row['ANKLE']))
            toadd.append(float(row['BICEPS']))
            toadd.append(float(row['FOREARM']))
            toadd.append(float(row['WRIST']))
            dataset.append(toadd)
    dataset = np.array(dataset)
    return dataset


def print_stats(dataset, col):
    # INPUT: 
    #     dataset - the body fat n by m+1 array
    #     col     - the index of feature to summarize on. 
    #               For example, 1 refers to density.

    # RETURNS:
    #     None
    # """
    print(len(dataset))
    sum = 0
    the_col = dataset[:,col]
    for i in range(len(dataset)):
        sum += dataset[i][col]
    
    print('{:.2f}'.format(sum/len(dataset)))

    print('{:.2f}'.format(np.std(the_col)))
    return


def regression(dataset, cols, betas):
    # INPUT: 
    #     dataset - the body fat n by m+1 array
    #     cols    - a list of feature indices to learn.
    #               For example, [1,8] refers to density and abdomen.
    #     betas   - a list of elements chosen from [beta0, beta1, ..., betam]

    # RETURNS:
    #     mse of the regression model
    # """

    mse = None
    thesum = 0
    added = 0
    for i in range(len(dataset)):
        thesum = 0
        thesum += betas[0]
        for u in range(len(cols)):
            thesum += betas[u+1] * dataset[:,cols[u]][i]
        thesum -= dataset[:,0][i]
        thesum = (thesum)**2
        added += thesum

    mse = (1/len(dataset))*added
    return mse


def gradient_descent(dataset, cols, betas):
    # INPUT: 
    #     dataset - the body fat n by m+1 array
    #     cols    - a list of feature indices to learn.
    #               For example, [1,8] refers to density and abdomen.
    #     betas   - a list of elements chosen from [beta0, beta1, ..., betam]

    # RETURNS:
    #     An 1D array of gradients
    # """
    grads = []
    thesum = 0
    added = 0
    mse = 0
    n = len(dataset)
    for a in range(len(betas)):
        mse = 0
        thesum = 0
        added = 0
        for i in range(len(dataset)):
            thesum += betas[0]
            # Go through each beta
            for u in range(len(cols)):
                thesum += betas[u+1] * dataset[:,cols[u]][i]
            # Subtract the yi
            thesum -= dataset[:,0][i]
            # If a == 0, no multiplication needed
            if a == 0:
                pass
            else:
                thesum *= dataset[:,cols[a-1]][i]
            added += thesum
            thesum = 0
        mse = (2/n) * added
        grads.append(mse)
    return np.array(grads)



def iterate_gradient(dataset, cols, betas, T, eta):
    # INPUT: 
    #     dataset - the body fat n by m+1 array
    #     cols    - a list of feature indices to learn.
    #               For example, [1,8] refers to density and abdomen.
    #     betas   - a list of elements chosen from [beta0, beta1, ..., betam]
    #     T       - # iterations to run
    #     eta     - learning rate

    # RETURNS:
    #     None
    # """
    # copybetas = copy.copy(betas)
    for i in range(1, T+1):
        prevbetas = copy.copy(betas)
        for u in range(len(betas)):
            betas[u] -= eta * gradient_descent(dataset,cols,prevbetas)[u]
        mse = regression(dataset, cols, betas)

        toPrint = str(i)
        toPrint += ' ' + str('{:.2f}'.format(mse))
        for a in range(len(betas)):
            toPrint += ' ' + str('{:.2f}'.format(betas[a]))
        print(toPrint)
    return
    


def compute_betas(dataset, cols):
    # INPUT: 
    #     dataset - the body fat n by m+1 array
    #     cols    - a list of feature indices to learn.
    #               For example, [1,8] refers to density and abdomen.

    # RETURNS:
    #     A tuple containing corresponding mse and several learned betas
    # """
    betas = None
    mse = None
    y = dataset[:,0] # Bodyfat column
    x = np.ones([len(y), len(cols)+1], dtype = float)  
    for i in range(len(cols)):
        x[:,i+1] = dataset[:,cols[i]]
    # print(x)

    xt = np.transpose(x)
    first = np.dot(xt, x)
    inv = np.linalg.inv(first)
    second = np.dot(xt, y)
    betas = np.dot(inv, second)
    # print(betas)
    betas = np.array(betas)
    mse = regression(dataset, cols, betas)
    return (mse, *betas)


def predict(dataset, cols, features):
    # INPUT: 
    #     dataset - the body fat n by m+1 array
    #     cols    - a list of feature indices to learn.
    #               For example, [1,8] refers to density and abdomen.
    #     features- a list of observed values

    # RETURNS:
    #     The predicted body fat percentage value
    # """
    result = None
    betas = compute_betas(dataset, cols)
    thesum = 0
    thesum += betas[1]
    for u in range(len(features)):
        thesum += betas[u+2] * features[u]
    result = thesum
    return result


def random_index_generator(min_val, max_val, seed=42):
    # """
    # DO NOT MODIFY THIS FUNCTION.
    # DO NOT CHANGE THE SEED.
    # This generator picks a random value between min_val and max_val,
    # seeded by 42.
    # """
    random.seed(seed)
    while True:
        yield random.randrange(min_val, max_val)


def gradient_descent2(dataset, cols, betas):
    # INPUT: 
    #     dataset - the body fat n by m+1 array
    #     cols    - a list of feature indices to learn.
    #               For example, [1,8] refers to density and abdomen.
    #     betas   - a list of elements chosen from [beta0, beta1, ..., betam]

    # RETURNS:
    #     An 1D array of gradients
    # """
    grads = []
    thesum = 0
    added = 0
    mse = 0
    n = len(dataset)
    for a in range(len(betas)):
        mse = 0
        thesum = 0
        added = 0
        # for i in range(len(dataset)):
        thesum += betas[0]
        # print('betas at 0 is' ,betas[0])
        # Go through each beta
        for u in range(len(cols)):
            thesum += betas[u+1] * dataset[cols[u]]
        # Subtract the yi
        thesum -= dataset[0]
        # If a == 0, no multiplication needed
        if a == 0:
            pass
        else:
            thesum *= dataset[cols[a-1]]
        added += thesum
        thesum = 0
        mse = 2 * added
        grads.append(mse)
    return np.array(grads)

def sgd(dataset, cols, betas, T, eta):
    # """
    # TODO: implement this function.
    # You must use random_index_generator() to select individual data points.

    # INPUT: 
    #     dataset - the body fat n by m+1 array
    #     cols    - a list of feature indices to learn.
    #               For example, [1,8] refers to density and abdomen.
    #     betas   - a list of elements chosen from [beta0, beta1, ..., betam]
    #     T       - # iterations to run
    #     eta     - learning rate

    # RETURNS:
    #     None
    # """
    thefeature = random_index_generator(0, len(dataset)-1)
    for i in range(1, T+1):
        nextfeature = next(thefeature)
        # print(nextfeature)
        rand_item = dataset[nextfeature]
        grads = gradient_descent2(rand_item, cols, betas)

        prevbetas = copy.copy(betas)
        for u in range(len(betas)):
            # print(eta * gradient_descent(dataset,cols,betas)[u])
            betas[u] -= eta * grads[u]
        # print(copybetas)
        mse = regression(dataset, cols, betas)

        toPrint = str(i)
        toPrint += ' ' + str('{:.2f}'.format(mse))
        for a in range(len(betas)):
            toPrint += ' ' + str('{:.2f}'.format(betas[a]))
        print(toPrint)
    return


if __name__ == '__main__':
    # Your debugging code goes here.
    dataset = get_dataset('bodyfat.csv')
    print(dataset.shape)
    print(dataset)
    print_stats(dataset, 1)
    print(regression(dataset, cols=[2,3], betas=[0,0,0]))
    print(regression(dataset, cols=[2,3,4], betas=[0,-1.1,-.2,3]))
    print(gradient_descent(dataset, cols=[2,3], betas=[0,0,0]))
    iterate_gradient(dataset, cols=[1,8], betas=[400,-400,300], T=10, eta=1e-4)
    print((1e-4) * gradient_descent(dataset, cols=[1,8], betas=[400,-400,300]))
    print(compute_betas(dataset, cols=[1,2]))
    print(predict(dataset, cols=[1,2], features=[1.0708, 23]))
    sgd(dataset, cols=[2,3], betas=[0,0,0], T=5, eta=1e-6)
    sgd(dataset, cols=[2,8], betas=[-40,0,0.5], T=10, eta=1e-5)
    pass
