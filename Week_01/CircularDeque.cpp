class MyCircularDeque {
private:
    int *deque;         // 用一个数组保存
    int head;           // 头部索引，指向头部元素所在位置
    int end;            // 尾部索引，指向尾部元素下一个位置
    int maxLen;         // 数组最大长度 
public:
    /** Initialize your data structure here. Set the size of the deque to be k. */
    MyCircularDeque(int k) {
        maxLen = k + 1;
        deque = new int[maxLen];
        head = 0;
        end = 0;
    }
    
    /** Adds an item at the front of Deque. Return true if the operation is successful. */
    bool insertFront(int value) {
        if(isFull()) {
            return false;
        }
        head = (head + maxLen - 1) % maxLen;
        deque[head] = value;
        return true;
    }
    
    /** Adds an item at the rear of Deque. Return true if the operation is successful. */
    bool insertLast(int value) {
        if(isFull()) {
            return false;
        }
        deque[end] = value;
        end = (end + 1) % maxLen;
        return true;
    }
    
    /** Deletes an item from the front of Deque. Return true if the operation is successful. */
    bool deleteFront() {
        if(isEmpty()) {
            return false;
        }
        head = (head + 1) % maxLen;
        return true;
    }
    
    /** Deletes an item from the rear of Deque. Return true if the operation is successful. */
    bool deleteLast() {
        if(isEmpty()) {
            return false;
        }
        end = (end + maxLen - 1) % maxLen;
        return true;
    }
    
    /** Get the front item from the deque. */
    int getFront() {
        if(isEmpty()) {
            return -1;
        }
        return deque[head];
    }
    
    /** Get the last item from the deque. */
    int getRear() {
        if(isEmpty()) {
            return -1;
        }
        return deque[(end + maxLen - 1) % maxLen];
    }
    
    /** Checks whether the circular deque is empty or not. */
    bool isEmpty() {
        return (end == head);
    }
    
    /** Checks whether the circular deque is full or not. */
    bool isFull() {
        return ((end + 1) % maxLen == head);
    }
};
