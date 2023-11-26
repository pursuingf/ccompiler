#pragma once
#ifndef LEXCAL_ANALYSIS
#define LEXCAL_ANALYSIS
#include "global.h"
using namespace std;
class lexer
{
private:
	//��c�ʷ�����
	//��Ҫ�������ַ���
	vector<string> skip = { " ","\t","\r" };
	//һԪ�����
	vector<string> unaryOP = { "+","-","&","|","^","*","/","<",">","=",";",",","(",")","[","]","{","}", "!" }; 
	//��Ԫ�����
	vector<string> binaryOP = { "<=", "!=", "==", ">=" ,"&&","||" };
	//ϵͳ������
	vector<string> keyword = { "int", "void", "while", "if", "else", "return" }; 
	//һЩ�ؼ��ֺͲ������Ķ�Ӧtoken
	map<string, pair<string, int>> strToken = {
		pair<string,pair<string,int>>("int",pair<string,int>("INT",-1)),
		pair<string,pair<string,int>>("void",pair<string,int>("VOID",-1)),
		pair<string,pair<string,int>>("id",pair<string,int>("ID",-1)),
		pair<string,pair<string,int>>("(",pair<string,int>("LP",-1)),
		pair<string,pair<string,int>>(")",pair<string,int>("RP",-1)),
		pair<string,pair<string,int>>("[",pair<string,int>("LS",-1)),
		pair<string,pair<string,int>>("]",pair<string,int>("RS",-1)),
		pair<string,pair<string,int>>("{",pair<string,int>("LB",-1)),
		pair<string,pair<string,int>>("}",pair<string,int>("RB",-1)),
		pair<string,pair<string,int>>("!",pair<string,int>("NOT",-1)),
		pair<string,pair<string,int>>("while",pair<string,int>("WHILE",-1)),
		pair<string,pair<string,int>>("if",pair<string,int>("IF",-1)),
		pair<string,pair<string,int>>("else",pair<string,int>("ELSE",-1)),
		pair<string,pair<string,int>>("return",pair<string,int>("RETURN",-1)),
		pair<string,pair<string,int>>("=",pair<string,int>("ASSIGN",-1)),
		pair<string,pair<string,int>>("+",pair<string,int>("OP1",0)),
		pair<string,pair<string,int>>("-",pair<string,int>("OP1",1)),
		pair<string,pair<string,int>>("&",pair<string,int>("OP1",2)),
		pair<string,pair<string,int>>("|",pair<string,int>("OP1",3)),
		pair<string,pair<string,int>>("^",pair<string,int>("OP1",4)),
		pair<string,pair<string,int>>("*",pair<string,int>("OP2",0)),
		pair<string,pair<string,int>>("/",pair<string,int>("OP2",1)),
		pair<string,pair<string,int>>("<",pair<string,int>("RELOP",0)),
		pair<string,pair<string,int>>("<=",pair<string,int>("RELOP",1)),
		pair<string,pair<string,int>>(">",pair<string,int>("RELOP",2)),
		pair<string,pair<string,int>>(">=",pair<string,int>("RELOP",3)),
		pair<string,pair<string,int>>("==",pair<string,int>("RELOP",4)),
		pair<string,pair<string,int>>("!=",pair<string,int>("RELOP",5)),
		pair<string,pair<string,int>>("||",pair<string,int>("OR",-1)),
		pair<string,pair<string,int>>("&&",pair<string,int>("AND",-1)),
		pair<string,pair<string,int>>(";",pair<string,int>("DEL",-1)),
		pair<string,pair<string,int>>(",",pair<string,int>("SEP",-1)),
		pair<string,pair<string,int>>("\n",pair<string,int>("NL",-1))
	};


	//�ʷ�������״̬��״̬��
	enum state { INIT, singleLineComments, multiLineComments, Num, Op, Str, MYERROR };
	state S = INIT;	//�ʷ���������ʼ״̬


	string sourceCode;	//Դ�����е��ַ���
	size_t pos = 0;		//��ǰ�����ַ����±�
	int symbol_count = 0;//�Ѿ��������ı�ʶ������

public:
	int codeline = 1;//��¼��ǰ������������

	/// <summary>
	/// ��Ҫչʾ�ľ��������������ʷ������Ľ��
	/// </summary>
	/// 
	map<int, string> identifierTable;//��ʶ����
	vector<pair<string, int>> lexicalUnit;//��������ʷ���Ԫ



	void setCodeString(string fstring);
	pair<string, int> lexicalAnalysis();
	void getResult(string& result);

};
void showIdentifierTable(const map<int, string>& identifierTable);
void showLexicalUnit(const vector<pair<string, int>>& lexicalUnit);
#endif // !LEXCAL_ANALYSIS

