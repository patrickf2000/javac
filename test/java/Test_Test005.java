public class Test_Test005 {
    public static void addTwo(int x, int y) {
        int answer = x + y;
        System.out.println(answer);
    }

    public static void main(String[] args) {
       System.out.println("Main");
       
       addTwo(20, 30);
       
       int x = 20;
       int y = 33;
       
       addTwo(20, x);
       addTwo(y, 20);
       addTwo(x, y);
    }
}
