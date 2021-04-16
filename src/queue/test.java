package queue;

public class test {

    public static void main(String[] args) {

        mahmoudQueue test = new mahmoudQueue();
        test.EnQueue(10);
        test.DeQueue();
        test.EnQueue(20);
        test.EnQueue(30);
        test.EnQueue(40);
        System.out.println(test.Front());
        test.Show();

    }
}
