package dynamic_array;

import java.lang.invoke.MethodHandles;

public class dynamicArray {

    int []list;
    int end;
    int maxSize;

    private void createList(){

        maxSize = 10;
        list = new int[maxSize];
        end = -1;
    }

    /* /////////////////////////////////////////////////////////// */

    private void insertList(int element){

        end++;
        list[end] = element;

        if(end == list.length){

            int[] temp = new int[maxSize*2];
            for (int i = 0; i<= end; i++){
                temp[i] = list[i];

            }
            list = temp;
        }
    }

    /* ////////////////////////////////////////////////////////// */

    private void insertListAt(int element, int position){

        for (int i=end ; i >=position ; i-- ){

            list[i + 1] = list[i];

        }

        end++;
        list[position] = element;

    }

    /* ///////////////////////////////////////////////////////// */

    private void printList(int list[]){
        System.out.print("[");
        for(int i = 0 ; i < end ; i++){
            System.out.print(list[i] + ",");
        }
        System.out.print("]");

    }

    /* ///////////////////////////////////////////////////////// */

    private void removeAt(int position){

        for(int i = position; i < end; i++){

            list[i] = list[i+1];
        }
        end--;
    }

    /* ///////////////////////////////////////////////////////// */

    private int count(){

        return end+1;
    }

    /* ///////////////////////////////////////////////////////// */

    private void modify(int newVal, int position){

        list[position] = newVal;

    }

    /* ///////////////////////////////////////////////////////// */

    private int readAt(int p){

        if(p > end){
            throw new IllegalAccessError();
        }

        return list[p];
    }

}
