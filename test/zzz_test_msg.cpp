

#include "../inc/_msg.h"
#include "../inc/_MsgHlp.h"
#include "../inc/cvt_def.h"

void    test_hdlMsg();
void    test_MsgHlp();
void    test_msgxml();
void    test_stt2msg_msg2stt();
void    test_real_head_file();

#include <fstream>

int main()
{
    test_MsgHlp();
    test_hdlMsg();
    test_msgxml();
    test_stt2msg_msg2stt();

    test_real_head_file();

    //cache_hash_func<char*> t;
    //cout << t("q")<<endl;

    return 0;
}


MsgInf* test_newMsg() {
    MsgInf* inf = new MsgInf("协议", 0x23, "SailStat");
    inf->setIdxInf(0,4);//定义协议时，注意必须设置 报文标识在包中的位置，否则后期拆包比对时容易出意外  
    inf->addFld(new Var_eX__CHAR("标识x","0x77","scDev", 1));
    inf->addFld(new Var_e_STRING("描述", "说明", "strTip", 1));
    inf->addFld(new Var_eU__CHAR("设备0", "255", "scDev0", 1));
    inf->addFld(new Var_eU__CHAR("设备1", "256", "scDev1", 1));
    inf->addFld(new Var_eSf__INT("经度", "11.11111111", "uiLng", 11930464.708333));
    inf->addFld(new Var_eSf__INT("纬度", "22.22222222", "siLat", 5965232.3527777));
    inf->addFld(new Var_eUfSHORT("速度", "77", "ucSpd", 91.444444));
    inf->addFld(new Var_eSfSHORT("加速", "12",  "scAcc", 91.444444));
    inf->addFld(new Var_eSfSHORT("艏摇", "-11", "usYaw", 91.011111));
    inf->addFld(new Var_eSfSHORT("横摇", "-22", "usRoll", 91.022222));
    inf->addFld(new Var_eSfSHORT("纵摇", "-33", "usPitch", 91.033333));
    inf->addFld(new Var_eU_SHORT("海拔", "44",  "usHeight", 91.444444));

    Var_BitGrp* bitgrp = new VarBitGrp_uchar("控制", "ucCtrl");
    {
        Var_Bit* bit = new Var_eBIT1("开关", "1", "bitPower");
        bit->setBitRep(0, "开启");
        bit->setBitRep(1, "关闭");
        bitgrp->addFld(bit);
    }
    {
        Var_Bit* bit = new Var_eBIT3("电源", "4", "bitStat");
        bit->setBitRep(0, "正常");
        bit->setBitRep(1, "故障");
        bitgrp->addFld(bit);
    }
    bitgrp->addFld(new Var_eBIT1("模式", "1", "bitMode"));
    bitgrp->addFld(new Var_eBIT2("挡位", "3", "bitGear"));
    Var_Bit* bit = new Var_eBIT1("状态", "1", "bitStatus");
    bit->setBitRep(0, "正常");
    bit->setBitRep(1, "故障");
    bitgrp->addFld(bit);
    inf->addFld(bitgrp);
    inf->addFld(new Var_e_STRING("追加字段", "后期追加功能版本", "strTip2", 1));
    inf->addFld(new Var_e_STRING("评价", "真是一个灵活的胖子", "strTip3", 1));

    return inf;
}

MsgInf* test_newMsg2() {
    MsgInf* inf = new MsgInf("协议2", 0x23, "SailStat");
    inf->setIdxInf(0,4);//定义协议时，注意必须设置 报文标识在包中的位置，否则后期拆包比对时容易出意外  
    inf->addFld(new Var_eX__CHAR("标识x","","scDev", 1));
    //inf->addFld(new Var_e_STRING("描述", "", "strTip", 1));
    inf->addFld(new Var_eU__CHAR("设备0", "", "scDev0", 1));
    inf->addFld(new Var_eU__CHAR("设备1", "", "scDev1", 1));
    inf->addFld(new Var_eSf__INT("经度", "", "uiLng", 11930464.708333));
    inf->addFld(new Var_eSf__INT("纬度", "", "siLat", 5965232.3527777));
    inf->addFld(new Var_eUfSHORT("速度", "", "ucSpd", 91.444444));
    inf->addFld(new Var_eSfSHORT("加速", "",  "scAcc", 91.444444));
    inf->addFld(new Var_eSfSHORT("艏摇", "", "usYaw", 91.011111));
    inf->addFld(new Var_eSfSHORT("横摇", "", "usRoll", 91.022222));
    inf->addFld(new Var_eSfSHORT("纵摇", "", "usPitch", 91.033333));
    inf->addFld(new Var_eU_SHORT("海拔", "",  "usHeight", 91.444444));

    Var_BitGrp* bitgrp = new VarBitGrp_uchar("控制", "ucCtrl");
    {
        Var_Bit* bit = new Var_eBIT1("开关", "2", "bitPower");
        bit->setBitRep(0, "开启");
        bit->setBitRep(1, "关闭");
        bitgrp->addFld(bit);
    }
    {
        Var_Bit* bit = new Var_eBIT3("电源", "2", "bitStat");
        bit->setBitRep(0, "正常");
        bit->setBitRep(1, "故障");
        bitgrp->addFld(bit);
    }
    bitgrp->addFld(new Var_eBIT1("模式", "2", "bitMode"));
    bitgrp->addFld(new Var_eBIT2("挡位", "2", "bitGear"));
    Var_Bit* bit = new Var_eBIT1("状态", "2", "bitStatus");
    bit->setBitRep(0, "正常");
    bit->setBitRep(1, "故障");
    bitgrp->addFld(bit);
    inf->addFld(bitgrp);
    //inf->addFld(new Var_e_STRING("追加字段", "", "strTip2", 1));
    //inf->addFld(new Var_e_STRING("评价", "", "strTip3", 1));

    return inf;
}

//test_jerstis  先生成的结构体头文件，在放开其注释
//#include "protocol2.h" //test_jerstis  

void test_real_head_file()
{
    //验证 生成的结构体头文件， 是不是符合要求
/*  //test_jerstis  
	SailStat st;			//通过内存结构 生成的 结构体定义
	st.scDev0 =99;
	st.scDev1 =99;
	st.ucCtrl.bitOnOff =1;
	st.ucCtrl.bitPower =1;
	st.ucCtrl.bitMode =1;
	st.ucCtrl.bitGear =2;
*/
    MsgInf* inf2= test_newMsg2();

    cout << "-------------后" << endl;
    //memcpy(inf2->buf_rcv, &st, sizeof(st));	//test_jerstis  
    inf2->buf2fld();
    inf2->real2show();
    inf2->showFldInfo();
    cout << "-------------------" << endl;
    cout << "控制: " << inf2->getFldShow("控制")<<endl;
    cout << "控制:开关 " << inf2->getBitFldVal("控制","开关") << endl;
}

void test_hdlMsg()
{
    //inf.setFldShow("控制2");

    MsgInf* inf1= test_newMsg();
    MsgInf* inf2= test_newMsg2();


    cout << "-------------前" << endl;
    inf1->showFldInfo();
    inf1->show2real();
    inf1->fld2buf();
    //cout << "控制: " << inf1->gsetFldShow("控制")<<endl;

    cout<< " inf1->getMemLen()"<< inf1->getMemLen()<< "\n";


    cout << "-------------后" << endl;
    memcpy(inf2->buf_rcv, inf1->buf_snd, inf1->getMemLen());
    inf2->buf2fld();
    inf2->real2show();
    inf2->showFldInfo();
    cout << "-------------------" << endl;
    cout << "控制: " << inf2->getFldShow("控制")<<endl;
    cout << "控制:开关 " << inf2->getBitFldVal("控制","开关") << endl;
}

void test_MsgHlp() {
    
    MsgInf* inf= test_newMsg();

    MsgHlp hlp(inf);
    Var* v = hlp.getFld("设备0");
    if (NULL != v){
        cout <<" hlp.getFld('设备0') get "<<v->getShow()<<endl;
        v->setShow("22");
        cout <<" hlp.getFld('设备0') get "<<v->getShow()<<endl;
    }

    Var* v2 = hlp.getFld("控制:状态");
    if (NULL != v2)
        cout << " hlp.getFld('控制:状态') " << v2->getShow() << ((Var_Bit*)v2)->getBitRep() << endl;
    Var* v3 = hlp.getFldCpp("ucCtrl.bitStatus");
    if (NULL != v3)
        cout << " hlp.getFldCpp('ucCtrl.bitStatus') " << v3->getShow() << ((Var_Bit*)v3)->getBitRep() << endl;

}

void test_msgxml() 
{
    MsgInf* inf = test_newMsg();
    map<int, MsgInf*> mapMsgInf;
    mapMsgInf[0x1234] = inf;
    mapMsgInf[0x123] = inf;

    wrMsgCfgXml("protocol.xml", mapMsgInf);
    mapMsgInf.clear();
    rdMsgCfgXml("protocol.xml", mapMsgInf);

    string strSttDefCpp = cvt_msg2stt(inf);
    cout << strSttDefCpp;
    
    fstream fs;
    fs.open("protocol.h", ios::ate|ios::out);    
    fs << strSttDefCpp;
    fs.close();
}

void test_stt2msg_msg2stt(){
    cout<< __FUNCTION__<<endl;
    MsgInf* msg= cvt_stt2msg("        \
typedef struct SailStat {           //name:航行姿態    idx:35    len:4294967295    IDlen:4    IDpos:0               \n \
    unsigned char     scDev;        //name:标识x   type:eX__CHAR    value:0x77    dimension:1.000000                \n \
    std::string       strTip;       //name:描述    type:e_STRING    value:说明    dimension:1.000000                \n \
    unsigned char     scDev0;       //name:设备0   type:eU__CHAR    value:255    dimension:1.000000                 \n \
    unsigned char     scDev1;       //name:设备1   type:eU__CHAR    value:256    dimension:1.000000                 \n \
    signed int        uiLng;        //name:经度    type:eSf__INT    value:11.11111111    dimension:11930464.708333  \n \ 
    signed int        siLat;        //name:纬度    type:eSf__INT    value:22.22222222    dimension:5965232.352778   \n \
    unsigned short    ucSpd;        //name:速度    type:eUfSHORT    value:77    dimension:91.444444                 \n \ 
    signed short      scAcc;        //name:加速    type:eSfSHORT    value:12    dimension:91.444444                 \n \ 
    signed short      usYaw;        //name:艏摇    type:eSfSHORT    value:-11    dimension:91.011111                \n \ 
    signed short      usRoll;       //name:横摇    type:eSfSHORT    value:-22    dimension:91.022222                \n \ 
    signed short      usPitch;      //name:纵摇    type:eSfSHORT    value:-33    dimension:91.033333                \n \ 
    unsigned short    usHeight;     //name:海拔    type:eU_SHORT    value:44    dimension:91.444444                 \n \ 
    typedef struct ucCtrl {         //name:控制    type:eBITGRP8    value:                                          \n \ 
        unsigned char     bitOnOff:1;    //name:开关    type:eBIT1    value:1                                       \n \ 
             //0:开启 1:关闭                                                                                        \n \ 
        unsigned char     bitPower:3;    //name:电源    type:eBIT3    value:4                                       \n \ 
             //0:正常 1:故障                                                                                        \n \ 
        unsigned char     bitMode:1;    //name:模式    type:eBIT1    value:1                                        \n \ 
        unsigned char     bitGear:2;    //name:挡位    type:eBIT2    value:3                                        \n \ 
        unsigned char     bitStatus:1;  //name:状态    type:eBIT1    value:1                                        \n \ 
             //0:正常 1:故障                                                                                        \n \ 
    };                                                                                                              \n \ 
    std::string     strTip2;    //name:追加字段    type:e_STRING    value:后期追加功能版本    dimension:1.000000    \n \ 
    std::string     strTip3;    //name:评价    type:e_STRING    value:真是一个灵活的胖子    dimension:1.000000      \n \
}; ");

    cout <<"------\n" << cvt_msg2stt(msg);

    return;        
}
