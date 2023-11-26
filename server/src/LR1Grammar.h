#pragma once
#pragma once
#ifndef LR1_GRAMMAR
#define LR1_GRAMMAR
#include"global.h"
#include"lexer.h"
#define GRAMMARFILE "grammar.txt"//该文件只读

//规范项
struct canonical {
	pair<string, vector<string>> p;//产生式的左部，产生式的右部
	int dot;
	vector<string>expect;
	bool operator==(canonical& c)
	{
		if (p == c.p && dot == c.dot && expect == c.expect)
			return true;
		else
			return false;
	}
};

struct closure {
public:
	vector<canonical>set;
	map<string, int>next;
};

enum actionStatus { ACTION_ERROR, ACTION_STATE, ACTION_REDUCTION, ACTION_ACC };//表示错误，状态，规约和接受
struct actionItem {
public:

	actionStatus status = ACTION_ERROR;
	int nextState = -1;
	pair<string, vector<string>> p;
};


class LR1Grammar {
private:
	string S;//存储输入的字符串或文本
	map<string, vector<string>>FIRST; //存储文法符号的FIRST集合
	map<int, closure>collection;//存储 LR(1) 文法中的项目集规范族
	closure genNext(closure& c, string A);//生成给定项目集 c 的下一个闭包
	vector<string> genX(closure& c);//根据给定项目集 c，生成下一个可能的文法符号
	bool canonicalSetSame(vector<canonical>& a, vector<canonical>& b);//检查两个项目集是否相同
public:
	vector<string>VN;//非终结符集合
	vector<string>VT;//终结符集合
	map<string, vector<vector<string>>>P;//产生式规则
	map<int, vector<actionItem>>ACTION; //存储 LR(1) 分析表中的 ACTION 部分
	map<int, vector<int>>GOTO;// 存储 LR(1) 分析表中的 GOTO 部分
	void readGrammar();//读取文法信息
	void showGrammar();//显示文本内容
	void genFIRST(); //生成文法符号的 FIRST 集合
	void showFIRST();//显示 FIRST 集合内容
	void genCLOSURE(closure& c);//显示 FIRST 集合内容
	void showCLOSURE(closure& c);//显示项目集闭包内容
	void analysisLR1();//执行 LR(1) 分析
	void showLR1();//显示 LR(1) 分析的结果
	void genLR1Table();//显示 LR(1) 分析的结果
	void showTable();//显示 LR(1) 分析表内容
};
#endif // !LR1_GRAMMAR