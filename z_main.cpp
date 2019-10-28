
#include "__msgdef/cvt_def.h"
#include "__msgdef/_MsgHlp.h"
#include "__msgdef/zzz_test_msg.h" 

int main(){
	test_MsgHlp();
	test_hdlMsg();
	test_msgxml();
	cache_hash_func<char*> t;
	cout << t("q")<<endl;

  

	return 0;
}
