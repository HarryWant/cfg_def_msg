
#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <iostream>
using namespace std;

//    HashMap由数组+链表组成的，
//        数组是HashMap的主体，链表则是主要为了解决哈希冲突而存在的，
//        若定位到的数组位置不含链表（）,对于查找，添加等操作很快，仅需一次寻址即可；
//        若定位到的数组包含链表，对于添加操作，其时间复杂度为O(n)，首先遍历链表，存在即覆盖，否则新增；
//            对于查找操作来讲，仍需遍历链表，然后通过key对象的equals方法逐一比对查找。
//    所以，性能考虑，HashMap中的链表出现越少，性能才会越好
//    hashcode 中取 位置索引，，，最好hasmap为2的幂     原因："且"优于"除"
//    也可以设置适当的扩容函数，  添加命中数组的百分比， 适时扩容

#define    TEST_HASHMAP
#ifdef        TEST_HASHMAP
int test_hashmap();
#endif

template<class Key, class Val>
class HashNode;

template <class Key, class Val, class HashFunc, class EqualKey= std::equal_to<Key> >
class HashMap
{
public:
    HashMap(int size,const Val val);
    ~HashMap();
    bool add(const Key& key, const Val& val);
    bool del(const Key& key);
    Val& get(const Key& key);
    Val& operator [](const Key& key);

private:
    HashFunc                hash;
    EqualKey                equal;
    HashNode<Key, Val>        **table;
    unsigned int            _size;
    Val                        nullValue;
    static int pos_for(int h, int len_2pow) {    return h & (len_2pow - 1);  }
    // 计算超过 capacity 的最小 2^n 
    size_t getTableSize(size_t capacity) 
    {    
        size_t ssize = 1;
        while (ssize < capacity)
            ssize <<= 1;
       return ssize;
         }
         //增加扰动
    size_t getHash(size_t h) {
        h ^= (h >> 20) ^ (h >> 12);
        return h ^ (h >> 7) ^ (h >> 4);
        }
};

template<class Key, class Val>
class HashNode
{
public:
    Key            _key;
    Val            _val;
    HashNode*    next;

    HashNode(Key key, Val val)
    {
        _key    = key;
        _val    = val;
        next    = NULL;
    }
    ~HashNode()
    {
    }
    HashNode& operator=(const HashNode& node)
    {
        _key    = node.key;
        _val    = node.val;
        next    = node.next;
        return *this;
    }
};

template <class Key, class Value, class HashFunc, class EqualKey>
HashMap<Key, Value, HashFunc, EqualKey>::HashMap(int size, Value nullVal)
    : nullValue(nullVal)
{
    _size = getTableSize(size);
    hash = HashFunc();
    equal = EqualKey();
    table = new HashNode<Key, Value>*[_size];
    for (unsigned i = 0; i < _size; i++)
        table[i] = NULL;
}

template <class Key, class Value, class HashFunc, class EqualKey>
HashMap<Key, Value, HashFunc, EqualKey>::~HashMap()
{
    for (unsigned i = 0; i < _size; i++)
    {
        HashNode<Key, Value> *currentNode = table[i];
        while (currentNode)
        {
            HashNode<Key, Value> *temp = currentNode;
            currentNode = currentNode->next;
            delete temp;
        }
    }
    delete table;
}

template <class Key, class Value, class HashFunc, class EqualKey>
bool HashMap<Key, Value, HashFunc, EqualKey>::add(const Key& key, const Value& value)
{
    int index = pos_for( hash(key), _size);
    HashNode<Key, Value> * node = new HashNode<Key, Value>(key, value);
    node->next = table[index];
    table[index] = node;
    node = node->next;
    HashNode<Key, Value>* prev = NULL;
    while (node)
    {
        if (node->_key == key)
        {
            if (prev == NULL)
                table[index] = node->next;
            else
                prev->next = node->next;
            delete node;
            return true;
        }
        prev = node;
        node = node->next;
    }
    return true;
}

template <class Key, class Value, class HashFunc, class EqualKey>
bool HashMap<Key, Value, HashFunc, EqualKey>::del(const Key& key)
{
    unsigned index =  pos_for( hash(key), _size);
    HashNode<Key, Value>* node = table[index];
    HashNode<Key, Value>* prev = NULL;
    while (node)
    {
        if (node->_key == key)
        {
            if (prev == NULL)
                table[index] = node->next;
            else
                prev->next = node->next;
            delete node;
            return true;
        }
        prev = node;
        node = node->next;
    }
    return false;
}

template <class Key, class Value, class HashFunc, class EqualKey>
Value& HashMap<Key, Value, HashFunc, EqualKey>::get(const Key& key)
{
    unsigned  index =  pos_for( hash(key), _size);
    if (table[index] == NULL)
        return nullValue;
    else
    {
        HashNode<Key, Value> * node = table[index];
        while (node)
        {
            if (node->_key == key)
                return node->_val;
            node = node->next;
        }
        return nullValue;
    }
}

template <class Key, class Value, class HashFunc, class EqualKey>
Value& HashMap<Key, Value, HashFunc, EqualKey>::operator [](const Key& key)
{
    return get(key);
}

/**
* hash算法仿函数
*/

template<class KeyType>
struct cache_hash_func {
};

inline std::size_t cache_hash_string(const char* __s) {
    unsigned long __h = 0;
    for (; *__s; ++__s)
        __h = 5 * __h + *__s;
    return std::size_t(__h);
}

template<>
struct cache_hash_func<std::string> {
    std::size_t operator()(const std::string & __s) const {
        return cache_hash_string(__s.c_str());
    }
};

template<>
struct cache_hash_func<char*> {
    std::size_t operator()(const char* __s) const {
        return cache_hash_string(__s);
    }
};

template<>
struct cache_hash_func<const char*> {
    std::size_t operator()(const char* __s) const {
        return cache_hash_string(__s);
    }
};

#endif
