#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

template<typename T>
class Vector {
private:
  	T* items;
  	int count;
public:
  	Vector() :count{ 0 }, items{nullptr} {

  	}
  	Vector(const Vector& vector) :count{vector.count} {
  		items = static_cast<T*>(malloc(sizeof(T) * count));
  		memcpy(items, vector.items, sizeof(T) * count);
  	}
  	Vector(Vector&& vector) :count{ vector.count }, items{ vector.items } {
  		vector.items = nullptr;
        vector.count = 0;
        
  	}
  	~Vector() {
  		for(int i = 0; i < this->count; i++){
            this->items[i].~T();
        }
        free(this->items);
        this->items = nullptr;
        this->count = 0;
  	}
    T& operator[](int index){
    	if (index<0||index>=count) {
    		cout<<"invalid index"<<endl;
    		return items[0];
    	}
    	return items[index];
    }
    int length(){
    	return count;
    }
  	void clean() {
  		for(int i = 0; i < this->count; i++){
            this->items[i].~T();
        }
        free(this->items);
        this->items = nullptr;
        this->count = 0;
  	}
  	void push_back(const T& item) {
  		T *new_items = (T *)malloc(sizeof(T) * this->count + 1); //BUG
        for(int i = 0; i < this->count; i++) {
            new(new_items + i) T(std::move(this->items[i]));
        }
        new(new_items + this->count) T(item);
        int pre = this->count;
        this->clean();
        //this->count++; //和T的移动构造函数实现又关系, 正常是单纯的内存拷贝,new可以掉构造函数, malloc不可以, delete可以调用析构函数,但是delete申请 
  	    this->count = pre + 1;
        this->items = new_items;
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
        //this->count++; //和T的移动构造函数实现又关系, 正常是单纯的内存拷贝,new可以掉构造函数, malloc不可以, delete可以调用析构函数,但是delete申请 
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
        //this->count++; //和T的移动构造函数实现又关系, 正常是单纯的内存拷贝,new可以掉构造函数, malloc不可以, delete可以调用析构函数,但是delete申请 
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
void PrintVector(Vector<T>& v){
	  int count=v.length();
	  for (int i = 0; i < count; i++)
	  {
        cout << v[i] << " ";
	  }
	  cout << endl;
}

int main() {
    Vector<int> v;
    for(int i = 0; i < 4; i++) {
        v.push_back(i);
    }
    PrintVector(v);
    if(!v.insert(4, 10)) {
        v.insert(4, 2);
    }
    PrintVector(v);
    if (!v.remove(10)) {
        v.remove(2);
    }
    PrintVector(v);
    cout << v.find(100) << endl;

    Vector<int> v2(v);
    Vector<int> v3(move(v2));

    PrintVector(v3);
    v2.push_back(1);
    PrintVector(v2);
    //PrintVector(v2);

  	return 0;
}
