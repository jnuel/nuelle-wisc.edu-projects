import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import numpy as np 

def get_dataset(training=True):
    # Input: an optional boolean argument (default value is True for training dataset)
    # Return: two NumPy arrays for the train_images and train_labels
    mnist = keras.datasets.mnist
    (train_images, train_labels), (test_images, test_labels) = mnist.load_data()
    if training==True:
        return np.array(train_images), np.array(train_labels)
    else:
        return np.array(test_images), np.array(test_labels)

def print_stats(train_images, train_labels):
    #  — This function will print several statistics about the data
    # Input: the dataset and labels produced by the previous function; does not return anything
    class_names = ['Zero', 'One', 'Two', 'Three', 'Four', 'Five', 'Six', 'Seven', 'Eight', 'Nine']
    print(len(train_images))
    image = train_images[0]
    toprint = str(len(image)) + 'x' + str(len(image[0]))
    print(toprint)
    for i in range(0,10):
        count = 0
        for u in range(0, len(train_labels)):
            if train_labels[u] == i:
                count += 1
        print(str(i) + '. ' + str(class_names[i])+ ' - ' + str(count))

def build_model():
    #  — takes no arguments and returns an untrained neural network model
    model = keras.Sequential()
    model.add(tf.keras.layers.Flatten(input_shape=(28,28)))
    model.add(tf.keras.layers.Dense(128, activation='relu'))
    model.add(tf.keras.layers.Dense(64, activation='relu'))
    model.add(tf.keras.layers.Dense(10))
    opt = tf.keras.optimizers.SGD(learning_rate=0.001)
    loss_fn = keras.losses.SparseCategoricalCrossentropy(from_logits=True)

    model.compile(loss=loss_fn, optimizer=opt, metrics=['accuracy'])
    return model
    
def train_model(model, train_images, train_labels, T):
    # — takes the model produced by the previous function 
    # and the dataset and labels produced by the first function 
    # and trains the data for T epochs; does not return anything
    model.fit(train_images, train_labels, epochs=T)

def evaluate_model(model, test_images, test_labels, show_loss=True):
    #  — takes the trained model produced by the previous function and the test image/labels, 
    # and prints the evaluation statistics as described below (displaying the loss metric value 
    # if and only if the optional parameter has not been set to False); does not return anything
    test_loss, test_accuracy = model.evaluate(test_images, test_labels, verbose = 0)
    test_accuracy *= 100
    if show_loss:
        print('Loss: ' + '{:.4f}'.format(test_loss))
        print('Accuracy: ' + '{:.2f}'.format(test_accuracy) + '%')
    else:
        print('Accuracy: ' + '{:.2f}'.format(test_accuracy) + '%')

def predict_label(model, test_images, index):
    #  — takes the trained model and test images, 
    # and prints the top 3 most likely labels for the image at the given index, 
    # along with their probabilities; does not return anything
    class_names = ['Zero', 'One', 'Two', 'Three', 'Four', 'Five', 'Six', 'Seven', 'Eight', 'Nine']
    predictions = model.predict(test_images)
    # # print(model.predict(test_images))
    # print(predictions[index])
    # keyvalue = {}
    for i in range(len(predictions[index])):
        predictions[index][i] *= 100
    #     keyvalue[class_names[i]] = '{:.2f}'.format(predictions[index][i]) + '%'
    #     # print('{:.2f}'.format(predictions[index][i]) + '%') 
    # a = sorted(keyvalue.items(), key=lambda x: x[1], reverse=True)
    # print(a)
    arr = np.array(predictions[index])
    top3 = arr.argsort()[-3:][::-1]
    # print(top3)
    print(str(class_names[top3[0]]) + ': ' + '{:.2f}'.format(predictions[index][top3[0]]) + '%') 
    print(str(class_names[top3[1]]) + ': ' + '{:.2f}'.format(predictions[index][top3[1]]) + '%') 
    print(str(class_names[top3[2]]) + ': ' + '{:.2f}'.format(predictions[index][top3[2]]) + '%') 
     


if __name__=="__main__":
    ## Test print label
    # (train_images, train_labels) = get_dataset()
    # (test_images, test_labels) = get_dataset(False)
    # model = build_model()
    # train_model(model, train_images, train_labels, 10)
    # evaluate_model(model, test_images, test_labels)
    # model.add(tf.keras.layers.Softmax())
    # predict_label(model, test_images, 1)

    # (train_images, train_labels) = get_dataset()
    # (test_images, test_labels) = get_dataset(False)
    # # print_stats(train_images, train_labels)
    # print_stats(test_images, test_labels)
    # model = build_model()
    # # print(model)
    # # print(model.loss)
    # # print(model.optimizer)
    # train_model(model, train_images, train_labels, 10)
    # # evaluate_model(model, test_images, test_labels, show_loss=False)
    # # evaluate_model(model, test_images, test_labels, show_loss=False)
    # evaluate_model(model, test_images, test_labels)
    # model.add(tf.keras.layers.Softmax())

    # # train_model(model, train_images, train_labels, 10)
    # predict_label(model, test_images, 1)
    pass