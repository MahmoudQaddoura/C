package linked_list;

public class mahmoudList {
    Node head;

   private class Node{
       int MainData;
       Node next;

       public Node(int data) {
            MainData = data;

       }
   }

   public void insertAt(int data, int position){
       Node newNode = new Node(data);

       if(position == 0){
           newNode.next = head;
           head = newNode;

       }
       else{
           Node counter = head;

           for (int i = 1; i < position; i++){
               counter = counter.next;
           }

           newNode.next = counter.next;
           counter.next = newNode;
       }
    }

    public void removeAt(int position){
       if(position == 0){
           Node oldHead = head;

           head = head.next;
           oldHead.next = null;
       }
       else{
           Node counter = head;
           for(int i = 1; i < position; i++){
               counter = counter.next;
           }
           Node oldNode = counter.next;
           counter.next = counter.next.next;
           oldNode.next = null;

       }

    }

    public void modify(int position, Node A){
       Node counter = head;

       for (int i = 0; i < position; i++){

       }

    }
    public void read(int position){
       Node counter = head;
       for(int i = 0; i < position; i++){
           counter = counter.next;
       }
       System.out.print("at position " + position + " = " + counter.MainData);
    }
    public void print(){
       Node counter = head;
       System.out.print("[ ");

       while(counter != null){
           System.out.print(counter.next.MainData+", ");
           counter = counter.next;

       }
       System.out.print("[ ");

    }

}