#!/usr/bin/env python
# coding: utf-8

# In[18]:


import os 
import numpy as np

def create_bow(vocab, filepath):
    """ Create a single dictionary for the data
        Note: label may be None
    """
    bow = {}
    # This function takes a path to a text file (assume a valid format, one token per line), reads the file in, 
    f = open(filepath, "r")
    lines = f.readlines()
    for line in lines:
        line = line.strip();
#         print(line)
        # creates a bag-of-words representation based on the vocabulary, 
        if line in vocab:
            if line in bow:
                bow.update({line: bow[line] + 1})
#                 bow[line] += 1
            else:
                bow[line] = 1
        elif None in bow:
            # Give all counts of word types not in the vocabulary to out of vocabulary (OOV)
#             bow[None] += 1
            bow.update({None: bow[None] + 1})
        else:
            bow[None] = 1
#         print(bow)
    # and returns the bag-of-words in dictionary format.   
    return bow

def load_training_data(vocab, directory):
    """ Create the list of dictionaries """
    dataset = []
    # load the entire contents of the training directory into such Python dictionaries, 
    for folder in os.listdir(directory):
#         print(folder)
        if not folder.startswith('.'):
            for filename in os.listdir(os.path.join(directory,folder)):
#             print(filename)
#             f = open(os.path.join(directory,folder,filename), "r")
                bow = create_bow(vocab, os.path.join(directory,folder,filename))
                pair = {}
                # label them with their corresponding subdirectory label ('2016' or '2020' as strings) 
                pair['label'] = folder
                pair['bow'] = bow
                dataset.append(pair)
    # return them in a list of length n=number of training documents.
#     print(dataset)
    return dataset

def create_vocabulary(directory, cutoff):
    """ Create a vocabulary from the training directory
        return a sorted vocabulary list
    """

    vocab = []
    count_vocab = {}
    
    # TODO: add your code here
    # To create the vocabulary for your classifier, 
    # traverse BOTH of these subdirectories under train/ (note: do not include test/) 
    for folder in os.listdir(directory):
#         print(folder)
        if not folder.startswith('.'): 
            for filename in os.listdir(os.path.join(directory,folder)):
#             print(filename)

                f = open(os.path.join(directory,folder,filename), "r")
                lines = f.readlines()
                # and count the number of times a word type appears in any file in either directory.
                for line in lines:
                    line = line.strip();
                    if line not in count_vocab:
                        count_vocab[line] = 1
                    else:
                        count_vocab[line] += 1
    # As a design choice, we will exclude any word types which appear at a frequency strictly less than the cutoff argument 
    # (cutoff = 1 means retain all word types you encounter).
#     print(count_vocab)  
    for each_word in count_vocab:
        if count_vocab[each_word] >= cutoff:
            vocab.append(each_word)
#     print(vocab)

    # Return a sorted list of these word types.
    vocab = sorted(vocab)
    
    return vocab

def prior(training_data, label_list):
    """ return the prior probability of the label in the training set
        => frequency of DOCUMENTS
    """
    # This method should return the log probability- logP(label) of the labels in the training set. 
    # In order to calculate these, you will need to count the number of documents with each label in the training data, found in the training/ subdirectory.

    smooth = 1 # smoothing factor
    logprob = {}
    num_label = {}
    # TODO: add your code here
    for each in training_data:
        if each['label'] in num_label:
            num_label[each['label']] += 1
        else:
            num_label[each['label']] = 1
#     print(num_label)
    total_files = len(training_data)
    for each in label_list:
        logprob[each] = np.log((num_label[each] + 1) / (total_files + 2))
    return logprob

def p_word_given_label(vocab, training_data, label):
    """ return the class conditional probability of label over all words, with smoothing """
    
    smooth = 1 # smoothing factor
    word_prob = {}
    
#     print(vocab)
#     print(training_data)
    not_in_words = 0
    total_words = 0
    # For each vocab word wih the label
    # For each vocab word, count how many times it appears in each BOW with the right label
    for each_vocab in vocab:
        word_prob[each_vocab] = 0
#     print(word_prob)
    for each_train in training_data:
        if each_train['label'] == label:
#             print("The bag of words", each_train['bow'])
            for each_word in each_train['bow']:
#                 print("current word:", each_word)
                if each_word in vocab:
                    word_prob[each_word] += each_train['bow'][each_word]
                elif each_word == None:
#                     print("in none")
                    if None in word_prob:
                        word_prob[None] += each_train['bow'][each_word]
                    else:
                        word_prob[None] = each_train['bow'][each_word]
                else:
                    not_in_words += 1
#     print("word prob", word_prob)
#     print("not in words", not_in_words)
    if None in word_prob:
        word_prob[None] += not_in_words
    else:
        word_prob[None] = not_in_words
        
#     print("word prob", word_prob)
    
    for each in word_prob:
        total_words += word_prob[each]
        
#     print('total words(actual length of corpus):', total_words, 'lenvocab:', len(vocab))
        
    for each in word_prob:
#         print('each', each, 'word_prob[each] + 1:',word_prob[each] + 1, '/ length of the corpus:', len(training_data),'len(vocab):',   len(vocab))
        word_prob[each] = np.log((word_prob[each] + 1) / (total_words + len(vocab) + 1))

    return word_prob

    
##################################################################################
def train(training_directory, cutoff):
    """ return a dictionary formatted as follows:
            {
             'vocabulary': <the training set vocabulary>,
             'log prior': <the output of prior()>,
             'log p(w|y=2016)': <the output of p_word_given_label() for 2016>,
             'log p(w|y=2020)': <the output of p_word_given_label() for 2020>
            }
    """
    retval = {}
    vocab = create_vocabulary(training_directory, cutoff)
    retval['vocabulary'] = vocab
    
    training_data = load_training_data(vocab, training_directory)
    the_prior = prior(training_data, ['2020', '2016'])
    retval['log prior'] = the_prior
    retval['log p(w|y=2020)'] = p_word_given_label(vocab, training_data, '2020')
    retval['log p(w|y=2016)'] = p_word_given_label(vocab, training_data, '2016')

    return retval


def classify(model, filepath):
    """ return a dictionary formatted as follows:
            {
             'predicted y': <'2016' or '2020'>, 
             'log p(y=2016|x)': <log probability of 2016 label for the document>, 
             'log p(y=2020|x)': <log probability of 2020 label for the document>
            }
    """
    retval = {}
#     print(model)
    log_prior_2016 = model['log prior']['2016']
    log_prior_2020 = model['log prior']['2020']
    total_2020_p = 0
    total_2016_p = 0
    bow = create_bow(model['vocabulary'], filepath)
    
    for each in bow:
        total_2020_p += (model['log p(w|y=2020)'][each])*bow[each]
        total_2016_p += (model['log p(w|y=2016)'][each])*bow[each]
    
    total_2020_p += log_prior_2020
    total_2016_p += log_prior_2016
    
    retval['log p(y=2020|x)'] = total_2020_p
    retval['log p(y=2016|x)'] = total_2016_p
    if total_2020_p > total_2016_p:
        retval['predicted y'] = '2020'
    else:
        retval['predicted y'] = '2016'

    return retval


# In[ ]:




