

#include "../inc/_hashmap.h"

#ifdef	TEST_HASHMAP

	//首先要定义hash值函数与键值比较函数
class HashFunc
{
public:
	int operator()(const string & key)
	{
		int hash = 0;		//此哈希取值函数 不是很好
		for (unsigned int i = 0; i < key.length(); ++i)
			hash = hash << 7 ^ key[i];
		return (hash & 0x7FFFFFFF);
	}
};

class EqualKey{
public:
	bool operator()(const string & A, const string & B){
		return (A.compare(B) == 0)?  true : false;
	}
};

int test_hashmap()
{

	HashMap<string, string, HashFunc> hashmap(100,"z_z");

	hashmap.add("1", "111");
	hashmap.add("2", "222");
	hashmap.add("3", "333");

	cout << "after add:" << endl;
	cout << hashmap.get("1").c_str() << endl;
	cout << hashmap.get("2").c_str() << endl;
	cout << hashmap["3"].c_str() << endl;


	if (hashmap.del("1"))
		cout << "\"1\" is del" << endl;
	cout << hashmap.get("1").c_str() << endl;

	hashmap["2"] = "777";
	cout << hashmap["2"].c_str() << endl;

	HashFunc* fn = new HashFunc();
	cout << (*fn)("1") <<endl;
//
	cout << "------------" <<endl;
	HashMap<string, int*, HashFunc, EqualKey> has(100,(int*)NULL);
	int i=1111;
	has.add("1", &i);
	if( NULL==has.get("0"))
		cout <<" NULL "<<endl;
	cout << *has.get("1")<<endl;

	cin.get();
	return 0;
}

#endif
