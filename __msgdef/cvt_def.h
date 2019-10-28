#ifndef _cvt_def_H_
#define _cvt_def_H_

#include <iostream>
#include "_msg.h"
#include "../__3rd/tinyXML2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

int cvt_msg2xml(MsgInf* msg, XMLDocument* doc, XMLElement* ele);  //将msg结构转为xml节点

int cvt_xml2msg(XMLElement* ele, MsgInf** msg);                   //将xml节点转为msg结构

string cvt_msg2stt(MsgInf* msg);	                                 //将结构体 转为cpp定义

int rdMsgCfgXml(string strFile, map<int, MsgInf*>& mapMsgInf);    //读取 协议配置文件
int wrMsgCfgXml(string strFile, map<int, MsgInf*>& mapMsgInf);    //写入 协议配置文件

#endif
