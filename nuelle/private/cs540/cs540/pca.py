from scipy.linalg import eigh  
import numpy as np
import matplotlib.pyplot as plt 
import matplotlib
matplotlib.use('Agg')


# load the dataset from a provided .npy file, re-center it around the origin and return it as a NumPy array of floats
def load_and_center_dataset(filename):
    x = np.load(filename) 
    x = np.reshape(x,(2000,784))
    x = np.array(x)
    return(x-np.mean(x, axis=0))

# — calculate and return the covariance matrix of the dataset as a NumPy matrix (d x d array)
def get_covariance(dataset): 
    x = np.array(dataset)
    xt = np.transpose(x)
    calculated = (1/(len(x)-1)) * np.dot(xt, x)
    return(calculated)

#— perform eigen decomposition on the covariance matrix S and return a diagonal matrix (NumPy array) with the largest m eigenvalues on the diagonal, and a matrix (NumPy array) with the corresponding eigenvectors as columns
def get_eig(S, m):
    x = np.array(S)
    w, v = eigh(x, eigvals=(len(x) - m, len(x)-1))

    revw = list(reversed(w))
    revw = np.diag(revw)

    return revw, v

#— similar to get_eig, but instead of returning the first m, return all eigenvectors that explains more than perc % of variance
def get_eig_perc(S, perc):
    x = np.array(S)
    w, v = eigh(x, eigvals=(0, len(x)-1))
    total = np.sum(w)
    index = 0
    newv = []
    totalv = 0
    for index in range(len(v)):
        if w[index]/total > perc:
            w = w[index:len(w)]
            v = v[:,[index, len(v)-1]]
            break
        index+=1

    w = list(reversed(w))
    w = np.diag(w)
    
    return w, v

#— project each image into your m-dimensional space and return the new representation as a d x 1 NumPy array
def project_image(image, U):
    d = len(image) #784 x 1 
    m = len(U[0]) # number of eigenvalues
    u = np.array(U)

    # print(len(newu[:,0]), np.shape(newu[:,0]))
    
    j = 0
    xproj = 0
    for j in range(m):
        aij = np.dot(np.transpose(u[:,j]), image)
        xproj += aij*u[:,j]
 
    return xproj

#— use matplotlib to display a visual representation of the original image and the projected image side-by-side
def display_image(orig, proj): 
    image = np.reshape(orig,(28,28))
    image_p = np.reshape(proj,(28,28))

    fig, axs = plt.subplots(1, 2, figsize=(9,3))
    axs[0].set_title('Original')
    axs[1].set_title('Projection')
    pos = axs[0].imshow(image, aspect='equal', cmap='gray')
    fig.colorbar(pos, ax=axs[0])

    pos2 = axs[1].imshow(image_p, aspect='equal', cmap='gray')
    fig.colorbar(pos2, ax=axs[1])
    # plt.show()
    plt.savefig("file.jpg")
    return 