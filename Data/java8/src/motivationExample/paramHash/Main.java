import customClass.myString;
import java.util.*;

public class Main {
    public static void main(String[] args) {

        int num = 10;
        myString str = new myString("Same Hash");
        myString neko = new myString("Same Hash");
        myString inu = new myString("Different Hash");
        str.print();
        neko.print();
        inu.print();
    }
}
