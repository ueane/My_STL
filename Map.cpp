#include <iostream>
#include <memory.h>
using namespace std;
/*
template<typename T1, typename T2>
class my_map;
template<typename T2>
int _compare(string a, string b) {
    return memcmp(&a, &b, a.length());
}*/

template<typename T1, typename T2>
class my_map {
private :
    struct RBTNode {
        pair<T1, T2> key_value;
        int black;
        RBTNode *lchild, *rchild;
        RBTNode *father;
    };
    RBTNode *root;
    RBTNode *NIL;
    
    RBTNode *copy_my_map(RBTNode *node, RBTNode *NILL){                        //拷贝
        if (node == NULL) return NULL;
        if (node == NILL) return this->NIL;
        RBTNode *node2 = init(node->key_value);
        node2->black = node->black;  
        node2->lchild = copy_my_map(node->lchild, NILL);
        node2->lchild->father = node2;
        node2->rchild = copy_my_map(node->rchild, NILL);
        node2->rchild->father = node2;
        return node2;
    }
    RBTNode *init_NIL() {
        RBTNode *NIL = (RBTNode *)malloc(sizeof(RBTNode));
        NIL->father = NULL;
        //NIL->lchild = NIL->rchild = NIL;
        //NIL->key_value = {0, 0};
        NIL->black = 1;
        return NIL;
    }
    RBTNode *init(pair<T1, T2> key_value) {
        RBTNode *p = (RBTNode *)malloc(sizeof(RBTNode));
        p->father = NULL;
        p->lchild = p->rchild = NIL;
        p->key_value.first = key_value.first;
        if (p->key_value.second != key_value.second) { 
            (p->key_value.second) = key_value.second;
        }
        p->black = 0;
        return p;
    }
    void clear(RBTNode *node) {
        if (node == NULL || node == NIL) return ;
        clear(node->lchild);
        clear(node->rchild);
        free(node);
        return ;
    }
    void clear_NIL() {
        free(NIL);
        NIL = NULL;
    }
/*-------------------------------------辅助--------------------------------------*/
    int has_red_child(RBTNode *node) {                                       //是否红孩
        return node->lchild->black == 0 || node->rchild->black == 0;
    }
    RBTNode *left_rotate(RBTNode *node) {                                 //左旋
        RBTNode *temp = node->rchild;
        temp->father = node->father;
        node->father = temp;
        node->lchild->father = node;
        node->rchild = temp->lchild;
        temp->lchild = node;
        return temp;
    }
    RBTNode *right_rotate(RBTNode *node) {                                //右旋
        RBTNode *temp = node->lchild;
        temp->father = node->father;
        node->father = temp;
        node->rchild->father = node;
        node->lchild = temp->rchild;
        temp->rchild = node;
        return temp;
    }
/*---------------------------------------插入-----------------------------------------*/
    RBTNode *insert_maintain(RBTNode *node) {                           //插入调整
        if (node->black == 0) return node;
        RBTNode *lchild = node->lchild;
        RBTNode *rchild = node->rchild;
        if (lchild->black && rchild->black) return node;
        if (!has_red_child(lchild) && !has_red_child(rchild)) return node;
        if (lchild->black == 0 && rchild->black == 0) {
            lchild->black = rchild->black = 1;
            node->black = 0;
            return node;
        }
        if (lchild->black == 0 && lchild->lchild->black == 0) {
            //LL
            node = right_rotate(node);
            node->lchild->black = 1;
        } else if (lchild->black == 0 && lchild->rchild->black == 0) {
            //LR
            node->lchild = left_rotate(node->lchild);
            node = right_rotate(node);
            node->lchild->black = 1;
        } else if (rchild->black == 0 && rchild->rchild->black == 0) {
            //RR
            node = left_rotate(node);
            node->rchild->black = 1;
        } else if (rchild->black == 0 && rchild->lchild->black == 0) {
            //RL
            node->rchild = right_rotate(node->rchild);
            node = left_rotate(node);
            node->rchild->black = 1;
        }
        return node;
    }
    RBTNode *__insert(RBTNode *node, pair<T1, T2> &key_value) {       //插入
        if (node == NULL || node == NIL) {
            RBTNode *temp = init(key_value);
            temp->black = (node == NULL);
            return temp;
        }
        if (node->key_value.first == key_value.first) return node;
        if (memcmp(&node->key_value.first, &key_value.first, sizeof(T1)) < 0) {
            node->rchild = __insert(node->rchild, key_value);
            node->rchild->father = node;
            node = insert_maintain(node);
        } else {
            node->lchild = __insert(node->lchild, key_value);
            node->lchild->father = node;
            node = insert_maintain(node);
        }
        if (node->father == NULL) {
            node->black = 1;
        }
        return node;
    }
/*---------------------------------------------查找--------------------------------------------*/
    RBTNode *search(RBTNode *node, const T1& key) {
        if (node == NULL || node == NIL) return NULL;
        if (sizeof(T1) == 32 && node->key_value.first == key) return node;
        if (!memcmp(&node->key_value.first, &key, sizeof(T1))) return node;
        if (memcmp(&node->key_value.first, &key, sizeof(T1)) < 0) {
            return search(node->rchild, key);
        } else {
            return search(node->lchild, key);
        }
    }
/*---------------------------------------------删除---------------------------------------------*/
    RBTNode *preecessor(RBTNode *node) {                              //前趋
        RBTNode *temp = node->lchild;
        while (temp->rchild != NIL) node = node->rchild;
        return temp;
    }
    RBTNode *erase_maintain(RBTNode *node) {                            //删除调整
        if (node->lchild->black != 2 && node->rchild->black != 2) {
            return node;
        }
        RBTNode *lchild = node->lchild;
        RBTNode *rchild = node->rchild;
        if (lchild->black == 2 && rchild->black == 0) {
            node = left_rotate(node);
            node->black = 1;
            node->lchild->black = 0;
            node->lchild = erase_maintain(node->lchild);
        } else if (rchild->black == 2 && lchild->black == 0) {
            node = right_rotate(node);
            node->black = 1;
            node->rchild->black = 0;
            node->rchild = erase_maintain(node->rchild);
        } else if (lchild->black == 2 && rchild->black == 1) {
            if (!has_red_child(rchild)) {
                rchild->black = 0;
                lchild->black = 1;
                node->black++;
                return node;
            } else if (rchild->rchild->black != 0) {
                node->rchild = right_rotate(node->rchild);
                node->rchild->black = 1;
                node->rchild->rchild->black = 0;
            }
            node = left_rotate(node);
            node->black = node->lchild->black;
            node->lchild->black = node->rchild->black = 1;
            node->lchild->lchild->black = 1;
        } else if (rchild->black == 2 && lchild->black == 1) {
            if (!has_red_child(lchild)) {
                lchild->black = 0;
                rchild->black = 1;
                node->black++;
                return node;
            } else if (lchild->lchild->black != 0) {
                node->lchild = left_rotate(node->lchild);
                node->lchild->black = 1;
                node->lchild->lchild->black = 0;
            }
            node = right_rotate(node);
            node->black = node->rchild->black;
            node->lchild->black = node->rchild->black = 1;
            node->rchild->rchild->black = 1;
        }
        return node;
    }
    int _compare(T1 a, T1 b) {
        return memcmp(&a, &b, sizeof(T1));
    }
    /*int _compare(string a, string b) {
        return memcmp(&a, &b, a.length());
    }*/
    RBTNode *erase(RBTNode *node, T1 key) {                           //删除
        if (node == NULL || node == NIL) return node;
        if (node->key_value.first == key) {
            if (node->lchild == NIL && node->rchild == NIL) {
                NIL->black += node->black;
                free(node);
                return NIL;
            } else if (node->lchild == NIL || node->rchild == NIL) {
                RBTNode *temp = (node->lchild == NIL ? node->rchild : node->lchild);
                temp->black += node->black;
                temp->father = node->father;
                free(node);
                return temp;
            } else {
                RBTNode *temp = preecessor(node);
                pair<T1, T2> tt = temp->key_value;
                temp->key_value = node->key_value;
                node->key_value = tt;
                /*
                temp->key ^= node->key;
                node->key ^= temp->key;
                temp->key ^= node->key;*/
                node->lchild = erase(node->lchild, key);
                node = erase_maintain(node);
            }
        }
        if (_compare(node->key_value.first, key) < 0) {
            node->rchild = erase(node->rchild, key);
            node = erase_maintain(node);
        } else if (_compare(node->key_value.first, key) > 0) {
            node->lchild = erase(node->lchild, key);
            node = erase_maintain(node);
        }
        if (node->father == NULL) {
            node->black = 1;
        }
        return node;
    }
/*-------------------------------------------输出---------------------------------*/
    void inorder(RBTNode *node) {
        if (node == NULL || node == NIL) return ;
        inorder(node->lchild);
        cout << "key = " << node->key_value.first << ", value = " << node->key_value.second << ", color = " << node->black;
        cout << ", lchild = " << node->lchild->key_value.first << ", rchild = " << node->rchild->key_value.first << endl; 
        inorder(node->rchild);
        return ;
    }
public :
    my_map() : root(NULL) {
        this->NIL = init_NIL();
        this->NIL->lchild = this->NIL->rchild = this->NIL;
    }
    my_map(const my_map &mp) : root(NULL) {
        this->NIL = init_NIL();
        this->NIL->lchild = this->NIL->rchild = this->NIL;
        this->root = copy_my_map(mp.root, mp.NIL);
    }
    my_map(my_map &&mp) : root(mp.root), NIL(mp.NIL) {
        mp.root = NULL;
        mp.NIL = NULL;
    }
    ~my_map() {
        clear(this->root);
        clear_NIL();
    }

    T2& operator[](const T1& key) {
        pair<T1, T2> pp;
        pp.first = key;
        this->root = __insert(this->root, pp);
        RBTNode *temp = search(this->root, key);
        return temp->key_value.second;
    }
    const my_map& operator=(const my_map& mmp) {
        this->~my_map();
        my_map mp(mmp);
        this->NIL = mp.NIL;
        this->root = mp.root;
        mp.NIL = NULL;
        mp.root = NULL;
        return *this;
    }

    bool empty() {
        if (this->root == NULL || this->root == this->NIL) return 1;
        return 0;
    }
    void insert(pair<T1, T2> key_value) {
        this->root = __insert(this->root, key_value);
        return ;
    }
    bool find(const T1& key) {
        RBTNode *temp = search(this->root, key);
        if (temp == NULL) return 0;
        return 1;
    }
    void pop(T1 key) {
        this->root = erase(this->root, key);
        return ;
    }
    void output() {
        inorder(this->root);
        return ;
    }
};



int main() {
    /*cout << "1" << endl << endl;
    my_map<int, string> mp;
    cout << mp.find(10) << endl;
    mp.insert({1, "abc"});
    mp.output();
    cout << mp.find(1) << endl;
    my_map<int, string> mp2(mp);
    cout << mp2.find(1) << endl;
    mp2.pop(10);
    mp2.output();
    mp2.pop(1);
    cout << mp2.find(1) << endl;
    cout << endl << sizeof(string) << endl;*/
    /*my_map<string, string> mp;
    mp["aaa"] = "qwe";
    mp.output();
    my_map<string, string> mmp;
    mmp = mp;
    mmp.output();
    //mp.insert({2, 3});
    cout <<  " ------------------"<<mp.find("aaa") << endl;*/
    my_map<int, string> mp;
    mp.insert({1, "a"});
    mp.insert({2, "b"});
    mp.insert({3, "c"});
    mp.insert({4, "d"});
    mp.insert({5, "e"});
    mp.insert({6, "f"});
    mp.insert({7, "g"});
    mp.insert({8, "h"});
    mp.insert({9, "i"});
    mp.insert({10, "j"});
    mp.output();
    cout << endl;
/*
    mp.insert({1, "abc"});
    mp.insert({2, "absc"});
    mp.insert({3, "abcs"});
    mp.insert({4, "abasac"});
    cout << mp.find(1) << endl;
    cout << mp.empty() << endl;
    mp.pop(1);
    mp.output();
    cout << endl;
    my_map<int, string> mmp(mp);
    mmp.output();
    cout << endl;
    mmp.pop(2);
    mmp = mp;
    mmp.output();
*/
    return 0;
}
