/**
 * Filename:   HashTable.java
 * Project:    p3
 * Authors:    Joy Nuelle
 *
 * Semester:   Fall 2018
 * Course:     CS400
 * 
 * Due Date:   10/29/18 Before 10:00 pm
 * Version:    1.0
 * 
 * Credits:    
 * 
 * Bugs:       no known bugs
 */


import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import java.util.NoSuchElementException;

// TODO: comment and complete your HashTableADT implementation
// DO NOT ADD PUBLIC MEMBERS TO YOUR CLASS
//
// TODO: describe the collision resolution scheme you have chosen
// identify your scheme as open addressing or bucket
// 
// if open addressing: describe probe sequence 
// if buckets: describe data structure for each bucket
//
// TODO: explain your hashing algorithm here 
// NOTE: you are not required to design your own algorithm for hashing,
//       you may use the hashCode provided by the <K key> object
//       
public class HashTable<K extends Comparable<K>, V> implements HashTableADT<K, V> {
    private double loadFactor;
    private Node<K, V> hashTable[];
    private Node<K, V> oldTable[];
    private int size;
    // TODO: ADD and comment DATA FIELD MEMBERS needed for your implementation

    // TODO: comment and complete a default no-arg constructor
    public HashTable() {
        hashTable = (Node<K, V>[]) new Node<?,?> [11];
        this.loadFactor = 0.75;
        this.size = 0;

    }

    // TODO: comment and complete a constructor that accepts initial capacity and load factor
    public HashTable(int initialCapacity, double loadFactor) {
        hashTable = (Node[]) new Node<?,?> [initialCapacity * 2];
        this.size = 0;
        this.loadFactor = loadFactor;
    }

    static class Node <KK, VV> {
        private VV value;
        private Node<KK, VV> next;
        private KK key;

        public Node(KK key, VV value) {
            this.key = key;
            this.value = value;
            this.next = null;
        }

        public Node(KK key, VV value, Node<KK, VV> next) {
            this.key = key;
            this.value = value;
            this.next = next;
        }

        public VV getValue() {
            return this.value;
        }

        public void setData(VV value) {
            this.value = value;
        }

        public KK getKey() {
            return this.key;
        }
        public Node<KK, VV> getNext() {
            return this.next;
        }

        public void setNext(Node<KK, VV> next) {
            this.next = next;
        }
    }

    public Node<K, V> lookup(K key) {
        Node<K, V> toReturn = null;
        int value = key.hashCode() % hashTable.length;
        if (hashTable[value] != null) { // there is a linked node there
            if (hashTable[value].getKey().equals(key)) { //that node matches the key
                return hashTable[value];
            }
            else {
                Node<K, V> current = hashTable[value];
                while ( current != null) {
                    if (current.getKey() == key)
                        toReturn = current;
                    current = current.getNext();
                }
            }
        }
        return toReturn;  
    }

    public void resize() {
        int newCode;
        if ((size() / hashTable.length) > this.loadFactor) {
            Node<K, V> hashTable1[] = (Node[]) new Node<?,?> [(hashTable.length * 2) + 1];
            for (int i = 0; i < hashTable.length; i++) {
                if (hashTable[i] != null) {
                    newCode = hashTable[i].getKey().hashCode() % hashTable.length;
                    while (hashTable[i].getNext() != null) { // while you're in the old hashtable, if there is "next" node keep rehashing and fixing
                        if (hashTable1[newCode] != null) { // there is already something in the new hashtable
                            Node<K, V> found = hashTable1[newCode]; //store that in a temporary node
                            hashTable1[newCode] = hashTable[i]; // set the new node to the front, and the appropriate next node
                            hashTable1[newCode].setNext(found);
                        }
                        else {
                            hashTable1[newCode] = hashTable[i];
                        }
                    }
                    hashTable1[newCode] = hashTable[i]; // if there is no "next" node in the old hashtable, just move it over!

                }
            }
            hashTable = hashTable1;
        }
    }
    // TODO: comment and complete this method
    @Override
    public void put(K key, V value) throws IllegalArgumentException {
        if (key == null) {
            throw new IllegalArgumentException();
        }
        resize(); // check for resize;
        int code = key.hashCode() % hashTable.length;
        Node<K, V> found = lookup(key);
        if (found != null && found.getKey() == key) {  //that node matches the key, double check
            found.setData(value); //replaces the node's value 
        }
        else if (found != null && found.getKey() != key) {
            Node<K, V> temp = hashTable[code];
            hashTable[code] = new Node<K, V> (key, value, temp);
            size++;
        }
        else if (found == null) {
            Node<K, V> temp = hashTable[code];
            hashTable[code] = new Node<K, V> (key, value, temp);
            size++;
        }
    }

    // TODO: comment and complete this method
    @Override
    public V get(K key) throws IllegalArgumentException, NoSuchElementException {
        if (key == null) {
            throw new IllegalArgumentException();
        }
        Node<K, V> node= lookup(key);
        if (node != null) {
            return node.getValue();
        }
        else throw new NoSuchElementException();
    }

    // TODO: comment and complete this method
    @Override
    public void remove(K key) throws IllegalArgumentException, NoSuchElementException {
        if (key == null) {
            throw new IllegalArgumentException();
        }
        int value = key.hashCode() % hashTable.length;
        if (hashTable[value] != null) { // there is a linked node there
            if (hashTable[value].getKey().equals(key)) { //that node matches the key
                if (hashTable[value].getNext() != null) {
                    hashTable[value].setNext(hashTable[value].getNext());
                }
                else {
                    hashTable[value] = null;
                }
            }
        }
        else throw new NoSuchElementException();
    }

    // TODO: comment and complete this method
    @Override
    public int size() {
        int count = 0;
        for (int i = 0; i < hashTable.length; i++) {
            if (hashTable[i] != null) {
                while (hashTable[i].getNext() != null) {
                    count++;
                }
                count++;
            }
        }
        return count;
    }

}
