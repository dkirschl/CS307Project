/**
 * Auto Generated Java Class.
 */
public class Dylanget {
  
  public static void main()
  {
    int number;
    number = 1;
    char[] a = new char[4];
    a =  number; 
    String a = "ISVALID" + a + "abcdefghijkabcdefghijkabcdefghijkabcdefghijkabcdefghijkabcdefghijkabcdefghijkabcdefghijkabcdefghijkabcdefghijkabcdefghijk                           asdfasdfasdfasdfasdfqasdfasdfasdfasdfasdfasdfasdfasdfadfasdfadsfadsfasdfasdfasdfasdfasdfasdf                       ";
    String table[];
    returnGet(a,table);
  }
  
  public static void returnGet(String input, String table[])
{
  int a,counter;
  
  a = (int) input[8];
  
  for(counter = 0; counter < a * 2; counter= counter+2)
  {
    table[counter] = input.substring(12 + 53*counter,112 + 53*counter);
    table[counter+1] = input.substring(113 + 53 * counter,118 + 53 * counter);
  }

 
  for(counter = 0; counter < a*2; counter = counter +2)
  {
   table[counter   ] = getSmall(table[counter]);
   table[counter +1] = getSmall(table[counter+1]);
  }
  /* ADD YOUR CODE HERE */
  
}

public static String getSmall(String input)
{
  int counter;
  for(counter = 0; counter < input.length(); counter++)
  {
    if(input.charAt(counter) == ' ')
    {
      if(checkAll(input, counter) == true)
      {
        return input.substring(0,counter);
      }
    }
  }
}

public static boolean checkAll(String input, int a)
{
  int b;
  b = a;
  for(b = b; b < input.length(); b++)
  {
    if(input.charAt(b) != ' ')
    {
      return false;
    }
  }
  return true;
}

}