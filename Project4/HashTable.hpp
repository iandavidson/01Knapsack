
#include <list>
#include <iostream>


//am going to use node struct as the type of list to store i, j, value and hashed value
using namespace std;
class HashTable {
private:
    struct node {
      node(){
        unique = -1;
        value =-1;
      }
      node(l, val):unique(l), value(val){}
      int unique;
      int value;

    }
public:
  HashTable(slots){
    this->k = slots;
    this->table = new list<node>[this->k];
  }

  int hashKey(int key){
    return key % this->k;
  }

  bool getKeyInTable(int key, int & keyValue){
    {
      int index = hashKey(key);

      // find the key in (i)th list
      list <node>::iterator i;
      for (i = table[index].begin(); i != table[index].end(); i++) {
        if (*i->unique == key){
          keyValue = *i->value;
          return true;
        }
      }
      return false;
  }

  void insertNode(int key, int val){
    int slot = hashKey(key);
    table[slot].push_back(node(key, val)); //not sure if this works lol
  }

  list<node> table;
  int k; //# of slots in hashTable

}
