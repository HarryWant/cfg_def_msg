#ifndef _cvt_def_H_
#define _cvt_def_H_

/*
    将内存中协议解析结构 转化为xml配置文件，方便协议解析规则持久化
     根据xml配置文件 生成内存中协议解析结构，以便运行时更改解析结构
        将实质的解析机构按照c结构体定义的方式打印出来，方便验证
*/


#include <iostream>
#include "_msg.h"
#include "../_3rd/tinyXML2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

int cvt_msg2xml(MsgInf* msg, XMLDocument* doc, XMLElement* ele);  //将msg结构转为xml节点

int cvt_xml2msg(XMLElement* ele, MsgInf** msg);                   //将xml节点转为msg结构

string  cvt_msg2stt(MsgInf* msg);                                 //将 内存结构 转为 cpp定义
MsgInf* cvt_stt2msg(string  stt);                                 //将 cpp定义  转为 内存结构

int rdMsgCfgXml(string strFile, map<int, MsgInf*>& mapMsgInf);    //读取 协议配置文件
int wrMsgCfgXml(string strFile, map<int, MsgInf*>& mapMsgInf);    //写入 协议配置文件

#endif
