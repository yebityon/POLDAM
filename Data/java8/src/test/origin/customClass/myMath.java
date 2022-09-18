package customClass;

public class myMath{

    public int computeMultiple(int a, int b)
    {
        return a * b;
    }

    public Integer computeMultiple(Integer a, Integer b)
    {
        return a * b;
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

    
}