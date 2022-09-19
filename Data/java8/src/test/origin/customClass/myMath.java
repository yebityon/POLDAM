package customClass;

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
        for(int i = 2; i * i <= ub; ++i)
        {
            if(ub % i == 0)
                return false;
        }
        return true;
    }

    // public List<int> primFactors(int ub)
    // {
    //     List <int> prime = new ArrayList<int>();

    //     for(int i = 3; i <= ub; ++i)
    //     {
    //         if(isPrime(i))
    //             prime.aappend(i);
    //     }

    //     return prime;
    // }
}