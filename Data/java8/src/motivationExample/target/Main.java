import customClass.myMath;
import java.util.*;

public class Main {
    public static void main(String[] args) {

        myMath calculator = new myMath();

        int num = 10;

        List<Integer> primes = calculator.primeFactors(num);

        for(int idx = 0; idx < primes.size(); ++idx)
        {
            String prime = Integer.toString(primes.get(idx));
            System.out.println(prime + " is" + "primeNumber");
        }
        System.out.println("Done!!");
    }   
}
