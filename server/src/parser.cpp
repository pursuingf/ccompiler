#include "parser.h"


parser::~parser()
{
	if (reductionTreeRoot != NULL)
	{
		vector<treeNode*> idstack;
		int nowlevel = reductionTreeRoot->level;
		idstack.push_back(reductionTreeRoot);
		while (!idstack.empty())
		{
			treeNode* nownode = idstack.front();
			idstack.erase(idstack.begin());
			for (int i = 0; i < nownode->children.size(); i++)
			{
				idstack.push_back(nownode->children[i]);
			}
			delete nownode;
		}
	}
}
void parser::initializeLR1()
{
	G.readGrammar();
	//G.showGrammar();
	G.genFIRST();
	//G.showFIRST();
	G.analysisLR1();
	//G.showLR1();
	G.genLR1Table();
	//G.showTable();
}
void parser::getInput(string input)
{
	L.setCodeString(input);
}
void parser::analysis()
{
	vector<int>state;//模拟语法分析的状态栈
	vector<pair<string, int>>symbol;//模拟语法分析的符号栈
	state.push_back(0);
	symbol.push_back(pair<string, int>(string("#"), -1));
	pair<string, int> epsilon_lexis = pair<string, int>("epsilon", -1);
	pair<string, int> epsilon_next_lexis;
	pair<string, int> lexis = L.lexicalAnalysis();//当前分析到的词法单元
	codeline = 1;
	stack<treeNode*> treeNodeStack;//用于构建语法树的栈结构
	treeNode* tp;

	//用于记录分析过程中状态、符号和输入的变化
	vector<string> strstate;
	vector<string> strsymbol;
	vector<string> input;
	do
	{
		if (lexis.first == "NL")
		{
			codeline++;
			lexis = L.lexicalAnalysis();
			continue;
		}
		//检查当前词法单元是否是终结符
		if (find(G.VT.begin(), G.VT.end(), lexis.first) == G.VT.end())
		{
			//cerr << "ERROR: " << lexis.first << "is not belong to VT" << endl;
			//cerr << "ERROR: " << lexis.first << "不是终结符" << endl;
			string expmsg = string("ERROR: 语法分析器发现") + lexis.first + string("不是终结符\n");
			throw expmsg;
		}

		//根据当前状态和当前词法单元从分析表中获取相应的操作
		actionItem item = G.ACTION[state.back()][find(G.VT.begin(), G.VT.end(), lexis.first) - G.VT.begin()];
		/*vector<actionItem> state_actions;
		vector<string> prod;*/
		switch (item.status)
		{
			//接受状态
		case ACTION_ACC:
			_genTreeLevel(reductionTreeRoot, 0);
			return;
			break;
		case ACTION_ERROR:
			if (lexis != epsilon_lexis)
			{
				epsilon_next_lexis = lexis;
				lexis = epsilon_lexis;
			}
			else
			{
				string expmsg;
				expmsg = string("ERROR: 语法分析器扫描到第") + to_string(codeline) + string("行的单词") + lexis.first + string("时发生错误\n");
				expmsg += string("目前符号栈内容为\n|---stack---\n");
				while (!symbol.empty())
				{
					expmsg += string("|---") + symbol.back().first + string("\n");
					symbol.pop_back();
				}
				expmsg += string("|---stack---\n");
				throw expmsg;
			}
			break;
			//如果是移入状态，则将当前词法单元添加到语法树节点栈和状态栈中
		case ACTION_STATE:
			tp = new(nothrow)treeNode;
			if (!tp) { exit(-2); }
			tp->content = lexis;
			treeNodeStack.push(tp);

			state.push_back(item.nextState);
			symbol.push_back(lexis);

			if (lexis == epsilon_lexis)
			{
				lexis = epsilon_next_lexis;
			}
			else
			{
				lexis = L.lexicalAnalysis();
			}
			break;
			//如果是规约状态
		case ACTION_REDUCTION:
		{
			//用于检查当前符号栈是否与产生式右侧匹配
			for (int i = item.p.second.size() - 1; i >= 0; i--)
			{
				//如果符号栈顶部的符号与产生式右部对应位置的符号相等
				if (symbol.back().first == item.p.second[i])
				{
					//从符号栈和状态栈中分别移除一个符号和一个状态
					symbol.pop_back();
					state.pop_back();
				}
				else
				{
					//cerr << "ERROR: an error has occured when " << symbol.back().first << "is not equal with " << item.p.second[i] << endl;
					//cerr << "ERROR: 发生了一个规约错误：" << symbol.back().first << "与" << item.p.second[i] << "不相同\n";
					//cerr << "you can correct the error according to the following production\n";
					//cerr << "你可以参考以下产生式改正错误\n";
					//cerr << item.p.first << "->";
					//for (auto k = 0; k < item.p.second.size(); k++)
					//{
					//	cout << item.p.second[k] << ' ';
					//}

					string expmsg;
					expmsg = string("ERROR: 语法分析器扫描到第") + to_string(codeline) + string("行的单词") + lexis.first + string("时发生错误\n");
					expmsg += string("发生了一个规约错误：") + symbol.back().first + string("与") + item.p.second[i] + string("不相同\n");
					expmsg += string("参考以下产生式改正错误\n");
					expmsg += item.p.first + string("->");
					for (auto k = 0; k < item.p.second.size(); k++)
					{
						expmsg += item.p.second[k] + string(" ");
					}
					expmsg += string("\n");
					throw expmsg;
				}
			}
			tp = new(nothrow)treeNode;
			if (!tp) { exit(-2); }
			tp->content = pair<string, int>(item.p.first, -1);
			for (int i = 0; i < item.p.second.size(); i++)
			{
				treeNodeStack.top()->parent = tp;
				tp->children.push_back(treeNodeStack.top());
				treeNodeStack.pop();
			}
			reverse(tp->children.begin(), tp->children.end());
			reductionTreeRoot = tp;
			treeNodeStack.push(tp);

			//补充语义分析

			symbol.push_back(pair<string, int>(item.p.first, -1));
			state.push_back(G.GOTO[state.back()][find(G.VN.begin(), G.VN.end(), symbol.back().first) - G.VN.begin()]);
			break;
		}
		default:break;
		}

		strstate.clear();
		strsymbol.clear();
		input.clear();
		for (int i = 0; i < state.size(); i++)
		{
			strstate.push_back(to_string(state[i]));
		}
		for (int i = 0; i < symbol.size(); i++)
		{
			strsymbol.push_back(symbol[i].first);
		}
		input.push_back(lexis.first);
		/*for (int i = pos; i < inputString.size(); i++)
		{
			input.push_back(inputString[i].first);
		}*/
		history.push_back({ strstate, strsymbol, input });
	} while (lexis.first != "ERROR");//直到词法分析返回error为止
}
void parser::showHistory()
{
	for (int i = 0; i < history.size(); i++)
	{
		cout << "---Step:" << i << "---";
		for (int j = 0; j < history[i].size(); j++)
		{
			for (int k = 0; k < history[i][j].size(); k++)
			{
				cout << history[i][j][k] << " ";
			}
			cout << "\t\t\t";
		}
		cout << endl;
	}
}
//记录每个节点的深度，并统计叶子节点的数量
void parser::_genTreeLevel(treeNode* nownode, int nowlevel)
{
	if (nowlevel > maxTreeLevel)
		maxTreeLevel = nowlevel;
	if (nownode->children.size() == 0)
		leafNum++;
	nownode->level = nowlevel;
	for (int i = 0; i < nownode->children.size(); i++)
	{
		_genTreeLevel(nownode->children[i], nowlevel + 1);
	}
}
void parser::showTree()
{
	cout << "----------------ReductionTree-------------------\n";
	vector<treeNode*> idstack;
	int nowlevel = reductionTreeRoot->level;
	idstack.push_back(reductionTreeRoot);
	while (!idstack.empty())
	{
		treeNode* nownode = idstack.front();
		idstack.erase(idstack.begin());
		if (nownode->level == nowlevel)
			cout << nownode->level << ' ';
		else
		{
			cout << '\n' << nownode->level << ' ';
			nowlevel++;
		}
		for (int i = 0; i < nownode->children.size(); i++)
		{
			idstack.push_back(nownode->children[i]);
		}
	}
	cout << "----------------End-------------------\n";
}
void parser::_showTree2(treeNode* nownode)
{
	for (int i = 0; i < nownode->level; i++)
		cout << string("|--");
	cout << "< " << nownode->content.first << ", ";
	if (nownode->content.first == string("ID"))
	{
		cout << L.identifierTable.find(nownode->content.second)->second;
	}
	else {
		cout << nownode->content.second;
	}
	cout << " >" << endl;
	for (int i = 0; i < nownode->children.size(); i++)
		_showTree2(nownode->children[i]);
}
void parser::showTree2()
{
	cout << "----------------ReductionTree2-------------------\n";
	_showTree2(reductionTreeRoot);
	cout << "----------------End-------------------\n";
}
void parser::showTreeToFile(const std::string& filename) {
	std::ofstream outputFile(filename); // 打开文件流
	if (outputFile.is_open()) {
		outputFile << "----------------ReductionTree-------------------\n";
		std::vector<treeNode*> idstack;
		int nowlevel = reductionTreeRoot->level;
		idstack.push_back(reductionTreeRoot);
		while (!idstack.empty()) {
			treeNode* nownode = idstack.front();
			idstack.erase(idstack.begin());
			if (nownode->level == nowlevel)
				outputFile << nownode->level << ' ';
			else {
				outputFile << '\n' << nownode->level << ' ';
				nowlevel++;
			}
			for (int i = 0; i < nownode->children.size(); i++) {
				idstack.push_back(nownode->children[i]);
			}
		}
		outputFile << "\n----------------End-------------------\n";
		outputFile.close(); // 关闭文件流
	}
	else {
		std::cerr << "Unable to open file: " << filename << std::endl;
	}
}
void parser::_showTreeToString(treeNode* nownode,string& ret)
{	
	ret += "[";
	ret += "\"" + to_string(nownode->level) + "\",";
	ret += "\"" + nownode->content.first + "\",";
	if (nownode->content.first == string("ID"))
	{
		ret += "\"" + L.identifierTable.find(nownode->content.second)->second + "\"";
	}
	else {
		ret += "\"" + to_string(nownode->content.second) + "\"";
	}
	ret += "],";
	for (int i = 0; i < nownode->children.size(); i++)
		_showTreeToString(nownode->children[i],ret);
	
}
string parser::showTreeToString() 
{
	string ret("");
	_showTreeToString(reductionTreeRoot, ret);
	ret.pop_back();
	//cout << ret;
	return ret;
}
void  parser::getResult(string& result)
{
	result = "";
	//action goto表
	
	result += "[[";
	result += "[";
	for (auto i = 0; i < G.VT.size(); i++)
		result += "\""+G.VT[i] + "\"" + ",";
	for (auto i = 0; i < G.VN.size(); i++) {
		result += "\"" + G.VN[i] + "\"";
		if (i != G.VN.size() - 1)
			result += ",";
	}
	result += "],";

	
	for (auto i = 0; i < G.ACTION.size(); i++)
	{
		result += "[";
		for (auto j = 0; j < G.ACTION[i].size(); j++)
		{
			switch (G.ACTION[i][j].status)
			{
			case ACTION_ACC:result += "\"ACC\","; break;
			case ACTION_STATE:result += ("\"s" + to_string(G.ACTION[i][j].nextState)+"\","); break;
			case ACTION_REDUCTION:
			{
				string context;
				context = "r:" + G.ACTION[i][j].p.first + "->";
				for (int k = 0; k < G.ACTION[i][j].p.second.size(); k++)
				{
					context += G.ACTION[i][j].p.second[k] + " ";
				}
				result += "\""+context+ "\",";
				break;
			}
			case ACTION_ERROR: result += "\"\","; break;
			default:result += "\"\","; break;
			}
		}
		for (auto j = 0; j <G.GOTO[i].size(); j++)
		{
			if (G.GOTO[i][j] != -1) {
				result += "\"" + to_string(G.GOTO[i][j]) + "\"";
			}
			else {
				result += "\"\"";
			}
			if (j != G.GOTO[i].size() - 1)
				result += ",";
		}
		result += "]";
		if (i != G.ACTION.size()- 1)
			result += ",";
	}
	result += "],";
	// 移进规约栈
	result += "[";
	for (auto i = 0; i < history.size(); i++)
	{
		string context1;
		if (history[i][0].size() == 0) context1 = "";
		else if (history[i][0].size() == 1) context1 = history[i][0][0];
		else if (history[i][0].size() == 2) context1 = history[i][0][0] + " " + history[i][0][1];
		else context1 = "..." + history[i][0][history[i][0].size() - 2] + " " + history[i][0][history[i][0].size() - 1];
		result += "[";
		result+= "\""+context1 +"\"" +",";

		string context2;
		if (history[i][1].size() == 0) context2 = "";
		else if (history[i][1].size() == 1) context2 = history[i][1][0];
		else if (history[i][1].size() == 2) context2 = history[i][1][0] + " " + history[i][1][1];
		else context2 = "..." + history[i][1][history[i][1].size() - 2] + " " + history[i][1][history[i][1].size() - 1];
		//cout << context2.data() << ' ';
		result += "\""+context2 +  "\""+",";
		if (history[i][2][0] == "#")
			result += "\""+(history[i][2][0])+ "\"";
		else
			result += "\""+(history[i][2][0] + "...")+ "\"";
		result += "]";
		if (i != history.size() - 1)
			result += ",";
	}
	result += "],[";

	result += showTreeToString();

	result += "]]";

	//cout << result << endl;
}

