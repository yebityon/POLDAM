import customClass.myMath;

public class Main {
    public static void main(String[] args) {

        System.out.println("Hello World!!");

        myMath calculator = new myMath();

        System.out.println("Create Math Class Instance");

        Integer a = 2;
        Integer b = 30;

        Integer result = calculator.computePow(a,b);
        
        System.out.println(result);
    }   
}