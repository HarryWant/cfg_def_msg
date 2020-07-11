#ifndef _MsgHlp_H_
#define _MsgHlp_H_

#include "_hashmap.h"
#include "_msg.h"

/*
    利用hashmap读取字段值 为了快速读取
*/

class MsgHlp
{
    //首先要定义hash值函数与键值比较函数
    class HashFunc{
    public:
        int operator()(const string & key){
            int hash = 0;                            //此哈希取值函数 不是很好
            for (unsigned int i = 0; i < key.length(); ++i)
                hash = hash << 7 ^ key[i];
            return (hash & 0x7FFFFFFF);
        }
    };

    class EqualKey{
    public:
        bool operator()(const string & A, const string & B){
            return (A.compare(B) == 0) ?     true : false;
        }
    };

    MsgInf* msgInf;
    HashMap<string, Var*, cache_hash_func<string>, EqualKey>* has;
    HashMap<string, Var*, HashFunc, EqualKey>* hasCpp;
public:
    MsgHlp(MsgInf* msg)
        : msgInf(msg) {
        //1,生成哈希表
        has      = new HashMap<string, Var*, cache_hash_func<string>, EqualKey>(1024, NULL);
        hasCpp   = new HashMap<string, Var*, HashFunc, EqualKey>(1024, NULL);
/*
        //2,添加 映射元素
        for (auto& fld : msgInf->vecFld) {
            has->add(fld->getName(), fld);
            hasCpp->add(fld->strVarCpp, fld);
            if (fld->getFldTyp() < eBITGRP)
                ;
            else {
                Var_BitGrp* fldBitGrp = (Var_BitGrp*)fld;
                for (auto& fld2 : fldBitGrp->vecVar){
                    has->add(fld->getName() + ":" + fld2->getName(), fld2);
                    hasCpp->add(fld->strVarCpp + "." + fld2->strVarCpp, fld2);
                }    
            }
        }
*/
        vector<Var*>::iterator fld = msgInf->vecFld.begin();
        for(;fld != msgInf->vecFld.end(); ++fld)
        {
            has        ->add((*fld)->getName(), (*fld));
            hasCpp    ->add((*fld)->strVarCpp, (*fld));
            if ((*fld)->getFldTyp() < eBITGRP)
                ;
            else {
                Var_BitGrp* fldBitGrp = (Var_BitGrp*)(*fld);
                vector<Var_Bit*>::iterator fld2 = fldBitGrp->vecVar.begin();
                 for(;fld2 != fldBitGrp->vecVar.end(); ++fld2){
                    has->add((*fld)->getName() + ":" + (*fld2)->getName(), (*fld2));
                    hasCpp->add((*fld)->strVarCpp + ":" + (*fld2)->strVarCpp, (*fld2));                
                }
            }
        }
    }
    ~MsgHlp() {
        delete has;
    }
    Var* getFld(string str) {
        return has->get(str);
    }
    Var* getFldCpp(string str) {
        return hasCpp->get(str);
    }
};


inline int getInt(string strVal)        { return atof(strVal.data()); }
inline short getShort(string strVal)    { return atof(strVal.data()); }
inline float getFloat(string strVal)    { return atof(strVal.data()); }
inline double getDouble(string strVal)  { return atof(strVal.data()); }

#endif
