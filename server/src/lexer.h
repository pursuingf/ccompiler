#pragma once
#ifndef LEXCAL_ANALYSIS
#define LEXCAL_ANALYSIS
#include "global.h"
using namespace std;
class lexer
{
private:
	//类c词法规则
	//需要跳过的字符集
	vector<string> skip = { " ","\t","\r" };
	//一元运算符
	vector<string> unaryOP = { "+","-","&","|","^","*","/","<",">","=",";",",","(",")","[","]","{","}", "!" }; 
	//二元运算符
	vector<string> binaryOP = { "<=", "!=", "==", ">=" ,"&&","||" };
	//系统保留字
	vector<string> keyword = { "int", "void", "while", "if", "else", "return" }; 
	//一些关键字和操作符的对应token
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


	//词法分析器状态机状态集
	enum state { INIT, singleLineComments, multiLineComments, Num, Op, Str, MYERROR };
	state S = INIT;	//词法分析器初始状态


	string sourceCode;	//源代码中的字符流
	size_t pos = 0;		//当前读到字符的下标
	int symbol_count = 0;//已经分析到的标识符计数

public:
	int codeline = 1;//记录当前分析到的行数

	/// <summary>
	/// 需要展示的就是这两个表——词法分析的结果
	/// </summary>
	/// 
	map<int, string> identifierTable;//标识符表
	vector<pair<string, int>> lexicalUnit;//分析结果词法单元



	void setCodeString(string fstring);
	pair<string, int> lexicalAnalysis();
	void getResult(string& result);

};
void showIdentifierTable(const map<int, string>& identifierTable);
void showLexicalUnit(const vector<pair<string, int>>& lexicalUnit);
#endif // !LEXCAL_ANALYSIS

