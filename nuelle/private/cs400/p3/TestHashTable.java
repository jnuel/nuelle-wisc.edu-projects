/**
 * Filename:   TestHashTable.java
 * Project:    p3
 * Authors:    TODO: add your name(s) and lecture numbers here
 *
 * Semester:   Fall 2018
 * Course:     CS400
 * 
 * Due Date:   TODO: add assignment due date and time
 * Version:    1.0
 * 
 * Credits:    TODO: name individuals and sources outside of course staff
 * 
 * Bugs:       TODO: add any known bugs, or unsolved problems here
 */

import java.util.NoSuchElementException; // expect to need
import static org.junit.Assert.*; 
import org.junit.Before;  // setUp method
import org.junit.After;   // tearDown method
import org.junit.Test;   

/** TODO: add class header comments here*/
public class TestHashTable{

	// TODO: add other fields that will be used by multiple tests
	
	// Allows us to create a new hash table before each test
	static HashTableADT<Integer, Object> ht;   
	static HashTableADT<Integer, Object> ht1;
	static HashTableADT<Integer, Object> ht2;
    static HashTableADT<Integer, Object> ht3;



	
	
	// TODO: add code that runs before each test method
	@Before
	public void setUp() throws Exception {
		ht = new HashTable<Integer, Object>(); 
	    ht1 = new HashTable<Integer, Object>(); 
	    ht2 = new HashTable<Integer, Object>(); 
	    ht3 = new HashTable<Integer, Object>(20, 1.0);  



	}

	// TODO: add code that runs after each test method
	@After
	public void tearDown() throws Exception {
		ht = null;
		ht1 = null;
		ht2 = null;
		ht3 = null;
	}
		
	/** IMPLEMENTED AS EXAMPLE FOR YOU
	 * Tests that a HashTable is empty upon initialization
	 */
	@Test
	public void test000_IsEmpty() {
		assertEquals("size with 0 entries:", 0, ht.size());
	}
	
	/** IMPLEMENTED AS EXAMPLE FOR YOU
	 * Tests that a HashTable is not empty after adding one (K, V) pair
	 */
	@Test
	public void test001_IsNotEmpty() {
		ht.put(1,"0001");
		int expected = 1;
		int actual = ht.size();
		assertEquals("size with one entry:",expected,actual);
	}

	/** IMPLEMENTED AS EXAMPLE FOR YOU
	 * Other tests assume <int,Object> pairs,
	 * this test checks that <Long,Object> pair also works.
	 */
	@Test
	public void test010_Long_Object() {
		Long key = 9876543210L;
		Object expected = "" + key;		
		HashTableADT<Long,Object> table = 
				new HashTable<Long,Object>();
		table.put(key, expected);
		Object actual = table.get(key);
		assertTrue("put-get of (Long,Object) pair",
				expected.equals(actual));
	}
	
	/*
	 * Tests that the value for a key is updated 
	 * when tried to insert again.
	 */
	@Test
	public void test011_Update() {
	    ht.put(1,"0001");
	    ht.put(1, "1000");
        Object expected = "1000";
        Object actual = ht.get(1);
        assertEquals("size with one entry:",expected,actual);
	}
	
	/*
	 * Tests that inserting many and removing one entry
	 * from the hash table works
	 */
	@Test(timeout=1000 * 10)
	public void test100_InsertManyRemoveOne() {
	    for (int i = 0; i < 500; i ++) {
	        ht1.put(i, i);
	    }
	    ht1.remove(1);
	    int expected = 499;
        int actual = ht1.size();
        assertEquals("500 nodes entered then one removed entry:",expected,actual);
	}
	
	/*
	 * Tests ability to insert many entries and 
	 * and remove many entries from the hash table
	 */
	@Test(timeout=1000 * 10)
	public void test110_InsertRemoveMany() {
	       for (int i = 0; i < 100; i ++) {
	            ht2.put(i, i);
	        }
	       for (int i = 0; i < 50; i++) {
	           ht2.remove(i);
	       }
	       int expected = 50;
	        int actual = ht2.size();
	        assertEquals("100 nodes entered then one removed entry:",expected,actual);
	}
	
	/*
     * Tests ability to insert several entries when 
     * given an inital capacity and load factor of 1.0
     */
    @Test(timeout=1000 * 10)
    public void test7_hashTableResize() {
           for (int i = 0; i < 21; i ++) {
                ht2.put(i, i);
            }
           int expected = 21;
            int actual = ht2.size();
            assertEquals("21 nodes entered:",expected,actual);
    }
    
    /*
     * Tests ability to insert a null key
     * 
     */    
    @Test(expected = IllegalArgumentException.class)
    public void test8_insertNullKey() {
           ht.put(null,  null);
    }
    
    /*
     * Tests ability to insert a null value
     * 
     */    
    @Test
    public void test9_insertNullValue() {
           ht.put(1,  null);
           Object expected = null;
           Object actual = ht.get(1);
           assertEquals("Inserted the key 1 with value null",expected,actual);

    }
    
    /*
     * Tests ability to get a null key
     * 
     */    
    @Test (expected = IllegalArgumentException.class)
    public void test10_getNullKey() {
           ht.get(null);
    }
    
    /*
     * Tests ability to get a key that isn't in the hashTable
     * 
     */    
    @Test (expected = NoSuchElementException.class)
    public void test11_getMissingKey() {
           ht.get(10000);
    }
    
    /*
     * Tests ability to remove a null key
     * 
     */    
    @Test (expected = IllegalArgumentException.class)
    public void test12_removeNullKey() {
           ht.remove(null);
    }
    
    /*
     * Tests ability to remove a key that isn't in the hashTable
     * 
     */    
    @Test (expected = NoSuchElementException.class)
    public void test13_removeMissingKey() {
           ht.remove(10000);
    }
}

