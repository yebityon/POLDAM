import customClass.myMath;

public class Main {
    public static void main(String[] args) {
    
        System.out.println("Hello World!!");

        myMath calculator = new myMath();

        System.out.println("Create Math Class Instance");

        Integer a = 2;
        Integer b = 10;

        System.out.println(calculator.addTwoValue(10,2));
        Integer result = calculator.computePow(a,b);
        System.out.println(result);

        if (calculator.isPrime(2))
        {
            System.out.println("2 is prime");
        } 
        else 
        {
            System.out.println("2 is not prime");
        }

        if (calculator.isPrime(10))
        {
            System.out.println("10 is prime");
        } 
        else 
        {
            System.out.println("10 is not prime");
        }

    }   
}
