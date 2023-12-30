package customClass;
import java.util.*;

public class myString {
    private String str;

    public myString(String str) {
        this.str = str;
    }

    public int length() {
        return str.length();
    }

    public String substring(int beginIndex, int endIndex) {
        return str.substring(beginIndex, endIndex);
    }

    public String replace(char oldChar, char newChar) {
        return str.replace(oldChar, newChar);
    }

    public String toUpperCase() {
        return str.toUpperCase();
    }

    public String toLowerCase() {
        return str.toLowerCase();
    }
    
    public boolean isPalindrome() {
        String reversedStr = new StringBuilder(str).reverse().toString();
        return str.equals(reversedStr);
    }

    public void print() {
        System.out.println(str);
    }

    public void printWithString(String s) {
        System.out.println(s + str);
    }
}
