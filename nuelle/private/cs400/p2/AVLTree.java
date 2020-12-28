/**
 * Filename:   AVLTree.java
 * Project:    p2
 * Authors:    Debra Deppeler, Joy Nuelle
 *
 * Semester:   Fall 2018
 * Course:     CS400
 * Lecture:    001
 * 
 * Due Date:   Before 10pm on September 24, 2018
 * Version:    1.0
 * 
 * Credits:    none
 * 
 * Bugs:       no known bugs
 *  */

import java.lang.IllegalArgumentException;

/** Simulates an AVLTree that can hold comparable objects of generic type K.
 * @param <K> a comparable object
 */
public class AVLTree<K extends Comparable<K>> implements AVLTreeADT<K> {
    private BSTNode<K> root;

    public static void main(String [] args) throws IllegalArgumentException, DuplicateKeyException {

        AVLTree tree = new AVLTree();

        tree.insert(1);
        tree.insert(1);
    }
    /** 
     * Represents a tree node.
     * @param <K> the comparable object that is to be stores in the tree.
     */
    class BSTNode<K> {
        /* fields */
        private K key;	// the label associated with each node
        private int height;	// number of nodes on the maximum path from root to leaf
        private BSTNode<K> left, right;	// the associated left and ride nodes(children) of the root or main node. 
        /**
         * Constructor for a BST node that assigns the key and makes the left and right nodes null. 
         * also assigns height for the node.
         * @param key
         */
        BSTNode(K key) {
            this.key = key;
            this.left = null;
            this.right = null;
            this.setHeight(1); //a new node will always have height 1.
        }

        /**
         * returns the key of the desired node
         * @return key
         */
        public K getKey() {
            return key;
        }

        /**
         * returns the height of the desired node
         * @return height
         */
        public int getHeight() {
            return height;
        }

        /**
         * returns the left child node of the parent.
         * @return left
         */
        public BSTNode<K> getLeft() {
            return left;
        }

        /**
         * returns the right child node of the parent
         * @return right
         */
        public BSTNode<K> getRight() {
            return right;
        }

        /**
         * returns whether or not the parent has a left child
         * @return true if the left child is not null
         */
        public boolean hasLeft() {
            if(this.left == null) {
                return false;
            }
            else return true;
        }

        /**
         * returns whether or not the parent has a right child
         * @return true if the right child is not null
         */
        public boolean hasRight() {
            if(this.right== null) {
                return false;
            }
            else return true;
        }

        /**
         * sets the height of the node
         * @param height
         */
        public void setHeight(int height) {
            this.height = height;
        }

        /**
         * assigns the left node to the node passed in
         * @param n the node to be assigned
         */
        public void setLeft(BSTNode<K> n) {
            left = n;
        }

        /**
         * assigns the right node to the node passed in
         * @param n the node to be assigned
         */
        public void setRight(BSTNode<K> n) {
            right = n;
        }

        /**
         * assigns the key that is passed in to the left node's key
         * @param key
         */
        public void setLeftKey(K key) {
            left.setKey(key);
        }

        /**
         * assigns the key that is passed in to the right node's key
         * @param key
         */
        public void setRightKey(K key) {
            right.setKey(key);
        }

        /**
         * assigns the key that is passed in to the current node's key
         * @param key
         */
        public void setKey(K newK) {
            key = newK;
        }

    }

    /**
     * Tests whether or not the tree is empty.
     * @return true if the root is not null, meaning it is not empty.
     */
    @Override
    public boolean isEmpty() {
        if ( root == null) {
            return true;
        }
        else return false;
    }

    /**
     * Inserts a node like a normal BST and then balances it by calculating the balance factor
     * and seeing if rotations are necessary. 
     */
    @Override
    public void insert(K key) throws DuplicateKeyException, IllegalArgumentException {
        root = insert(root, key);
    }

    /**
     * A helper recursive method to insert the node into the tree. 
     * @param n
     * @param key
     * @return the node that is at the root of the tree
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    private BSTNode<K> insert(BSTNode<K> n,K key) throws DuplicateKeyException, IllegalArgumentException {
        if (key == null) {
            throw new IllegalArgumentException();
        }

        if (n == null) {
            n = new BSTNode<K>(key);
            return n;
        }

        if (n.getKey().equals(key)) {
            throw new DuplicateKeyException();
        }

        if (key.compareTo(n.getKey()) < 0) {
            n.setLeft(insert(n.getLeft(), key));
            // add key to left subtree
        }

        else {
            //add key to right subtree
            n.setRight(insert(n.getRight(), key));
        }

        n.setHeight(1 + max(height(n.getLeft()), height(n.getRight())));
        //have to balance the tree to maintain AVL
        int balanceFactor = getBalanceFactor(n);
        n = balance(n, balanceFactor);
        return n;
    }

    /**
     * A helper method.
     * Tests for the maximum height amongst two nodes.
     * @param leftHeight
     * @param rightHeight
     * @return the left height if left height is greater than right, and vice versa.
     */
    private int max(int leftHeight, int rightHeight) {
        if (leftHeight > rightHeight) {
            return leftHeight;
        }
        else if (rightHeight > leftHeight) {
            return rightHeight;
        }
        else return leftHeight;
    }
    /**
     * A helper method.
     * Returns the height of the node if it isn't null to begin with.
     * @param n
     * @return the height of the node, or 0 if the node is null.
     */
    private int height (BSTNode<K> n) {
        if (n == null) {
            return 0;
        }
        else return n.getHeight();
    }

    /**
     * A helper method.
     * Recursively calculates the balance factor of the node(the height of the left - the height of the right)
     * @param n
     * @return height of the left minus height of the right.
     */
    private int getBalanceFactor(BSTNode<K> n) {
        if (n == null) {
            return 0;
        }
        int countLeft = height(n.getLeft());
        int countRight = height(n.getRight());
        return countLeft-countRight;
    }

    /**
     * A helper method.
     * Appropriately rotates the Tree to fit AVL requirements. 
     * @param n
     * @param balanceFactor
     * @return the new parent node
     */
    private BSTNode<K> balance(BSTNode<K> n, int balanceFactor) {
        //left left case
        if (balanceFactor > 1 && root.getKey().compareTo(n.getLeft().getKey()) > 0) {
            return rightRotate(n);
        }
        //right right case
        else if (balanceFactor < -1 && root.getKey().compareTo(n.getRight().getKey()) < 0) {
            return leftRotate(n);
        }
        //left right case
        else if (balanceFactor > 1 && root.getKey().compareTo(n.getLeft().getKey()) < 0) {
            n.setLeft(leftRotate(n.getLeft()));
            return rightRotate(n);
        }
        // right left case
        else if (balanceFactor < -1 && root.getKey().compareTo(n.getRight().getKey()) > 0) {
            n.setRight(rightRotate(n.getRight()));
            return leftRotate(n);
        }
        return n;
    }

    /**
     * A helper method to rotate the set of nodes to the right. 
     * @param a the parent node that is the "Grandparent" of the rotation
     * @return the new parent node
     */
    private BSTNode<K> rightRotate(BSTNode<K> a) {
        BSTNode<K> b = a.getLeft();
        BSTNode<K> c = b.getRight();

        b.setRight(a);
        a.setLeft(c);

        b.setHeight(1 + max(height(a.getLeft()), height(a.getRight())));
        a.setHeight(1 + max(height(b.getLeft()), height(b.getRight())));
        return b;
    }

    /**
     * A helper method to rotate the set of nodes to the left.
     * @param a the parent node that is the "Grandparent" of the rotation
     * @return the new parent node
     */
    private BSTNode<K> leftRotate(BSTNode<K> a) {
        BSTNode<K> b = a.getRight();
        BSTNode<K> c = b.getLeft();

        b.setLeft(a);
        a.setRight(c);

        a.setHeight(1 + max(height(a.getLeft()), height(a.getRight())));
        b.setHeight(1 + max(height(b.getLeft()), height(b.getRight())));
        return b;
    }

    /**
     * Deleted the node with the current key. 
     */
    @Override
    public void delete(K key)  {
        root = delete(root, key);
    }

    /**
     * A recursive helper method to find the node that matches the key,
     * and if no key is found that matches then we return the tree unchanged.
     * @param n
     * @param key
     * @return the new tree
     */
    private BSTNode<K> delete(BSTNode<K> n, K key)  {
        if (key == null) {
            throw new IllegalArgumentException();
        }
        if (n == null) {
            return null;
        }

        if (key.equals(n.getKey())) {
            // n is the node to be removed
            if (n.getLeft() == null && n.getRight() == null) {
                return null;
            }
            else if (n.getLeft() == null) {
                return n.getRight();
            }
            else if (n.getRight() == null) {
                return n.getLeft();
            }

            // if we get here, then n has 2 children
            K smallVal = smallest(n.getRight());
            n.setKey(smallVal);
            n.setRight( delete(n.getRight(), smallVal) );
        }

        //go left
        else if (key.compareTo(n.getKey()) < 0) {
            n.setLeft(delete(n.getLeft(), key));
        }

        //go right
        else if (key.compareTo(n.getKey()) > 0) {
            n.setRight( delete(n.getRight(), key) );
        }

        root.setHeight(1 + max(height(root.getLeft()), height(root.getRight())));
        //have to balance the tree again!
        int balanceFactor = getBalanceFactor(n);
        n = balance(n, balanceFactor);
        return n;
    }

    /**
     * A helper method to get the smallest node on the left, our go-to node when
     * deleting with 2 children. 
     * @param n
     * @return the smallest node in the left side of each tree. 
     */
    private K smallest(BSTNode<K> n) {
        if (n.getLeft() == null) {
            return n.getKey();
        }
        else {
            return smallest(n.getLeft());

        }
    }

    /**
     * Returns whether or not the key desired is in the tree.
     * @return true if the key is in the tree, false if not.
     */
    @Override
    public boolean search(K key) {
        return search(root, key);
    }

    /**
     * A recursive helper method to search for the key in the tree. 
     * @param n
     * @param key
     * @return true if the key is found, false if not.
     */
    private boolean search(BSTNode<K> n, K key) {
        if (key == null) {
            throw new IllegalArgumentException();
        }
        if (n == null) {
            return false;
        }

        if (n.getKey().equals(key)) {
            return true;
        }

        if (key.compareTo(n.getKey()) < 0) {
            // key < this node's key; look in left subtree
            return search(n.getLeft(), key);
        }
        else {
            // key > this node's key; look in right subtree
            return search(n.getRight(), key);
        }
    }

    /**
     * Prints the AVL tree in inorder. (so in ascending order)
     */
    @Override
    public String print() {
        return inOrder(root, "");
    }

    /**
     * A recursive helper method to build the string that contains all the nodes in the tree in inorder. 
     * @param n
     * @param build
     * @return the string with the nodes in ascending order
     */
    private String inOrder(BSTNode<K> n, String build) {
        if (n == null) {
            return "";
        }
        return build = (inOrder(n.getLeft(), build) + n.getKey() + " " + inOrder(n.getRight(), build));
    }

    /**
     * This method checks to see if the AVL Tree is height balanced.
     * @return true if it is height balanced, false if not. 
     */
    @Override
    public boolean checkForBalancedTree() {
        return recursiveCheck(root, false);
    }

    /**
     * A recursive helper method to check if the tree is height balanced. 
     * @param n
     * @param check
     * @return true if the method is height balanced, false if otherwise. 
     */
    private boolean recursiveCheck(BSTNode n, boolean check) {
        int balanceFactor = getBalanceFactor(n);
        if (balanceFactor < 1 && balanceFactor > -1) {
            check = true;
        }
        else if (n.hasLeft()) {
            check = recursiveCheck(n.getLeft(), check);
        }
        else if (n.hasRight()) {
            check = recursiveCheck(n.getRight(), check);
        }
        return check;
    }

    /**
     * A method to check if the tree is a binary search tree. 
     * @return true if it is a binary search tree, false if not.
     */
    @Override
    public boolean checkForBinarySearchTree() {
        if (checkForBinarySearchTree(root)) {
            return true;
        }
        else return false;
    }

    /**
     * A recursive helper method to check each node and make sure the order of the nodes and associated
     * keys are in the right order. If they are, then it is a BST.
     * @param n
     * @return true if the keys are all in correct order, or if the root is null meaning the tree is empty.
     */
    private boolean checkForBinarySearchTree(BSTNode<K> n) {
        if (n == null) {
            return true;
        }
        else if (n.getLeft() == null || n.getRight() == null) {
            return true;
        }

        else if ((n.getKey().compareTo(n.getLeft().getKey()) > 0) || n.getKey().compareTo(n.getRight().getKey()) < 0) {
            return false;
        }
        return (checkForBinarySearchTree(n.getLeft()) && checkForBinarySearchTree(n.getRight()));
    }
}
