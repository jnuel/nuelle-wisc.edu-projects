/**
 * Filename: TestAVLTree.java Project: p2 Authors: Debra Deppeler, Joy Nuelle
 *
 * Semester: Fall 2018 Course: CS400 Lecture: 001
 * 
 * Due Date: Before 10pm on September 24, 2018 Version: 1.0
 * 
 * Credits: none
 * 
 * Bugs: no known bugs
 */

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import java.lang.IllegalArgumentException;
import org.junit.Test;

/**
 * These tests ensure that the AVLTree is functioning properly and tests bounds 
 * of all the methods that are used in AVLTree.java
 * @author JoyNuelle
 *
 */
public class TestAVLTree {

    /**
     * Tests that an AVLTree is empty upon initialization.
     */
    @Test
    public void test01isEmpty() {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        assertTrue(tree.isEmpty());
    }

    /**
     * Tests that an AVLTree is not empty after adding a node.
     */
    @Test
    public void test02isNotEmpty() {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        try {
            tree.insert(1);
            assertFalse(tree.isEmpty());
        } catch (DuplicateKeyException e) {
            System.out.println(e.getMessage());
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Tests functionality of a single delete following several inserts.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test03insertManyDeleteOne() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        try {
            tree.insert(1);
            tree.insert(2);
            tree.insert(3);
            tree.insert(4);
            tree.delete(1);
            assertFalse(tree.search(1));
        } catch (DuplicateKeyException e) {
            System.out.println(e.getMessage());
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Tests functionality of many deletes following several inserts.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test04insertManyDeleteMany()
        throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        try {
            tree.insert(1);
            tree.insert(2);
            tree.insert(3);
            tree.insert(4);
            tree.insert(5);
            tree.delete(1);
            tree.delete(5);
            tree.delete(4);
            assertFalse(tree.search(1));
            assertFalse(tree.search(5));
            assertFalse(tree.search(4));
        } catch (DuplicateKeyException e) {
            System.out.println(e.getMessage());
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Tests that search can't find something that was not inserted into the AVL tree.
     */
    @Test
    public void test05searchEmptyTree() {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        assertFalse(tree.search(1));
    }

    /**
     * Tests that print on an empty tree prints nothing.
     */
    @Test
    public void test06printEmptyTree() {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        assertTrue(tree.print().equals(""));
    }

    /**
     * Tests that the empty tree returns true when it is tested for Balance
     */
    @Test
    public void test07checkBalanceEmptyTree() {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        assertTrue(tree.checkForBalancedTree());
    }

    /**
     * Tests that the empty tree returns true when it is tested if it is a BST
     */
    @Test
    public void test08checkBSTEmptyTree() {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        assertTrue(tree.checkForBinarySearchTree());
    }

    /**
     * Tests when there is one item in the tree, search can find it.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test09searchOneItem() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        try {
            tree.insert(1);
            assertTrue(tree.search(1));
        } catch (DuplicateKeyException e) {
            System.out.println(e.getMessage());
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Tests that print on an AVL tree with one item prints the item and a space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test10printOneItem() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        try {
            tree.insert(1);
            assertTrue(tree.print().equals("1 "));
        } catch (DuplicateKeyException e) {
            System.out.println(e.getMessage());
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Tests that the tree with one item returns true that it is balanced.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test11checkBalanceOneItem() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        try {
            tree.insert(1);
            assertTrue(tree.checkForBalancedTree());
        } catch (DuplicateKeyException e) {
            System.out.println(e.getMessage());
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Tests that a tree with one item returns true that it is a BST.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test12checkBSTOneItem() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        try {
            tree.insert(1);
            assertTrue(tree.checkForBinarySearchTree());
        } catch (DuplicateKeyException e) {
            System.out.println(e.getMessage());
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Tests that when 2 deletes are called after only having one insert, nothing is deleted.
     */
    @Test
    public void test13insertOneDeleteMany() {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        try {
            tree.insert(1);
            tree.delete(1);
            tree.delete(0);
        } catch (DuplicateKeyException e) {
            System.out.println(e.getMessage());
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Tests that when another key is inserted with the same key, a DuplicateKeyException is thrown.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test(expected = DuplicateKeyException.class)
    public void test14insertDuplicate() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(1);
        tree.insert(1);
    }

    /**
     * Tests that inserting a null key throws an IllegalArgumentException.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test(expected = IllegalArgumentException.class)
    public void test15insertNull() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(null);
    }

    /**
     * Tests that searching a null key throws an IllegalArgumentException.
     */
    @Test(expected = IllegalArgumentException.class)
    public void test16searchNull() {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.search(null);
    }

    /**
     * Tests that deleting a null key throws an IllegalArgumentException.
     */
    @Test(expected = IllegalArgumentException.class)
    public void test17deleteNull() {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.delete(null);
    }


    /**
     * Tests that the delete method can properly insert and delete one node.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test18deleteOne() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(1);
        tree.delete(1);
        assertTrue(tree.isEmpty());
    }

    /**
     * Tests that the print method prints a string with one space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test19balance1() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(1);
        tree.insert(2);
        tree.insert(3);
        assertEquals(tree.print(), "1 2 3 ");
    }

    /**
     * Tests that the print method prints a string with one space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test20balance2() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(2);
        tree.insert(4);
        tree.insert(6);
        tree.insert(8);
        tree.insert(10);
        tree.insert(7);
        assertEquals(tree.print(), "2 4 6 7 8 10 ");
    }

    /**
     * Tests that the print method prints a string with one space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test21balance3() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.insert(40);
        tree.insert(50);
        tree.insert(60);
        tree.insert(70);
        tree.insert(80);

        assertEquals(tree.print(), "10 20 30 40 50 60 70 80 ");
    }

    /**
     * Tests that the print method prints a string with one space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test22balancee4() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.insert(40);
        tree.insert(50);
        tree.insert(60);
        tree.insert(70);
        tree.insert(55);

        assertEquals(tree.print(), "10 20 30 40 50 55 60 70 ");
    }

    /**
     * Tests that the print method prints a string with one space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test23balance5() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.insert(40);
        tree.insert(50);
        tree.insert(60);
        tree.insert(70);
        tree.insert(65);

        assertEquals(tree.print(), "10 20 30 40 50 60 65 70 ");
    }

    /**
     * Tests that the print method prints a string with one space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test24balance6() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.insert(40);
        tree.insert(50);
        tree.insert(60);
        tree.insert(70);
        tree.insert(15);

        assertEquals(tree.print(), "10 15 20 30 40 50 60 70 ");
    }

    /**
     * Tests that the print method prints a string with one space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test25balance7() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.insert(40);
        tree.insert(50);
        tree.insert(60);
        tree.insert(70);
        tree.insert(25);

        assertEquals(tree.print(), "10 20 25 30 40 50 60 70 ");
    }

    /**
     * Tests that the print method prints a string with one space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test26balance8() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.insert(40);
        tree.insert(50);
        tree.insert(60);
        tree.insert(70);
        tree.insert(45);

        assertEquals(tree.print(), "10 20 30 40 45 50 60 70 ");
    }

    /**
     * Tests that the print method prints a string with one space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test27balance9() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.insert(40);
        tree.insert(50);
        tree.insert(60);
        tree.insert(70);
        tree.insert(35);

        assertEquals(tree.print(), "10 20 30 35 40 50 60 70 ");
    }

    /**
     * Tests that the print method prints a string with one space.
     * 
     * @throws DuplicateKeyException
     * @throws IllegalArgumentException
     */
    @Test
    public void test28balance10() throws IllegalArgumentException, DuplicateKeyException {
        AVLTree<Integer> tree = new AVLTree<Integer>();
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.insert(40);
        tree.insert(50);
        tree.insert(60);
        tree.insert(70);
        tree.insert(5);

        assertEquals(tree.print(), "5 10 20 30 40 50 60 70 ");
    }



}
