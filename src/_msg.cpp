
#include "../inc/_msg.h"

#if defined(_WIN32) || defined(_WIN64)
    #pragma comment(lib,"ws2_32.lib")
#endif

void MsgInf::buf2fld() 
{
    cur_buf2fld = buf_rcv;
    vector<Var*>::iterator fld = vecFld.begin();
    for(;fld != vecFld.end(); ++fld)
        (*fld) ->buf2fld(this);
}
void MsgInf::fld2buf() {
    cur_fld2buf = buf_snd;
    vector<Var*>::iterator fld = vecFld.begin();
    for(;fld != vecFld.end(); ++fld)
        (*fld)->fld2buf(this);
}

void MsgInf::real2show() 
{
    vector<Var*>::iterator fld = vecFld.begin();
    for(;fld != vecFld.end(); ++fld)
    {
        (*fld)->real2show();
    }
}
void MsgInf::show2real() 
{
    vector<Var*>::iterator fld = vecFld.begin();
    for(;fld != vecFld.end(); ++fld)
    {
        (*fld)->show2real();
        //if (fld->getFldTyp() < eBITGRP)
        //    ;
        //else {
        //    Var_BitGrp* fldBitGrp = (Var_BitGrp*)fld;
        //    for (auto& fld2 : fldBitGrp->vecVar)
        //        fld2->show2real();
        //}
    }
}

void MsgInf::showFldInfo() {
    vector<Var*>::iterator fld = vecFld.begin();
    for(;fld != vecFld.end(); ++fld)
    {
        cout << *fld << endl;
        if ((*fld)->getFldTyp() > eBITGRP) {
            Var_BitGrp* fldBitGrp = (Var_BitGrp*) (*fld);
             vector<Var_Bit*>::iterator fld2 = fldBitGrp->vecVar.begin();
             for(;fld2 != fldBitGrp->vecVar.end(); ++fld2)
                cout << "   " << *fld2 << endl;
        }
    }
}

ostream& operator<<(ostream& os, Var* p) {
    os << " 字段：" << p->getName() 
        << " 类型：" << getFldTypName(p->getFldTyp()) 
        << " 数值：" << p->getShow() ;
    return os;
}

void MsgInf::addFld(Var* fld) {
    vecFld.push_back(fld);
    //uiLen;
}

int MsgInf::getMemLen() 
{
    int lenAll =0;
    vector<Var*>::iterator fld = vecFld.begin();
    for(;fld != vecFld.end(); ++fld)
        lenAll += (*fld)->getMemLen();
    return uiLen = lenAll;
        
}

string MsgInf::getFldShow(string strFldName) 
{
    vector<Var*>::iterator fld = vecFld.begin();
    for(;fld != vecFld.end(); ++fld)
    {
        if (strFldName == (*fld)->getName())
            return (*fld)->getShow();
        if ((*fld)->getFldTyp() > eBITGRP) {
            Var_BitGrp* fldBitGrp = (Var_BitGrp*)(*fld);
             vector<Var_Bit*>::iterator fld2 = fldBitGrp->vecVar.begin();
             for(;fld2 != fldBitGrp->vecVar.end(); ++fld2)
                if (strFldName == (*fld2)->getName())
                    return (*fld2)->getShow();
        }
    }
    return "not find";
        
}

void MsgInf::setFldShow(string strFldName, string strFldShow){
    vector<Var*>::iterator fld = vecFld.begin();
    for(;fld != vecFld.end(); ++fld)
    {
        if (strFldName == (*fld)->getName())
            (*fld)->setShow(strFldShow);
        if ((*fld)->getFldTyp() > eBITGRP) {
            Var_BitGrp* fldBitGrp = (Var_BitGrp*)(*fld);
             vector<Var_Bit*>::iterator fld2 = fldBitGrp->vecVar.begin();
             for(;fld2 != fldBitGrp->vecVar.end(); ++fld2)
                if (strFldName == (*fld2)->getName())
                    (*fld2)->setShow(strFldShow);
        }
    }
}

int MsgInf::getBitFldVal(string strFldGrpName, string strFldBitName) {
    vector<Var*>::iterator fld = vecFld.begin();
    for(;fld != vecFld.end(); ++fld)
    {
        if ((*fld)->getFldTyp() > eBITGRP && strFldGrpName == (*fld)->getName()){
            Var_BitGrp* fldBitGrp = (Var_BitGrp*)(*fld);
            vector<Var_Bit*>::iterator fld2 = fldBitGrp->vecVar.begin();
              for(;fld2 != fldBitGrp->vecVar.end(); ++fld2)
                 return (*fld2)->valBit;
        }
    }
    return 0;
}

Var* newVar(enumFldTyp typ, string name, string val, string varcpp, double dimension) {
    switch (typ){
    case eS__CHAR:    return    new Var_eS__CHAR(name, val, varcpp, dimension);
    case eU__CHAR:    return    new Var_eU__CHAR(name, val, varcpp, dimension);
    case eX__CHAR:    return    new Var_eX__CHAR(name, val, varcpp, dimension);
    case eSf_CHAR:    return    new Var_eSf_CHAR(name, val, varcpp, dimension);
    case eUf_CHAR:    return    new Var_eUf_CHAR(name, val, varcpp, dimension);
    case eS_SHORT:    return    new Var_eS_SHORT(name, val, varcpp, dimension);
    case eU_SHORT:    return    new Var_eU_SHORT(name, val, varcpp, dimension);
    case eX_SHORT:    return    new Var_eX_SHORT(name, val, varcpp, dimension);
    case eSfSHORT:    return    new Var_eSfSHORT(name, val, varcpp, dimension);
    case eUfSHORT:    return    new Var_eUfSHORT(name, val, varcpp, dimension);
    case eS___INT:    return    new Var_eS___INT(name, val, varcpp, dimension);
    case eU___INT:    return    new Var_eU___INT(name, val, varcpp, dimension);
    case eX___INT:    return    new Var_eX___INT(name, val, varcpp, dimension);
    case eSf__INT:    return    new Var_eSf__INT(name, val, varcpp, dimension);
    case eUf__INT:    return    new Var_eUf__INT(name, val, varcpp, dimension);
    //case eS__LONG:    return    new Var_long(name, 0, varcpp, dimension);
    //case eU__LONG:    return    new Var_u_short(name,val, varcpp, , dimension);
    case e__FLOAT:    return    new Var_e__FLOAT(name, val, varcpp, dimension);
    case e_DOUBLE:    return    new Var_e_DOUBLE(name, val, varcpp, dimension);
    case e_STRING:    return    new Var_e_STRING(name, val, varcpp, dimension);
    default:
        return        NULL;
    }
}

Var_Bit* newVarBit(enumFldTyp typ, string name, string val, string varcpp) {
    switch (typ)    {
    case eBIT1:        return    new Var_eBIT1(name, val, varcpp);
    case eBIT2:        return    new Var_eBIT2(name, val, varcpp);
    case eBIT3:        return    new Var_eBIT3(name, val, varcpp);
    case eBIT4:        return    new Var_eBIT4(name, val, varcpp);
    default:        return    NULL;
    }
}

Var_BitGrp* newVarGrp(enumFldTyp typ, string name, string varcpp) {
    switch (typ){
    case eBITGRP8:    return    new VarBitGrp_uchar(name, varcpp);
    case eBITGRP16:   return    new VarBitGrp_ushort(name, varcpp);
    case eBITGRP32:   return    new VarBitGrp_uint(name, varcpp);
    default:        return    NULL;
    }
}

string Var_Bit::getBitRep(int val) {
    map<int, string>::iterator it = mapBitRep.find(val);
    if (it != mapBitRep.end())
        return it->second;
    else
        return "未定义";
}

void Var_BitGrp::addFld(Var_Bit* bit) {
    vecVar.push_back(bit);
}

const char* getFldTypName(enumFldTyp e) {
    static char str_base[][16]  = {"eDEFAULT",
        "eS__CHAR","eU__CHAR",   "eX__CHAR",   "eSf_CHAR","eUf_CHAR",
        "eS_SHORT","eU_SHORT",   "eX_SHORT",   "eSfSHORT","eUfSHORT",
        "eS___INT","eU___INT",   "eX___INT",   "eSf__INT","eUf__INT",
        "eS__LONG","eU__LONG",   "e__FLOAT",   "e_DOUBLE", "e_STRING"    };
    static char str_grp[][16]   = {"eBITGRP",  "eBITGRP8","eBITGRP16","eBITGRP32",};
    static char str_bit[][16]   = {"eBIT",     "eBIT1","eBIT2","eBIT3","eBIT4",};
    if (e >= eDEFAULT && e < eBITGRP)
        return str_base[e - eDEFAULT];
    if (e >= eBITGRP && e < eBITGRP+4)
        return str_grp[e - eBITGRP];
    if (e >= eBIT && e < eBIT+5)
        return str_bit[e - eBIT];
    else 
        return "无效";
}

map<string, enumFldTyp> mapFldTyp_n2v;
enumFldTyp    getFldTyp(string strFldTyp) {
    static bool init=0;
    if (!init) {
        mapFldTyp_n2v["eDEFAULT"] = eDEFAULT;
        mapFldTyp_n2v["eS__CHAR"]  = eS__CHAR;
        mapFldTyp_n2v["eU__CHAR"]  = eU__CHAR;
        mapFldTyp_n2v["eX__CHAR"]  = eX__CHAR;
        mapFldTyp_n2v["eSf_CHAR"]  = eSf_CHAR;
        mapFldTyp_n2v["eUf_CHAR"]  = eUf_CHAR;
        mapFldTyp_n2v["eS_SHORT"]  = eS_SHORT;
        mapFldTyp_n2v["eU_SHORT"]  = eU_SHORT;
        mapFldTyp_n2v["eSfSHORT"]  = eSfSHORT;
        mapFldTyp_n2v["eUfSHORT"]  = eUfSHORT;
        mapFldTyp_n2v["eX_SHORT"]  = eX_SHORT;
        mapFldTyp_n2v["eS___INT"]  = eS___INT;
        mapFldTyp_n2v["eU___INT"]  = eU___INT;
        mapFldTyp_n2v["eSf__INT"]  = eSf__INT;
        mapFldTyp_n2v["eUf__INT"]  = eUf__INT;
        mapFldTyp_n2v["eX___INT"]  = eX___INT;
        mapFldTyp_n2v["eS__LONG"]  = eS__LONG;
        mapFldTyp_n2v["eU__LONG"]  = eU__LONG;
        mapFldTyp_n2v["e__FLOAT"]  = e__FLOAT;
        mapFldTyp_n2v["e_DOUBLE"]  = e_DOUBLE;
        mapFldTyp_n2v["e_STRING"]  = e_STRING;
        mapFldTyp_n2v["eBITGRP"]  = eBITGRP;
        mapFldTyp_n2v["eBITGRP8"]  = eBITGRP8;
        mapFldTyp_n2v["eBITGRP16"] = eBITGRP16;
        mapFldTyp_n2v["eBITGRP32"] = eBITGRP32;
        mapFldTyp_n2v["eBIT"]     = eBIT;
        mapFldTyp_n2v["eBIT1"]     = eBIT1;
        mapFldTyp_n2v["eBIT2"]     = eBIT2;
        mapFldTyp_n2v["eBIT3"]     = eBIT3;
        mapFldTyp_n2v["eBIT4"]     = eBIT4;
        init++;
    }
    map<string, enumFldTyp>::iterator it = mapFldTyp_n2v.find(strFldTyp);
    if (it != mapFldTyp_n2v.end())
        return mapFldTyp_n2v[strFldTyp];
    else
        return (enumFldTyp)eDEFAULT;
}

#define FUN__declare(SPACE,Type,MapFun) Type SPACE MapFun(const char* cmd);
 
#define FUN__define(Type,MapFun)        Type MapFun(const char* cmd){           
#define ___IF_CMP(keyword)                   if(!strcmp(#keyword,cmd))      return keyword;
#define ___ELSE(keyword)                     else                           return keyword;
#define FUN__enddef                     }
    

FUN__define(enum_endian,getEndian)
___IF_CMP(eBE)
___IF_CMP(eLE)
___ELSE(eLEBE)
FUN__enddef  

FUN__define(enumSendRecv,getSendRecv)
___IF_CMP(eSEND)
___IF_CMP(eRECV)
___ELSE(eSENDRECV)
FUN__enddef   


