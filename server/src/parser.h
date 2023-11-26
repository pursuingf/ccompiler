#pragma once
#ifndef SYNTAX_ANALYSIS
#define SYNTAX_ANALYSIS
#include"global.h"
#include"lexer.h"
#include"LR1Grammar.h"
#define GRAMMARFILE "grammar.txt"//该文件只读


struct treeNode {
	int level = 0;
	treeNode* parent = NULL;
	vector<treeNode*> children;
	pair<string, int> content;


	treeNode() { clear(); }
	void clear()
	{
		level = -1;
		parent = NULL;
		children.clear();
		content = pair<string, int>("", -1);
	}
};

class parser
{
private:
	int codeline;//代码行号
	void _genTreeLevel(treeNode* nownode, int nowlevel);
	void _showTree2(treeNode* nownode);
	void _showTreeToString(treeNode* nownode, string&);
public:
	LR1Grammar G;
	lexer L;
	vector<vector<vector<string>>> history;
	treeNode* reductionTreeRoot = NULL;
	int maxTreeLevel = 0;
	int leafNum = 0;
	
	void initializeLR1();
	void getInput(string input);
	void analysis();
	void showHistory();
	void showTree();
	void showTree2();
	void showTreeToFile(const std::string& filename);
	void getResult(string& result);
	string showTreeToString();
	
	~parser();
};
#endif // !SYNTAX_ANALYSIS
