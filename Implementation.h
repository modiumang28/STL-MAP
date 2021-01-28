#include <string>
using namespace std;

template <typename V>
class MapNode {
public:

  string key;
  V value;
  MapNode<V> *next;

  MapNode(string key, V value) {
    this->key = key;
    this->value = value;
    next = NULL;
  }

  ~MapNode() {
    delete next;
  }
};

template <typename V>
class ourmap {
  MapNode<V>* *buckets;
  int totalSize;
  int bucketSize;

public:
  ourmap() {
    totalSize = 0;
    bucketSize = 5;
    buckets = new MapNode<V>*[bucketSize];
    for(int i = 0; i < bucketSize; i++) {
      buckets[i] = NULL;
    }
  }

  ~ourmap() {
    for(int i = 0; i < bucketSize; i++) {
      delete buckets[i];
    }
    delete [] buckets;
  }

private:

  int getBucketIndex(string key) {
    int hashCode = 0;
    int currCoeff = 1;
    for(int i = key.size()-1; i >= 0; i--) {
      hashCode += key[i] * currCoeff;
      hashCode = hashCode % bucketSize;
      currCoeff *= 37;
      currCoeff = currCoeff % bucketSize;
    }
    return hashCode % bucketSize;
  }

  void rehash() {
    MapNode<V>* *temp = buckets;
    buckets = new MapNode<V>*[2*bucketSize];
    for(int i = 0; i < 2*bucketSize; i++) {
      buckets[i] = NULL;
    }
    // MapNode<V>* *newBucketArray = new MapNode<V>*[2*bucketSize];
    // for(int i = 0; i < 2*bucketSize; i++) {
    //   newBucketArray[i] = NULL;
    // }
    int oldBucketSize = bucketSize;
    totalSize = 0;
    bucketSize *= 2;
    for(int i = 0; i < oldBucketSize; i++) {
      MapNode<V> *head = temp[i];
      while(head != NULL) {
        string key = head->key;
        V value = head->value;
        insert(key, value);
        head = head->next;
      }
    }
    for(int i = 0; i < oldBucketSize; i++) {
      delete temp[i];
    }
    delete [] temp;
  }

public:

  int size() {
    return totalSize;
  }

  double getLoadFactor() {
    return (1.0*totalSize)/bucketSize;
  }

  V getValue(string key) {
    int bucketIndex = getBucketIndex(key);
    MapNode<V> *head = buckets[bucketIndex];
    while(head != NULL) {
      if(head->key == key) {
        return head->value;
      }
      head = head->next;
    }
    return 0;
  }

  void insert(string key, V value) {
    int bucketIndex = getBucketIndex(key);
    MapNode<V> *head = buckets[bucketIndex];
    MapNode<V> *temp = head;
    while(temp != NULL) {
      if(temp->key == key) {
        temp->value = value;
        return;
      }
      temp = temp->next;
    }
    MapNode<V> *newNode = new MapNode<V>(key, value);
    newNode->next = head;
    head = newNode;
    buckets[bucketIndex] = head;
    totalSize++;
    double loadFactor = (1.0*totalSize)/bucketSize;
    if(loadFactor > 0.7) {
      rehash();
    }
  }

  V remove(string key) {
    int bucketIndex = getBucketIndex(key);
    MapNode<V> *head = buckets[bucketIndex];
    MapNode<V> *prev = NULL;
    MapNode<V> *curr = head;
    while(curr != NULL) {
      if(curr->key == key) {
        V value = curr->value;
        if(prev == NULL) {
          head = curr->next;
        } else {
          prev->next = curr->next;
        }
        curr->next = NULL;
        delete curr;
        buckets[bucketIndex] = head;
        totalSize--;
        return value;
      }
      prev = curr;
      curr = curr->next;
    }
    return 0;
  }
};
