/** Test Program Source File
 * Used in demostration of Secure Copy
 * from CS file system to student's local matchine.
 * @author: deppeler
 * @version: 1.0 Fall 2018
 */

/** Print Hello World to the default output stream. */
public class HelloWorld {

  /** Says hellow to each command line argument.
   * @param args command line argument strings
   */
  public static void main (String [] args) {
    for (String arg : args) 
      System.out.println("Hello " + arg + "!");
    System.out.println("Hello World!");
  }
}
