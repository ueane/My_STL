#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

template<typename T>
class Queue {
private:
    T* items;
    int count;
public:
    Queue() : count(0), items(nullptr) {
    }
    Queue(const Queue& Queue) : count(Queue.count) {
        items = static_cast<T*>(malloc(sizeof(T) * count));
        memcpy(items, Queue.items, sizeof(T) * count);
    }
    Queue(Queue&& Queue) : count{Queue.count}, items(Queue.items) {
        Queue.items = nullptr;
        Queue.count = 0;
    }
    ~Queue() {
        for(int i = 0; i < this->count; i++) {
            this->items[i].~T();
        }
        free(this->items);
        this->items = nullptr;
        this->count = 0;
    }
    T& operator[](int index) {
        if(index < 0 || index >= count) {
            cout << "invalid index, the first is" << endl;
            return items[0];
        }
        return items[index];
    }
    int length() {
        return count;
    }
    int size() {
        return count;
    }
    void clean() {
        for(int i = 0; i < this->count; i++) {
            this->items[i].~T();
        }
        free(this->items);
        this->items = nullptr;
        this->count = 0;
    }
    void push(const T& item) {
        T *new_items = (T *)malloc(sizeof(T) * this->count + 1); //BUG
        for(int i = 0; i < this->count; i++) {
            new(new_items + i) T(std::move(this->items[i]));
        }
        new(new_items + this->count) T(item);
        int pre = this->count;
        this->clean();
        this->count = pre + 1;
        this->items = new_items;
    }
    T front() {
        return items[0];
    }
    void pop() {
        if(count == 0) {
            printf("pop error\n");
            return ;
        }
        remove(0);
    }
    int empty() {
        return count == 0;
    }
    bool insert(const T& item,int index) {
        if(this->count < index || index < 0) return false;
        T *new_items = (T *)malloc(sizeof(T) * (this->count + 1)); //BUG
        for(int i  = 0; i < index; i++) {
            new(new_items + i) T(std::move(this->items[i]));
        }
        new(new_items + index) T(item);
        for(int i  = index + 1; i <= this->count; i++) {
            new(new_items + i) T(std::move(this->items[i - 1]));
        }
        
        int pre = this->count;
        this->clean();
        this->count = pre + 1;
        this->items = new_items;
        return true;
    }
    bool remove(int index) {
        if(index < 0 || index >= this->count) return false;
        T *new_items = (T *)malloc(sizeof(T) * (this->count - 1)); //BUG
        for(int i = 0; i < index; i++) {
            new(new_items + i) T(std::move(this->items[i]));
        }
        
        for(int i  = index + 1; i < this->count; i++) {
            new(new_items + i - 1) T(std::move(this->items[i]));
        }
        
        int pre = this->count;
        this->clean();
        this->count = pre - 1;
        this->items = new_items;
        
        return true;
    }
    int find(const T& item) {
        for(int i = 0; i < this->count; i++){
            if(this->items[i] == item) {
                return i;
            }
        }
        return -1;
    }
};

template<typename T>
void PrintQueue(Queue<T>& q){
      int count=q.length();
      for (int i = 0; i < count; i++)
      {
        cout << q[i] << " ";
      }
      cout << endl;
}

int main() {
    Queue<int> q;
    q.push(3);
    q.push(1);
    q.push(2);
    q.push(4);
    PrintQueue(q);

    q.pop();
    q.pop();
    PrintQueue(q);
    cout << q.length() << endl;
    cout << q[1] << endl;
    q.clean();
    cout << q.length() << endl;

    return 0;
}
