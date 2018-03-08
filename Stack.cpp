#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

template<typename T>
class Stack {
private:
    T* items;
    int count;
public:
    Stack() : count(0), items(nullptr) {
    }
    Stack(const Stack& Stack) : count(Stack.count) {
        items = static_cast<T*>(malloc(sizeof(T) * count));
        memcpy(items, Stack.items, sizeof(T) * count);
    }
    Stack(Stack&& Stack) : count{Stack.count}, items(Stack.items) {
        Stack.items = nullptr;
        Stack.count = 0;
    }
    ~Stack() {
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
    T top() {    
        return items[count - 1];
    }
    void pop() {
        if(count == 0) {
            printf("pop error\n");
            return ;
        }
        remove(count - 1);
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
void PrintStack(Stack<T>& s){
      int count=s.length();
      for (int i = 0; i < count; i++)
      {
        cout << s[i] << " ";
      }
      cout << endl;
}

int main() {
    Stack<int> s;
    s.push(3);
    s.push(1);
    s.push(2);
    s.push(4);
    PrintStack(s);

    s.pop();
    s.pop();
    PrintStack(s);
    cout << s.size() << endl;

    cout << s.top() << endl;

    s.clean();
    cout << s.size() << endl;

    return 0;
}           
