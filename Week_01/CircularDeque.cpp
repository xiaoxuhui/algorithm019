class MyCircularDeque {
private:
    int *szDeque;// 用一个数组表示
    int head;// 队头下标
    int end;// 队尾下标
    int size;//队列最大长度
public:
    /** Initialize your data structure here. Set the size of the deque to be k. */
    MyCircularDeque(int k) {
        size = k + 1;
        szDeque = new int[k+1];
        // for(int i = 0;i<size;++i) {
        //     szDeque[i] = -1;
        // }
        head = 0;
        end = 0;
    }

    // ~MyCircularDeque() {
    //     delete[] szDeque；
    // }
    
    /** Adds an item at the front of Deque. Return true if the operation is successful. */
    bool insertFront(int value) {
        if(isFull()){
            return false;
        }
        head = (head + size - 1) % size;
        szDeque[head] = value;
        return true;
    }
    
    /** Adds an item at the rear of Deque. Return true if the operation is successful. */
    bool insertLast(int value) {
        if(isFull()) {
            return false;
        }
        szDeque[end]=value;
        end = (end + size + 1) % size;
        return true;
    }
    
    /** Deletes an item from the front of Deque. Return true if the operation is successful. */
    bool deleteFront() {
        if(isEmpty()) {
            return false;
        }
        //szDeque[head]=-1;
        head = (head + 1) % size;
        return true;
    }
    
    /** Deletes an item from the rear of Deque. Return true if the operation is successful. */
    bool deleteLast() {
        if(isEmpty()) {
            return false;
        }
        //szDeque[end]=-1;
        end = (end + size - 1) % size;
        return true;
    }
    
    /** Get the front item from the deque. */
    int getFront() {
        if(isEmpty()) {
            return -1;
        }
        return szDeque[head];
    }
    
    /** Get the last item from the deque. */
    int getRear() {
        if(isEmpty()) {
            return -1;
        }
        return szDeque[(end+size-1)%size];
    }
    
    /** Checks whether the circular deque is empty or not. */
    bool isEmpty() {
        return (head==end);
    }
    
    /** Checks whether the circular deque is full or not. */
    bool isFull() {
        return ((end+1)%size) == (head);
    }
};
