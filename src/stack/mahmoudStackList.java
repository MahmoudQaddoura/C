package stack;
import java.lang.invoke.MethodHandles;

//the class has been given a generic type of T as an argument
// which allows the user to chose the type of data that is going
// to be used when the constructor is called.
// T is now the data type used in the code to be exchanged for specific
// or primitive data types.
// wrapper classes are used as the arguments to the constructor

public class mahmoudStackList<T>{
    /*
    1) push(element value) => insert
    2) pop() => remove
    3) Top() => read the last element pushed
    4) IsEmpty => tells if the stack is empty
     */

    //private implementation details. unnecessary for the user to see
    private T []A;
    int top;
    int maxSize;

    //constructor
    public mahmoudStackList(){
        top = -1;
        maxSize = 10;
        A = (T[]) new Object[maxSize];

    }


    public void Push(T x){
        if (top == maxSize - 1){
            throw new IllegalAccessError();

            //where a dynamic array could be created
        }
        else{

            A[++top] = x;
        }
    }

    public T Pop(){
        if (top == -1){
            throw new IllegalAccessError();
        }
        else{
            return A[top--];
        }
    }

    public T Top() {
        if (top == -1){
            throw new IllegalAccessError();

        }
        else{
            return A[top];
        }
    }

    public boolean IsEmpty(){
        if(top == -1){
            return true;
        }
        else{
            return false;
        }
    }

}
