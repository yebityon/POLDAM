import customClass.myMath;
import customClass.myString;
import java.util.*;

public class Main {
    public static void main(String[] args) {

        myMath calculator = new myMath();
        int num = 10;
        List<Integer> p = calculator.sieveOfEratosthenes(num);
        List<Integer> primes = calculator.primeFactors(num);

        myString str = new myString("Sample Program is end");
        myString output = new myString("is primeNumber");
        
        for(int idx = 0; idx < primes.size(); ++idx)
        {
            String prime = Integer.toString(primes.get(idx));
            output.printWithString(prime);
        }
        
        myString neko = new myString("Sample Program is end");

        str.print();
    }
}
