package queue;

/*
EnQueue(x) -->  insert element at the rear
DeQueue() --> remove element from the front
front() --> shows front element
IsEmpty() --> shows if empty
*/

public class mahmoudQueue {

    private int maxSize;
    private int rear;
    private int front;
    private int[] queue;

    //constructors are functions used to
    // initialize values fof an object's properties
    public mahmoudQueue(){
        maxSize = 4;
        front = -1;
        rear = -1;

        queue = new int[maxSize];
    }

    public boolean IsEmpty(){
     if (rear == -1 && front == -1)
         return true;
     else
         return false;

    }

    public void EnQueue(int data) {
        if ((rear + 1 )% maxSize == front){
            throw new IllegalAccessError();
        }

        if (IsEmpty()) {
            rear++;
            front++;
        }
       else {
            rear = (rear + 1) % maxSize;
        }

       //this will enter the data into the rear index
       queue[rear] = data;
    }

    public void DeQueue() {
        if(IsEmpty()){
            throw new IllegalAccessError();
        }

        if(front == rear && rear != -1){
            front = -1;
            rear = -1;
        }
        else{
            front = (front+1) % maxSize;
        }

    }

    public int Front(){
        if (IsEmpty() == true){
            throw new IllegalAccessError();
        }
        else{
            return queue[front];
        }

    }
    public void Show(){
        System.out.print("elements: ");
        for (int i = 0; i<=front ; i++) {
            System.out.print(queue[i] + " ");

        }
    }

}
