package linked_stack;

public class mahmoudLinkedStack {
    Node head;
    //points at the last node

    private class Node{
        int data;
        //reference to the next node
        Node next;
        //constructor; creates new Node
        Node(int data){
            //this points to the class variable
            this.data = data;
        }
    }

    //adds to the stack
    public void push(int newData){
        //newNode is a pointer variable to the actual node where the data is stored
        Node newNode = new Node(newData);

        //head == null is a condition where the head is not pointing to a Node
        if (head == null){
            //make it point to the newNode created
            head = newNode;
        }
        else{
            //make head point from the previous node to the next Node
            newNode.next = head;
            head = newNode;

        }
    }
    public void pop(){
        if (head == null){
            throw new IllegalAccessError();
        }
        else{
            Node counter = head;
            Node oldNode = counter.next;
            counter.next = counter.next.next;
            oldNode.next = null;
        }

    }

    public Node top(){
        if (head==null){
            throw new IllegalAccessError();
        }
        else{
            return head;
        }

    }

}
