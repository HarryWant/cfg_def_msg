#include "../inc/cvt_def.h"

int cvt_msg2xml(MsgInf* msg, tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* ele)
{
    XMLElement* xml = doc->NewElement("Msg");                                //协议
    xml->SetAttribute("name",msg->strName.c_str());
    xml->SetAttribute("SttDef",msg->strSttDef.c_str());
    xml->SetAttribute("idx", msg->uiIdx);
    xml->SetAttribute("len", msg->uiLen);
    xml->SetAttribute("IDlen", msg->uiIDlen);
    xml->SetAttribute("IDpos", msg->uiIDpos);

    vector<Var*>::iterator it1 = msg->vecFld.begin();
   for(;it1 != msg->vecFld.end(); ++it1)
    {
        Var* fld =*it1;
        tinyxml2::XMLElement* ele = doc->NewElement("MsgFld");                  //字段
        ele->SetAttribute("name",fld->strName.c_str());
        ele->SetAttribute("type", getFldTypName(fld->eFldTyp));
        ele->SetAttribute("value", fld->valShow.c_str());
        ele->SetAttribute("varcpp", fld->strVarCpp.c_str());
        ele->SetAttribute("dimension", fld->dbDimension);
        if (fld->getFldTyp() < eBITGRP)                                         //1,普通类型    
            ;
        else {
            Var_BitGrp* fldBitGrp = (Var_BitGrp*)fld;                           //2,位域数组
            vector<Var_Bit*>::iterator it2 = fldBitGrp->vecVar.begin();
            for(;it2 != fldBitGrp->vecVar.end(); ++it2){
                Var_Bit* fld2        =*it2;
                tinyxml2::XMLElement* ele2 = doc->NewElement("MsgFld");             //2.0位字段
                ele2->SetAttribute("name", fld2->strName.c_str());
                ele2->SetAttribute("type", getFldTypName(fld2->eFldTyp));
                ele2->SetAttribute("value", fld2->valShow.c_str());
                ele2->SetAttribute("varcpp", fld2->strVarCpp.c_str());
                map<int, string>::iterator it3= fld2->mapBitRep.begin();
                for(;it3 != fld2->mapBitRep.end(); ++it3)
                {
                    tinyxml2::XMLElement* ele3 = doc->NewElement("BitRep");         //2.0.0位含义
                    ele3->SetAttribute("val", it3->first);
                    ele3->SetAttribute("rep", it3->second.c_str());
                    ele2->InsertEndChild(ele3);
                }
                ele->InsertEndChild(ele2);
            }
        }
        xml->InsertEndChild(ele);
    }
    ele->InsertEndChild(xml);
    return 0;
}


int cvt_xml2msg(tinyxml2::XMLElement* xml, MsgInf** msg)
{
    *msg = new MsgInf(xml->Attribute("name"), atof(xml->Attribute("idx")), xml->Attribute("SttDef"));
    (*msg)->uiLen = xml->IntAttribute("len");
    (*msg)->setIdxInf(xml->IntAttribute("IDpos"),xml->IntAttribute("IDlen"));

    //取各字段值
    XMLElement* eleMsgFld = xml->FirstChildElement("MsgFld");
    while (eleMsgFld)
    {
        //string str = eleMsgFld->Attribute("name");
        if (eleMsgFld->NoChildren())        //普通类型
        {
            Var* fld = newVar(getFldTyp(eleMsgFld->Attribute("type")), eleMsgFld->Attribute("name"),
                                        eleMsgFld->Attribute("value"), eleMsgFld->Attribute("varcpp"),
                                         eleMsgFld->DoubleAttribute("dimension"));
            if (NULL != fld)
                (*msg)->addFld(fld);
        }
        else                                //位域类型
        {
            Var_BitGrp* grp = newVarGrp(getFldTyp(eleMsgFld->Attribute("type")),
                                            eleMsgFld->Attribute("name"), eleMsgFld->Attribute("varcpp"));
            XMLElement* eleMsgFldBit = eleMsgFld->FirstChildElement();
            while (eleMsgFldBit)            //位域字段
            {
                Var_Bit* bit = newVarBit(getFldTyp(eleMsgFldBit->Attribute("type")), eleMsgFldBit->Attribute("name"),
                    eleMsgFldBit->Attribute("value"), eleMsgFldBit->Attribute("varcpp"));
                XMLElement* eleBitRep = eleMsgFldBit->FirstChildElement();
                while (eleBitRep)           //bit位含义
                {
                    bit->setBitRep(eleBitRep->IntAttribute("val"), eleBitRep->Attribute("rep"));
                    eleBitRep = eleBitRep->NextSiblingElement();
                }
                grp->addFld(bit);
                eleMsgFldBit = eleMsgFldBit->NextSiblingElement();
            }
            if (NULL != grp)
                (*msg)->addFld(grp);
        }
        eleMsgFld = eleMsgFld->NextSiblingElement("MsgFld");
    }
    return 0;
}

//将内部数据结构转化为 cpp数据结构
class StrSttDef {
public:
    StrSttDef(string& strStt, string nameHead, string nameTail) 
        :_strStt(strStt), _nameHead(nameHead), _nameTail(nameTail) {
        _strStt += nameHead;
        _indent2 = "\t";
    };        //定义 最基础结构是使用，其他情况不得使用
    StrSttDef(StrSttDef& parent, string nameHead, string nameTail)
        :_strStt(parent._strStt),_indent(parent._indent2), _nameHead(nameHead), _nameTail(nameTail) {
        _strStt += _indent + nameHead ;
        _indent2 = _indent + "\t";
    }
    ~StrSttDef() {
        _strStt += _indent+ _nameTail;
    }
    void setVarDef(string typ, string var)
        {    _strStt += _indent2 + typ + " \t" + var + ";";}
    void setVarNote(string str)
        {    _strStt += "\t//" + str + "\n";}
    string& _strStt;
    string _indent,_indent2, _nameHead,_nameTail;

};
#define note_val2str_Def(fmt,val_typ)                                   \
string note_val2str( string strDst,const val_typ& valSrc) {         \
    char  cs[64];                                                   \
    sprintf(cs, fmt, valSrc);                                       \
    return strDst + ":" + string(cs)+"\t";                          \
};
note_val2str_Def("%d",signed char)
note_val2str_Def("%d",signed short)
note_val2str_Def("%d",signed int)
note_val2str_Def("%d",unsigned char)
note_val2str_Def("%d",unsigned short)
note_val2str_Def("%u",unsigned int)
note_val2str_Def("%f",float)
note_val2str_Def("%f",double)
note_val2str_Def("%s",char*)
//+note_val2str("name", fld->strName.c_str())   //note_val2str_Def 不能适用
inline string note_val2str(string strDst,const string& valSrc){
    return strDst + ":" + valSrc + "\t";
};

inline string int2str(int val){
    char buf[64] = "";
    sprintf(buf, "%d", val);
    return buf;
}
inline int str2int(const string &val){
    return atoi(val.c_str());
}

string cvt_msg2stt(MsgInf* msg)
{
    string strSttDefCpp ="";
    StrSttDef p0 = StrSttDef(strSttDefCpp,
        "typedef struct " + msg->strSttDef + " {\t\t", 
        "} "+ msg->strSttDef +" ;\n");
    strSttDefCpp +="\t//"+ note_val2str("name", msg->strName)
        + note_val2str("idx",msg->uiIdx)
        + note_val2str("len", msg->uiLen)
        +note_val2str("IDlen", msg->uiIDlen)
        +note_val2str("IDpos", msg->uiIDpos)+"\n";

    vector<Var*>::iterator it1 = msg->vecFld.begin();
    for(;it1 != msg->vecFld.end(); ++it1)
    {
        Var* fld =*it1;
        if (fld->getFldTyp() < eBITGRP) {                                                //1,普通类型        
            p0.setVarDef( fld->strVarTyp, fld->strVarCpp );
            p0.setVarNote(
                 note_val2str("name", fld->strName)                                         //字段
                + note_val2str("type", getFldTypName(fld->eFldTyp))
                + note_val2str("value", fld->valShow.c_str())
                + note_val2str("dimension", fld->dbDimension));
        }else {
            Var_BitGrp* fldBitGrp = (Var_BitGrp*)fld;                                    //2,位域数组
            StrSttDef p1 = StrSttDef(p0,
                "struct " + fld->strVarCpp + " {  ", "} " + fld->strVarCpp+ " ;\n");
            p1.setVarNote(
                note_val2str("name", fld->strName)                                          //字段
                + note_val2str("type", getFldTypName(fld->eFldTyp))
                + note_val2str("value", fld->valShow.c_str()));
            vector<Var_Bit*>::iterator it2 = fldBitGrp->vecVar.begin();
            for(;it2 != fldBitGrp->vecVar.end(); ++it2)
            {
                Var_Bit* fld2        =*it2;
                p1.setVarDef(
                    fld->strVarTyp, fld2->strVarCpp+ ":" +int2str(((Var_Bit*)fld2)->lenBit));
                p1.setVarNote(
                    note_val2str("name", fld2->strName)                                     //字段
                    + note_val2str("type", getFldTypName(fld2->eFldTyp))
                    + note_val2str("value", fld2->valShow.c_str()));                    //2.0位字段
                if (fld2->mapBitRep.size() > 0) {
                    StrSttDef p2 = StrSttDef(p1, "\t //", " \n");                           //若有字段值含义
                    map<int, string>::iterator it3= fld2->mapBitRep.begin();
                    for(;it3 != fld2->mapBitRep.end(); ++it3)
                        strSttDefCpp += " " + int2str(it3->first)+ ":" + string(it3->second.c_str());
                }
            }
        }
    }
    //最好对注释行进行格式化
    return strSttDefCpp;
}

//按照 '\t' ' ' '/' 拆分字符串到 vector
void splitStr(const string& s, vector<string>& v)
{
    string tmp = "";
    for(size_t i=0; i<s.size(); i++){
        if ('\t' != s[i] && ' ' != s[i] ) 
            tmp +=s[i];
        else
            if(tmp.size()){
                v.push_back(tmp);
                tmp = "";
            }
    }
    //if (tmp.size())
    //    v.push_back(tmp);
}

string splitStr2(const string& s)
{    
    size_t i = 0;
    string str2;
    while ('\t' != s[i] && ' ' != s[i]) 
            i++;
    for(; i<s.size(); i++){
        if ('\t' != s[i] && ' ' != s[i])
            str2 += s[i];
        else
            str2 += ( !str2.empty() && ' '!=str2[str2.size()-1] ) ? " " : "";
    }
    if(' ' == str2[str2.size()-1])
        str2[str2.size()-1]='\0';
    return str2;
}

void splitString(const string& src, vector<string>& v, const string& cut)
{
  string::size_type pos1, pos2;        //1 2 前后
  pos2 = src.find(cut);
  pos1 = 0;
  while(string::npos != pos2)
  {
    v.push_back(src.substr(pos1, pos2-pos1));
    pos1 = pos2 + cut.size();
    pos2 = src.find(cut, pos1);
  }
  if(pos1 != src.length())
    v.push_back(src.substr(pos1));
}

//获取属性
void splitStrAttr(const string& str, map<string, string>& mapStrInf)
{
    vector<string> vectmp;
    splitString(str, vectmp, " ");
    for(vector<string>::iterator it =vectmp.begin();it != vectmp.end(); ++it) {
        vector<string> vecpair;
        splitString(*it,  vecpair, ":");
        mapStrInf[vecpair[0]] =vecpair.size()>1? vecpair[1]:"";
    }
    return ;
}

void getVarCppName(string& str, string& str2){
    for(size_t i=0; i<str.size(); i++)
        if ('{' == str[i] || ';' == str[i])
            str[i] = ' ';
    vector<string> vectmp;
    splitStr(str, vectmp);    //指定多余两个
    str2 = vectmp[vectmp.size()-1];
    return ;
}

// 通过回执 判断属性 协议, 字段, 位组, 位域, 位域枚举, 闭合定义
//                     1     2     3    4       5        6
int getNameValue(const string& str, map<string, string>& mapStrInf, string& strVarName)
{
    string str2=splitStr2(str);
    vector<string> vectmp;
    splitString(str2, vectmp, "//");
    vector<string>::iterator it =vectmp.begin();    

    if ( 1==vectmp.size() )
    {
        if( string::npos != vectmp[0].find(";"))        //闭合定义
            return 6;
        if( string::npos != vectmp[0].find(":"))        //位域含义 //除掉std::string
        {
            splitStrAttr(vectmp[0], mapStrInf);
            return 5;
        }
    }
    if ( 2==vectmp.size())
    {
         if(0==vectmp[0].size()){
            if (string::npos !=vectmp[1].find("}"))
                return 6;
            splitStrAttr(vectmp[1], mapStrInf);
            return 5;
        }
        if( string::npos != vectmp[1].find("idx:"))    {    //协议定义
            splitStrAttr(vectmp[1], mapStrInf);
            getVarCppName(vectmp[0],strVarName);
            return 1;
        }
        if( string::npos != vectmp[1].find("BITGRP")) {     //位域组合
            splitStrAttr(vectmp[1], mapStrInf);
            getVarCppName(vectmp[0], strVarName);
            return 3;
        }
        if( string::npos != vectmp[0].find("string")) {     // string
            splitStrAttr(vectmp[1], mapStrInf);
            getVarCppName(vectmp[0],strVarName);
            return 2;
        }
        if( string::npos != vectmp[0].find(":")) {          // 位域
            splitStrAttr(vectmp[1], mapStrInf);
            getVarCppName(vectmp[0],strVarName);
            return 4;
        } else {
            splitStrAttr(vectmp[1], mapStrInf);
            getVarCppName(vectmp[0],strVarName);
            return 2;
        }
    }
    return -1;
}


MsgInf* cvt_stt2msg(string  stt)
{
    MsgInf *msg = NULL;
    vector<string> vec;                //map<string, string> name2value;
    splitString(stt, vec, "\n");        //按行存入 方便循环
    vector<string>::iterator it =vec.begin();    //一行不做处理
    for(;it != vec.end(); ++it)
    {
        string strVarCpp;
        map<string, string> mapStrInf;
        int rtn = getNameValue(*it, mapStrInf, strVarCpp) ;
        switch (rtn) {
            case 1:{
                    msg = new MsgInf( mapStrInf["name"], atoi(mapStrInf["idx"].c_str()), strVarCpp);
                    msg->setIdxInf( atoi(mapStrInf["IDpos"].c_str()),atoi(mapStrInf["IDlen"].c_str()));
                }break;
            case 2:{
                    Var* fld = newVar(getFldTyp(mapStrInf["type"]), mapStrInf["name"], mapStrInf["value"], strVarCpp,atof(mapStrInf["dimension"].c_str()));
                    if (NULL != fld)
                        msg->addFld(fld);
                }break;
            case 3:{
                    Var_BitGrp* grp = newVarGrp(getFldTyp(mapStrInf["type"]), mapStrInf["name"], strVarCpp);
                    //进入循环 进行解析 解析控制字
                    Var_Bit* bit;
                    int jie_rtn=4;
                    while(6 != jie_rtn) {
                        jie_rtn = getNameValue(*++it, mapStrInf, strVarCpp="");
                        if ( 4 ==jie_rtn){
                            bit = newVarBit(getFldTyp(mapStrInf["type"]), mapStrInf["name"], mapStrInf["value"], mapStrInf["varcpp"]);
                            grp->addFld(bit);
                            continue;
                        }
                        if ( 5 ==jie_rtn){    
                              for( map<string,string>::iterator it = mapStrInf.begin() ;
                                      it != mapStrInf.end(); ++it)    
                                bit->setBitRep(atoi(it->first.c_str()),it->second);            //bit位含义
                            continue;
                        }
                        if(6 == jie_rtn)
                            break;
                    }
                    if (NULL != grp)
                        msg->addFld(grp);
                }break;
            case 5:
                goto end;
            default: ;
        }
    }
end:
    return msg;
}

using namespace tinyxml2;

int rdMsgCfgXml(string strFile, map<int, MsgInf*>& mapMsgInf)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(strFile.c_str())){
        doc.PrintError();
        exit(1);
    }
 
    XMLElement* root = doc.RootElement();
    //XMLElement* msgXmlALL = root->FirstChildElement("MsgAll");
    XMLElement* msgXml = root->FirstChildElement("Msg");

    while (msgXml)
    {
        MsgInf* tmp = NULL;
        cvt_xml2msg(msgXml,&tmp);
        tmp->show2real();
        mapMsgInf[tmp->getIdx()] = tmp;
        msgXml = msgXml->NextSiblingElement("Msg");
    }

    return 0;
}

int wrMsgCfgXml(string strFile, map<int, MsgInf*>& mapMsgInf)
{
    tinyxml2::XMLDocument doc;

    XMLElement* msgAll = doc.NewElement("MsgAll");

    map<int, MsgInf*>::iterator msgInf= mapMsgInf.begin();
    for(;msgInf != mapMsgInf.end(); ++msgInf)
    {
        cvt_msg2xml(msgInf->second, &doc, msgAll);
        //tinyxml2::XMLDocument tmp;            //只为能顺利创建保存一个    //不可行
        //cvt_msg2xml(msgInf.second,&tmp);
        //tmp.RootElement()->FirstChildElement();
        //msgAll->InsertEndChild(tmp.RootElement());
    }

    doc.InsertEndChild(msgAll);

    XMLError eResult = doc.SaveFile(strFile.c_str());
    if (eResult != XML_SUCCESS)
        cout << "error\n";

    return 0;
}

//typedef struct Ctrl2 {
//    unsigned a : 7;    //默认为4 byte
//    char : 2;          //0自动补全char的剩下的字节    且无变量则不能使用该位
//    char b : 8;        //char b : 9;        //err
//}Ctrl2;
