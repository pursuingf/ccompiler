#include "LR1Grammar.h"

void LR1Grammar::readGrammar()
{
	ifstream f(GRAMMARFILE, ios::in);
	if (!f.is_open())
	{

		throw string("ERROR: 内部错误:无法打开文法文件\n");
	}
	string buf;
	string tmpV;
	pair<string, vector<vector<string>>> tmpP;
	vector<string> branch;//用来存储产生式的右侧部分的一个分支
	bool flag = false;
	while (!f.eof())
	{
		getline(f, buf);
		flag = false;//用于判断当前是否在产生式的左侧
		tmpP.first = string("");
		tmpP.second.clear();
		branch.clear();
		for (auto i = 0; i < buf.size(); i++)
		{
			switch (buf[i])
			{
				//如果是"<"，表示开始处理非终结符
			case '<':
				tmpV.clear();
				i++;
				while (buf[i] != '>') { tmpV += buf[i]; i++; }
				VN.push_back(tmpV);
				if (!flag)
					tmpP.first = tmpV;
				else
					branch.push_back(tmpV);
				break;
				//如果是"\"，表示开始处理终结符
			case '\'':
				tmpV.clear();
				i++;
				while (buf[i] != '\'') { tmpV += buf[i]; i++; }
				VT.push_back(tmpV);
				branch.push_back(tmpV);
				break;
				//如果当前字符是":"，表示产生式左侧结束
			case ':':
				while (buf[i] != '=') { tmpV += buf[i]; i++; }
				flag = true;
				break;
				//如果当前字符是"|"，表示当前产生式右侧的一个分支结束
			case '|':
				tmpP.second.push_back(branch);
				branch.clear();
				break;
			default:break;
			}
		}
		tmpP.second.push_back(branch);
		branch.clear();
		P.insert(tmpP);
		tmpP.first = string("");
		tmpP.second.clear();
	}
	//设置文法的开始符号
	S = VN[0];
	//对非终结符集合 VN 进行排序，使得其中的符号按照字典顺序排列
	sort(VN.begin(), VN.end());
	//使用 unique 函数去除重复的非终结符，并返回去重后的新的终结符的末尾迭代器
	vector<string>::iterator pos = unique(VN.begin(), VN.end());
	VN.erase(pos, VN.end());
	sort(VT.begin(), VT.end());
	pos = unique(VT.begin(), VT.end());
	VT.erase(pos, VT.end());
	VT.push_back(string("#"));//将特殊的结束符"#"加入到终结符集合"VT"的尾部
	f.close();
	if (VN.size() != P.size())
	{
		//cerr << "ERROR: the number of productions is not equal with the number of VN\n";
		//cerr << "ERROR: 产生式与非终结符数目不对应\n";
		//exit(-1);
		throw string("ERROR: 内部错误:文法的产生式与非终结符数目不对应\n");
	}
}
void LR1Grammar::showGrammar()
{
	cout << "----------------LR1Grammar-------------------\n";
	cout << "----------------VN-------------------\n";
	for (int i = 0; i < VN.size(); i++)
		cout << VN[i] << ' ';
	cout << "\n----------------VT-------------------\n";
	for (int i = 0; i < VT.size(); i++)
		cout << VT[i] << ' ';
	cout << "\n----------------S-------------------\n";
	cout << S << endl;
	cout << "----------------P-------------------\n";
	map<string, vector<vector<string>>>::iterator p;
	for (p = P.begin(); p != P.end(); p++)
	{
		cout << p->first << "->";
		for (auto j = 0; j < p->second.size(); j++)
		{
			for (auto k = 0; k < p->second[j].size(); k++)
			{
				cout << p->second[j][k] << ' ';
			}
			cout << '|';
		}
		cout << endl;
	}
	cout << "----------------End-------------------\n";
}
//FIRST集合
void LR1Grammar::genFIRST()
{
	//each VT对于每一个终结符，把自己加入对应的FIRST集合中
	for (int i = 0; i < VT.size(); i++)
		FIRST[VT[i]] = { VT[i] };

	bool ischange = true;
	//这个过程会持续直到ischange为false,表示FIRST集合不再变化，没有新的终结符被添加进来，计算结束。
	while (ischange)
	{
		ischange = false;//用于追踪在计算FIRST集合时是否发生了变化
		//each VN
		map<string, vector<vector<string>>>::iterator p;
		for (p = P.begin(); p != P.end(); p++)
		{
			string X = p->first;
			//如果该非终结符不在FIRST集合中，先将其初始化为空集
			if (FIRST.find(X) == FIRST.end())
				FIRST[X] = {};
			//each branch
			for (int j = 0; j < p->second.size(); j++)
			{
				//如果产生式右侧不为空
				if (p->second[j].size() > 0)
				{
					//取产生式右边的第一个符号Y
					string Y = p->second[j][0];
					//将Y的FIRST集合中的元素逐个添加到当前非终结符X的FIRST集合中，如果有新元素加入，则标记ischange为true，表示发生了变化。
					for (int r = 0; r < FIRST[Y].size(); r++)
					{
						if (count(FIRST[X].begin(), FIRST[X].end(), FIRST[Y][r]) == 0)
						{
							FIRST[X].push_back(FIRST[Y][r]);
							ischange = true;
						}
					}
				}
				//如果产生式右部为空，抛出错误信息
				else
				{
					//cerr << "ERROR: there is a null production\n";
					//cerr << "ERROR: 存在右部为空的产生式\n";
					//exit(-1);
					throw string("ERROR: 内部错误:文法存在右部为空的产生式\n");
				}
			}
		}
	}
}
void LR1Grammar::showFIRST()
{
	cout << "----------------FIRST-------------------\n";
	map<string, vector<string>>::iterator p;
	for (p = FIRST.begin(); p != FIRST.end(); p++)
	{
		cout << "FIRST[" << p->first << "]={";
		for (auto j = 0; j < p->second.size(); j++)
		{
			cout << p->second[j] << ',';
		}
		cout << "}\n";
	}
	cout << "----------------End-------------------\n";
}
//用于生成闭包(A→α⋅Xβ)
/*
* 根据点后的符号，将对应非终结符的产生式加入到当前项集的闭包中，
并根据该符号后的符号串来构建展望集合。
在添加新项之前，会检查是否已存在相同的项，若已存在，则只更新该项的展望集合。
*/
void LR1Grammar::genCLOSURE(closure& c)
{
	//each canonical
	for (int i = 0; i < c.set.size(); i++)
	{
		//如果当前项已经到达了产生式右部的末尾，则跳过该项，它无法展开
		if (c.set[i].dot >= c.set[i].p.second.size())
			continue;
		//获取当前项中点后面的符合
		string dotV = c.set[i].p.second[c.set[i].dot];
		//如果dotV是VN
		if (count(VN.begin(), VN.end(), dotV) != 0)
		{
			//检查dotV的每一条产生式
			for (int j = 0; j < P[dotV].size(); j++)
			{
				canonical newcan;
				newcan.p = pair<string, vector<string>>(dotV, P[dotV][j]);
				newcan.dot = 0;

				//遍历dotV后的第一个符号
				int k = c.set[i].dot + 1;
				//如果遍历完成，将set[i]的expect加入到新的expect
				if (k == c.set[i].p.second.size())
					newcan.expect.insert(newcan.expect.end(), c.set[i].expect.begin(), c.set[i].expect.end());
				//否则加入FIRST[beta]
				else
				{
					string v = c.set[i].p.second[k];
					for (int r = 0; r < FIRST[v].size(); r++)
					{
						newcan.expect.push_back(FIRST[v][r]);
					}
				}
				//去重
				bool isrepeat = false;
				for (int t = 0; t < c.set.size(); t++)
				{
					if (c.set[t].p == newcan.p && c.set[t].dot == newcan.dot)
					{
						isrepeat = true;
						c.set[t].expect.insert(c.set[t].expect.end(), newcan.expect.begin(), newcan.expect.end());
						vector<string>::iterator pos = unique(c.set[t].expect.begin(), c.set[t].expect.end());
						c.set[t].expect.erase(pos, c.set[t].expect.end());
						sort(c.set[t].expect.begin(), c.set[t].expect.end());
						break;
					}
				}
				if (!isrepeat)
				{
					sort(newcan.expect.begin(), newcan.expect.end());
					c.set.push_back(newcan);
				}
			}
		}
	}
}
void LR1Grammar::showCLOSURE(closure& c)
{
	cout << "----------------CLOSURE_SET-------------------\n";
	for (int i = 0; i < c.set.size(); i++)
	{
		cout << c.set[i].p.first << "->";
		for (int j = 0; j < c.set[i].p.second.size(); j++)
		{
			if (j == c.set[i].dot)
				cout << "·";
			cout << c.set[i].p.second[j] << ' ';
		}
		if (c.set[i].dot == c.set[i].p.second.size())
			cout << "·";
		cout << " --- ";
		for (int j = 0; j < c.set[i].expect.size(); j++)
			cout << c.set[i].expect[j] << ' ';
		cout << endl;
	}
	cout << "----------------CLOSURE_NEXT-------------------\n";
	map<string, int>::iterator p;
	for (p = c.next.begin(); p != c.next.end(); p++)
	{
		cout << p->first << "->" << p->second << endl;
	}
	cout << "----------------End-------------------\n";
}
//生成给定项目集 c 的下一个闭包
closure LR1Grammar::genNext(closure& c, string A)
{
	closure ret = {
		{
		},
		{
		}
	};
	for (int i = 0; i < c.set.size(); i++)
	{
		if (c.set[i].dot < c.set[i].p.second.size() && c.set[i].p.second[c.set[i].dot] == A)
		{
			canonical newcan = c.set[i];
			newcan.dot++;
			ret.set.push_back(newcan);
		}
	}
	return ret;
}
//根据给定项目集 c，生成下一个可能的文法符号
vector<string> LR1Grammar::genX(closure& c)
{
	vector<string> ret;
	for (int i = 0; i < c.set.size(); i++)
	{
		if (c.set[i].dot < c.set[i].p.second.size())
			ret.push_back(c.set[i].p.second[c.set[i].dot]);
	}
	vector<string>::iterator pos = unique(ret.begin(), ret.end());//对ret进行去重
	ret.erase(pos, ret.end());
	return ret;
}
//用于比较两个canonical是否相同
bool LR1Grammar::canonicalSetSame(vector<canonical>& a, vector<canonical>& b)
{
	if (a.size() != b.size())
		return false;
	else
	{
		for (int i = 0; i < a.size(); i++)
		{
			bool flag = false;
			for (int j = 0; j < b.size(); j++)
			{
				if (a[i] == b[j])
				{
					flag = true;
					break;
				}
			}
			if (!flag)
				return false;
		}
		return true;
	}
}
//生成 LR(1) 项目集规范族
void LR1Grammar::analysisLR1()
{
	//用于计数，追踪项目集的数量
	int count = 0;

	//栈用于跟踪待处理的项目集
	stack<int> wait;
	//初始化起始的项目集，即 S' -> S#
	closure I = {
		{
			{pair<string, vector<string>>(string("S'"),{S}),0,{string("#")}}
		},
		{
		}
	};
	genCLOSURE(I);
	collection[count] = I;
	wait.push(count);
	count++;
	while (!wait.empty())
	{
		int now = wait.top();
		wait.pop();
		vector<string>XList = genX(collection[now]);//获取当前项目集所有可能的后继符号
		for (int i = 0; i < XList.size(); i++)
		{
			closure newclo = genNext(collection[now], XList[i]);
			genCLOSURE(newclo);

			int exist = -1;
			map< int, closure >::iterator p;
			for (p = collection.begin(); p != collection.end(); p++)
			{
				if (canonicalSetSame(p->second.set, newclo.set))
				{
					exist = p->first;
					break;
				}
			}
			if (exist == -1)
			{
				collection[now].next[XList[i]] = count;
				collection[count] = newclo;
				wait.push(count);
				count++;
			}
			else
			{
				collection[now].next[XList[i]] = exist;
			}
		}
	}
}
void LR1Grammar::showLR1()
{
	map< int, closure >::iterator p;
	for (p = collection.begin(); p != collection.end(); p++)
	{
		cout << "**********************************************\n";
		cout << "No:" << p->first << endl;
		showCLOSURE(p->second);
		cout << "**********************************************\n";
	}
}
//生成ACTION表和GOTO表
void LR1Grammar::genLR1Table()
{
	map<int, closure>::iterator p;
	for (p = collection.begin(); p != collection.end(); p++)
	{
		vector<actionItem> newaction(VT.size());
		vector<int>newgoto(VN.size(), -1);
		vector<canonical>set = p->second.set;
		map<string, int>next = p->second.next;
		map<string, int>::iterator q;
		for (q = next.begin(); q != next.end(); q++)
		{
			vector<string>::iterator pos;
			if ((pos = find(VN.begin(), VN.end(), q->first)) != VN.end())
			{
				newgoto[pos - VN.begin()] = q->second;
			}
			else if ((pos = find(VT.begin(), VT.end(), q->first)) != VT.end())
			{
				newaction[pos - VT.begin()].status = ACTION_STATE;
				newaction[pos - VT.begin()].nextState = q->second;
			}
			else
			{
				//cerr << "ERROR: an error has occured when generating ACTION/GOTO table\n";
				//cerr << "ERROR: 生成ACTION/GOTO表时遇到问题\n";
				//exit(-1);
				throw string("ERROR: 内部错误:生成ACTION/GOTO表时遇到问题\n");
			}
		}
		for (int i = 0; i < set.size(); i++)
		{
			if (set[i].dot == set[i].p.second.size())
			{
				if (set[i].p.first == string("S'"))
				{
					(newaction.end() - 1)->status = ACTION_ACC;
				}
				else
				{
					canonical tmpcan = set[i];
					for (int j = 0; j < tmpcan.expect.size(); j++)
					{
						vector<string>::iterator pos = find(VT.begin(), VT.end(), tmpcan.expect[j]);
						newaction[pos - VT.begin()].status = ACTION_REDUCTION;
						newaction[pos - VT.begin()].p = tmpcan.p;
					}
				}
			}
		}
		ACTION[p->first] = newaction;
		GOTO[p->first] = newgoto;
	}
}
void LR1Grammar::showTable()
{
	if (ACTION.size() != GOTO.size())
	{
		//cerr << "ERROR: the ACTION table can not match GOTO table\n";
		//cerr << "ERROR: ACTION表和GOTO表不对应\n";
		//exit(-1);
		throw string("ERROR: 内部错误:ACTION表和GOTO表不对应\n");
	}
	map<int, vector<actionItem>>::iterator p;
	for (p = ACTION.begin(); p != ACTION.end(); p++)
	{
		cout << p->first << '\t';
		for (int i = 0; i < p->second.size(); i++)
		{
			switch (p->second[i].status)
			{
			case ACTION_ACC:cout << "ACC\t"; break;
			case ACTION_STATE:cout << "s" << p->second[i].nextState << "\t"; break;
			case ACTION_REDUCTION:
				cout << "r:" << p->second[i].p.first << "->";
				for (int j = 0; j < p->second[i].p.second.size(); j++)
				{
					cout << p->second[i].p.second[j] << ' ';
				}
				break;
			case ACTION_ERROR:cout << "\t"; break;
			default:break;
			}
			cout << "\t";
		}
		cout << '\t';
		for (int i = 0; i < GOTO[p->first].size(); i++)
			cout << GOTO[p->first][i] << ' ';
		cout << endl;
	}
}