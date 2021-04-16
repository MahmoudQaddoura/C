package stack;

public class test {
    public static void main(String[] args) {
        mahmoudStackList<Integer> test = new mahmoudStackList();

        test.Push(10);
        test.Push(40);
        int read = test.Top();
        System.out.println(read);
        test.Pop();
        read = test.Top();
        System.out.print(read);

    }
}
