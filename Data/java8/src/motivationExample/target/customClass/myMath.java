package customClass;
import java.util.*;

public class myMath{

    public int computeMultiple(int a, int b)
    {
        return a * b;
    }

    public Integer computeMultiple(Integer a, Integer b)
    {
        Integer result = 0;
        for (int i = 0; i < b; ++i)
        {
            result += a;
        }
        return result;
    }
    
    public Integer computePow(Integer a, Integer b)
    {
        Integer result = 1;

        for(int i = 0; i < b; ++i)
        {
            result = computeMultiple(result,a);
        }

        return result;
    }

    public int addTwoValue(int a, int b)
    {
        return a + b;
    }

    public Boolean isPrime(int ub)
    {
        if ( ub <= 1)
        {
            return false;
        }

        for(int i = 2; i < ub; ++i)
        {
            if(ub % i == 0)
                return false;
        }
        return true;
    }

 public List<Integer> primeFactors(int ub)
    {
        List <Integer> prime = new ArrayList<Integer>();

        for(Integer i = 0; i <= ub; ++i)
        {
            if(isPrime(i))
                prime.add(i);
        }

        return prime;
    }
}