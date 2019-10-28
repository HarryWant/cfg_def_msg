#ifndef _msg_H_
#define _msg_H_

#include <map>
#include <vector>
#include <string>
//linux
#include <stdlib.h>	//atof
#include <string.h>	//memcpy
#include <stdio.h>	//memcpy
#include <iostream>
#include "../__3rd/tinyXML2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;
													
#if defined(_WIN32) || defined(_WIN64)
    #include <winsock.h>                //htons(), htonl(), ntohl(), ntohs()
    #define sprintf sprintf_s           //win下 sprintf 不安全
#endif
#if defined(__unix__) || defined(unix)  
    #include <arpa/inet.h>
#endif

//下列分表为 unsigned short 和 unsigned short 的度° 对应的int值
#define USp360 182.04166666666
#define SSp360 91.022222221999
#define UIp360 11930464.708333
#define SIp360 5965232.3527777

//win下  注掉了msxml.h中的(typedef class XMLDocument XMLDocument;)  不然多重定义
//g++  *.cpp -std=c++11


enum enum_endian    {   
    eLE =1,
    eBE =2
};
enum enumSendRecv   {
    eSEND =1,
    eRECV =2 
};
enum enumFldTyp
{
    eDEFAULT,
    eS__CHAR,   eU__CHAR,   eX__CHAR,   eSf_CHAR,   eUf_CHAR,
    eS_SHORT,   eU_SHORT,   eX_SHORT,   eSfSHORT,   eUfSHORT,
    eS___INT,   eU___INT,   eX___INT,   eSf__INT,   eUf__INT,
    eS__LONG,   eU__LONG,
    e__FLOAT,   e_DOUBLE,					//16以下

    eBITGRP	= 1<<5,							//16以上	1<<4
    eBITGRP8,   eBITGRP16,  eBITGRP32,		

    eBIT = (1<<5) + (1<<3),					//16+4以上
    eBIT1,  eBIT2,  eBIT3,  eBIT4			
};

const char* getFldTypName(enumFldTyp e);
enumFldTyp	getFldTyp(string strFldTyp);


template <typename T>
T splitBitGrp(T t_Grp, int offset, int size)
{
	T result = t_Grp << offset;
	return result >> (sizeof(T) * 8 - size);
};

template <typename T>
T mergeBitGrp(T t_bit, int offset, int size, T t_BitGrp)
{
	t_bit = t_bit << (sizeof(T) * 8 - size);
	t_bit = t_bit >> offset;
	return  (t_BitGrp | t_bit);
};

class Var;

class MsgInf {
public:
	enum_endian     e_endian;
	unsigned int    uiIdx;
	unsigned int    uiLen;
	unsigned int    uiIDpos;
	unsigned int    uiIDlen;
	vector<Var*>    vecFld;
public:
	string          strName;
	char            buf_rcv[512];
	char            buf_snd[512];
	char*           cur_buf2fld;
	char*           cur_fld2buf;
	MsgInf(string strName,unsigned int idx)
		:strName(strName),uiIdx(idx)
	{};
	virtual ~MsgInf() {}
	unsigned int getIdx() { return	uiIdx; }
	void setIdxInf(int pos, int len) { uiIDpos = pos; uiIDlen = len; }
	void addFld(Var* fld);
	void real2show();
	void show2real();
	void buf2fld();			
	void fld2buf();
	void showFldInfo();
	//查找字段值需要遍历	改进利用哈希表存储各字段指针
	string getFldShow(string strFldName);
	void setFldShow(string strFldName, string strFldShow);
	int getBitFldVal(string strFldGrpName, string strFldBitName);
};

//TT	valReal;	//不能用容器存		不能使用模板的原因
//可以考虑添加		char format[24];		//定义显示格式
class Var{
protected:
	enumFldTyp      eFldTyp;
	string          strName;
	string          strVarTyp;
	string          strVarCpp;
	int             len;
	string          valShow;
	double          dbDimension;								
public:
	Var(enumFldTyp eFldTyp, string strName, string strShow, string strVarTyp, string strVarCpp, double dbDimension=1 )
		:eFldTyp(eFldTyp), strName(strName), valShow(strShow), 
		strVarTyp(strVarTyp), strVarCpp(strVarCpp), dbDimension(dbDimension)
	{};
	enumFldTyp getFldTyp()      { return eFldTyp;}
	string getName()            { return strName; };
	string getShow()            { return valShow; };
	string getVarCpp()          { return strVarCpp; };
	double getDimension()       { return dbDimension; };
	void setShow(string s)      { valShow   = s; };
	void setName(string s)      { strName   = s; };
	void setVarCpp(string s)    { strVarCpp = s; };
	void setDimension(double d) { dbDimension=d; }
	virtual void real2show() = 0;
	virtual void show2real() = 0;
	virtual void buf2fld(MsgInf*) = 0;
	virtual void fld2buf(MsgInf*) = 0;
	friend int cvt_msg2xml(MsgInf* msg, tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* ele);
	friend int rdMsgCfgXml(string strFile);
	friend int wrMsgCfgXml(string strFile);
	friend string cvt_msg2stt(MsgInf* msg);
	friend ostream& operator<<(ostream& os, Var* p);
	friend class MsgHlp;
	friend void initMapMsgFldHlp();
};
ostream& operator<<(ostream& os, Var* p);

//减少重复	提高可读怿
#define Var_Def0( fld_typ, val_typ, fun_val_hton,fun_val_ntoh, fmt_val2str)	\
class  Var_##fld_typ :public Var {														\
public:																				\
	val_typ val;																	\
	Var_##fld_typ(string name ,string show, string varcpp, double dimension=1)			\
		:Var(fld_typ, name, show, #val_typ, varcpp,  dimension)		{				\
		len = sizeof(val_typ);														\
	}																				\
	virtual void show2real() {														\
		val = (val_typ)(atof(valShow.data()));										\
	}																				\
	virtual void real2show() {														\
		char  cs[32];																\
		sprintf(cs, fmt_val2str,(val));												\
		valShow = string(cs);														\
	}																				\
	virtual void buf2fld(MsgInf* p) {												\
		memcpy(&val,(void*) p->cur_buf2fld, len);									\
		val	= fun_val_ntoh (val);													\
		p->cur_buf2fld += len;														\
	}																				\
	virtual void fld2buf(MsgInf* p) {												\
		val_typ tmp = fun_val_hton (val);											\
		memcpy((void*)p->cur_fld2buf, &tmp, len);									\
		p->cur_fld2buf += len;														\
	}																				\
};
#define Var_Def2( fld_typ, val_typ, fun_val_hton,fun_val_ntoh, fmt_val2str,val_typ2)	\
class Var_##fld_typ :public Var {																	\
public:																							\
	val_typ val;																				\
	Var_##fld_typ (string name ,string show, string varcpp, double dimension=1)						\
		:Var(fld_typ, name, show, #val_typ, varcpp, dimension)	{								\
		len = sizeof(val_typ);																	\
	}																							\
	virtual void show2real() {																	\
		val = (val_typ)(atof(valShow.data())*dbDimension);										\
	}																							\
	virtual void real2show() {																	\
		char  cs[32];																			\
		sprintf(cs, fmt_val2str, (val_typ2)(val/dbDimension));									\
		valShow = string(cs);																	\
	}																							\
	virtual void buf2fld(MsgInf* p) {															\
		memcpy(&val,(void*) p->cur_buf2fld, len);												\
		val	= fun_val_ntoh (val);																\
		p->cur_buf2fld += len;																	\
	}																							\
	virtual void fld2buf(MsgInf* p) {															\
		val_typ tmp = fun_val_hton (val);														\
		memcpy((void*)p->cur_fld2buf, &tmp, len);												\
		p->cur_fld2buf += len;																	\
	}																							\
};

Var_Def0( eS__CHAR, signed   char, , , "%d")
Var_Def0( eU__CHAR, unsigned char, , , "%u")
Var_Def0( eX__CHAR, unsigned char, , , "0x%02X")
Var_Def2( eSf_CHAR, signed   char, , , "%lf",float)
Var_Def2( eUf_CHAR, unsigned char, , , "%lf",float)
Var_Def0( eS_SHORT, signed   short, htons, ntohs, "%d")
Var_Def0( eU_SHORT, unsigned short, htons, ntohs, "%u")
Var_Def0( eX_SHORT, unsigned short, htons, ntohs, "0x%04X")
Var_Def2( eSfSHORT, signed   short, htons, ntohs, "%lf", double)
Var_Def2( eUfSHORT, unsigned short, htons, ntohs, "%lf", double)
Var_Def0( eS___INT, signed   int, htonl, ntohl, "%d")
Var_Def0( eU___INT, unsigned int, htonl, ntohl, "%u")
Var_Def0( eX___INT, unsigned int, htonl, ntohl, "0x%08X")
Var_Def2( eSf__INT, signed   int, htonl, ntohl, "%lf", float)
Var_Def2( eUf__INT, unsigned int, htonl, ntohl, "%lf", float)
Var_Def0( e__FLOAT, float, , , "%f")
Var_Def0( e_DOUBLE, double, , , "%lf")

//生成方法	目的：减少调用处的代码长度
Var* newVar(enumFldTyp typ,string name,string val, string varcpp, double dimension);

//可明确标识 各值含义 利用数组
class Var_Bit :public Var {
public:
	char		valBit;
	int			lenBit;
	string		valRep;
	Var_Bit(enumFldTyp fld_typ, string name, string show, string varcpp)
		:Var(fld_typ, name, show,  "---", varcpp)
	{};
	void show2real() {
		valBit = (int)atof(valShow.data());
	}
	void real2show() {
		char  cs[32];
		sprintf(cs, "%d", valBit);
		valShow = string(cs);
	}
	void buf2fld(MsgInf*) {	}
	void fld2buf(MsgInf*) {	}
	string getBitRep() { return  mapBitRep[(int)valBit]; }
	void setBitRep(int val, string mean) { mapBitRep[val] = mean; }
	string getBitRep(int val);
	map<int, string> mapBitRep;
};

#define VarBit_Def(cls_name, fld_typ, len_bit)      \
class cls_name :public Var_Bit {                    \
public:                                             \
    cls_name(string name,string show,string varcpp) \
        :Var_Bit(fld_typ, name, show, varcpp)       \
    {                                                                                                              \
        lenBit = len_bit;                           \
    }                                                                                                              \
};

VarBit_Def(Var_Bit1, eBIT1, 1);
VarBit_Def(Var_Bit2, eBIT2, 2);
VarBit_Def(Var_Bit3, eBIT3, 3);
VarBit_Def(Var_Bit4, eBIT4, 4);

#define VarBit_Def2( fld_typ, len_bit)      \
class Var_##fld_typ :public Var_Bit {                    \
public:                                             \
    Var_##fld_typ (string name,string show,string varcpp) \
        :Var_Bit(fld_typ, name, show, varcpp)       \
    {                                                                                                              \
        lenBit = len_bit;                           \
    }                                                                                                              \
};

VarBit_Def2(eBIT1, 1);
VarBit_Def2(eBIT2, 2);
VarBit_Def2(eBIT3, 3);
VarBit_Def2(eBIT4, 4);

Var_Bit* newVarBit(enumFldTyp typ, string name, string val, string varcpp);

class Var_BitGrp :public Var {
public:
	vector<Var_Bit*> vecVar;
	Var_BitGrp(enumFldTyp eFldTyp, string name, string strVarTyp, string varcpp)
		:Var(eFldTyp, name,"", strVarTyp, varcpp)
	{};
	void addFld(Var_Bit* fld);		//添加实现 最好校验一下
	friend int cvt_msg2xml(MsgInf* msg, tinyxml2::XMLDocument* xml);
	//virtual void show2real();		//单并组	(bit：显值到实值，grp：合并)
	//virtual void real2show();		//组拆单	(grp：分拆，bit：实值到显值)
	//virtual void buf2fld(MsgInf* p);
	//virtual void fld2buf(MsgInf* p);
};

//减少重复	提高可读怿
#define VarBitGrp_Def(cls_BitGrp, fld_typ, val_typ, fun_val_hton,fun_val_ntoh, fmt_val2str) \
class cls_BitGrp :public Var_BitGrp {														\
public:																						\
	val_typ val;																			\
	cls_BitGrp(string name,string varcpp)													\
		:Var_BitGrp(fld_typ,name, #val_typ, varcpp)											\
	{																						\
		len = sizeof(val);																	\
	}																						\
	void show2real(){																		\
		int bit_len = len * 8, cur_pos = 0,i=0;												\
		int cnt_all = vecVar.size();														\
		val=0;		/*每次要清零*/															\
		for ( i = 0;((i < cnt_all) &&(cur_pos < bit_len));i++) {							\
			vecVar[i]->show2real();															\
			val = mergeBitGrp((val_typ)vecVar[i]->valBit, cur_pos, vecVar[i]->lenBit, val);	\
			cur_pos += vecVar[i]->lenBit;													\
		}																					\
	}																						\
	void real2show() {																		\
		char  cs[32];																		\
		sprintf(cs, fmt_val2str, (val_typ)val);												\
		valShow = string(cs);																\
		int bit_len = len * 8, cur_pos = 0,i=0;												\
		int cnt_all = vecVar.size();														\
		for (int i = 0;(i < cnt_all) && (cur_pos < bit_len);i++) {							\
			vecVar[i]->valBit = splitBitGrp(val, cur_pos, vecVar[i]->lenBit);				\
			cur_pos += vecVar[i]->lenBit;													\
			vecVar[i]->real2show();															\
		}																					\
	}																						\
	void buf2fld(MsgInf* p) {																\
		memcpy(&val,(void*) p->cur_buf2fld, len);											\
		val	= fun_val_ntoh (val);															\
		p->cur_buf2fld += len;																\
	}																						\
	void fld2buf(MsgInf* p) {																\
		val_typ tmp = fun_val_hton (val);													\
		memcpy((void*)p->cur_fld2buf, &tmp, len);											\
		p->cur_fld2buf += len;																\
	}																						\
};
												
VarBitGrp_Def(VarBitGrp_uchar, eBITGRP8, unsigned char, , ,"0x%02X" )
VarBitGrp_Def(VarBitGrp_ushort, eBITGRP16, unsigned short, htons, ntohs, "0x%04X")
VarBitGrp_Def(VarBitGrp_uint, eBITGRP32, unsigned int, htonl, ntohl, "0x%08X")

//生成方法	目的：减少调用处的代码长度
Var_BitGrp* newVarGrp(enumFldTyp typ, string name, string varcpp);

//数值与字符的转换  使用的是 atof和sprintf  为的是跨平台
//		atof能够识别"0x",小数	atoi不能识别"0x"	
//		sprintf可跨平台			itoa并不是一个标准的C函数，它是Windows特有的

class Var_New :public Var {
public:
	Var_New()
		:Var(eDEFAULT, "**", "0", "****", "cpp", 1) {
	}
	virtual void real2show() {};
	virtual void show2real() {};
	virtual void buf2fld(MsgInf*) {};
	virtual void fld2buf(MsgInf*) {};
};

#endif


//无论是单精度还是双精度在存储中都分为三个部分：
//	1.			符号位(Sign) ：0代表正，1代表为负
//	2.			指数位（Exponent）：用于存储科学计数法中的指数数据，并且采用移位存储
//	3.			尾数部分（Mantissa）：尾数部分
