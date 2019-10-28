
#include "zzz_test_msg.h"

#ifdef		TEST_msg

MsgInf* test_newMsg() {
	MsgInf* inf = new MsgInf("协议", 0x23);
	inf->setIdxInf(4,4);//定义协议时，注意必须设置 报文标识在包中的位置，否则后期拆包比对时容易出意外  
	inf->addFld(new Var_eX__CHAR("标识x","0x77","scDev", 1));
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
		Var_Bit* bit = new Var_eBIT3("电源", "4", "bitPower");
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

	return inf;
}

void test_hdlMsg()
{
	//inf.setFldShow("控制2");

	MsgInf* inf1= test_newMsg();
	MsgInf* inf2= test_newMsg();


	cout << "-------------前" << endl;
	inf1->showFldInfo();
	inf1->show2real();
	inf1->fld2buf();
	//cout << "控制: " << inf1->gsetFldShow("控制")<<endl;

	cout << "-------------后" << endl;
	memcpy(inf2->buf_rcv, inf1->buf_snd, 512);
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

void test_msgxml() {

	MsgInf* inf = test_newMsg();
	map<int, MsgInf*> mapMsgInf;
	mapMsgInf[0x1234] = inf;
	mapMsgInf[0x123] = inf;


	wrMsgCfgXml("protocol.xml", mapMsgInf);
	rdMsgCfgXml("protocol.xml", mapMsgInf);

	string strSttDefCpp = cvt_msg2stt(inf);
	cout << strSttDefCpp;
}


#endif
