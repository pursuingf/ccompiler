#include "lexer.h"

/// <summary>
/// 设置设置源代码字符串
/// </summary>
/// <param name="fstring"></param>
void lexer::setCodeString(string fstring)
{
	sourceCode = fstring;
	cout<<sourceCode.size()<<endl;
}

/// <summary>
/// 使用状态机来扫描源代码字符串并识别词法单元
/// </summary>
/// <returns></returns>
pair<string, int> lexer::lexicalAnalysis()
{
	string buf;
	while (pos < sourceCode.size())
	{
		//当前字符串位置的第一个字符
		string first_ch;
		first_ch = sourceCode[pos];
		//当前字符串位置的第二个字符，如果越界则取字符为\0
		string second_ch;
		second_ch = pos + 1 < sourceCode.size() ? sourceCode[pos + 1] : '\0';
		//记录可能的由两个字符构成的运算符
		string double_op = first_ch + second_ch;
		//状态机
		switch (S)
		{
		//INIT状态
		case INIT:
			//检查当前字符是否在 skip 集合中，即空格、制表符或回车
			if (find(skip.begin(), skip.end(), first_ch) != skip.end())
			{
				pos++;
				S = INIT;
			}
			//判断是否是换行符
			else if (first_ch == "\n")
			{
				pos++;
				S = INIT;
				codeline++;//行数加1
				//添加换行符对应的token并返回
				lexicalUnit.push_back(strToken.find(first_ch)->second);
				return strToken.find(first_ch)->second;
			}
			//如果当前是/
			else if (first_ch == "/")
			{
				if (second_ch == "/")
				{
					pos += 2;
					S = singleLineComments;
				}
				else if (second_ch == "*")
				{
					pos += 2;
					S = multiLineComments;
				}
				else
				{
					S = Op;
				}
			}
			//判断是数字
			else if (first_ch[0] >= '0' && first_ch[0] <= '9')
			{
				buf.erase();
				buf += first_ch;
				pos++;
				S = Num;
			}
			//判断一元操作符
			else if (find(unaryOP.begin(), unaryOP.end(), first_ch) != unaryOP.end())
			{
				S = Op;
			}
			//判断是字母或者下划线
			else if (first_ch[0] >= 'a' && first_ch[0] <= 'z' || first_ch[0] >= 'A' && first_ch[0] <= 'Z' || first_ch[0] == '_')
			{
				buf.erase();
				buf += first_ch;
				pos++;
				S = Str;
			}
			//如果以上条件都不满足，则抛出异常
			else
			{
				S = MYERROR;
			}
			break;
		//singleLineComments单行注释状态
		case singleLineComments:
			//如果不是换行符，则表示注释尚未结束
			if (first_ch != "\n")
			{
				pos++;
				S = singleLineComments;
			}
			//反之，注释结束,将\n记录
			else
			{
				pos++;
				S = INIT;
				codeline++;
				lexicalUnit.push_back(strToken.find(first_ch)->second);
				return strToken.find(first_ch)->second;
			}
			break;
		//multiLineComments多行注释状态
		case multiLineComments:
			//检查多行注释是否结束
			if (first_ch == "*" && second_ch == "/")
			{
				pos += 2;
				S = INIT;
			}
			else if (first_ch == "\n")
			{
				pos++;
				S = multiLineComments;
				codeline++;
				lexicalUnit.push_back(strToken.find(first_ch)->second);
				return strToken.find(first_ch)->second;
			}
			else
			{
				pos++;
				S = multiLineComments;
			}
			break;
		//Num状态
		case Num:
			if (first_ch[0] >= '0' && first_ch[0] <= '9')
			{
				buf += first_ch;
				pos++;
				S = Num;
			}
			else
			{
				S = INIT;
				//将整个数字加入词法单元
				lexicalUnit.push_back(pair<string, int>("NUM", atoi(buf.c_str())));
				return pair<string, int>("NUM", atoi(buf.c_str()));
			}
			break;
		//Op状态
		case Op:
			if (find(binaryOP.begin(), binaryOP.end(), double_op) != binaryOP.end())
			{
				pos += 2;
				S = INIT;
				lexicalUnit.push_back(strToken.find(double_op)->second);
				return strToken.find(double_op)->second;
			}
			else
			{
				pos++;
				S = INIT;
				lexicalUnit.push_back(strToken.find(first_ch)->second);
				return strToken.find(first_ch)->second;
			}
			break;
		//Str状态
		case Str:
			//检查当前字符是否是字母、下划线或数字,判断字符串
			if (first_ch[0] >= 'a' && first_ch[0] <= 'z' || first_ch[0] >= 'A' && first_ch[0] <= 'Z' || first_ch[0] == '_' || first_ch[0] >= '0' && first_ch[0] <= '9')
			{
				buf += first_ch;
				pos++;
				S = Str;
			}
			//字符串结束
			else
			{
				//判断是否是关键字
				if (find(keyword.begin(), keyword.end(), buf) != keyword.end())
				{
					S = INIT;
					lexicalUnit.push_back(strToken.find(buf)->second);
					return strToken.find(buf)->second;
				}
				//不是关键字，则是标识符
				else
				{
					S = INIT;
					bool exist = false;
					int in_ID = 0;
					//判断标识符是否已经存在于符号表中
					for (map<int, string>::iterator iter = identifierTable.begin(); iter != identifierTable.end(); iter++)
					{
						if (iter->second == buf)
						{
							exist = true;
							in_ID = iter->first;
							break;
						}
					}
					if (!exist)
					{
						//symbol_count表示已经分析到的标识符的个数
						//添加标识符
						identifierTable.insert(pair<int, string>(symbol_count, buf));
						//添加标识符对应的token
						lexicalUnit.push_back(pair<string, int>("ID", symbol_count));
						return pair<string, int>("ID", symbol_count++);
					}
					else
					{
						lexicalUnit.push_back(pair<string, int>("ID", in_ID));
						return pair<string, int>("ID", in_ID);
					}
				}
			}
			break;
		//ERROR状态
		case MYERROR:
			//cerr << "ERROR: An error has occured in lexcal analyser: wrong charactor " << first_ch << "of line" << codeline << endl;
			//cerr << "ERROR: 词法分析器无法分析位于第"<<codeline<<"行的字符：" << first_ch << endl;
			string expmsg = string("ERROR: 词法分析器无法分析位于第") + to_string(codeline) + string("行的字符：") + first_ch;
			throw expmsg;
			lexicalUnit.push_back(pair<string, int>("ERROR", -1));
			return pair<string, int>("ERROR", -1);
			break;
		}
	}
	//标识分析结束
	lexicalUnit.push_back(pair<string, int>("#", -1));
	return pair<string, int>("#", -1);
}
void lexer::getResult(string& result)
{
	do {
		lexicalAnalysis();
	} while (lexicalUnit.rbegin()->first != "#");
	result = "[[";
	for (auto it = identifierTable.begin(); it != identifierTable.end(); ++it) {
		if (it != identifierTable.begin())
			result += ",";
		result += +"[\"" + to_string(it->first) + "\",";
		result += "\"" + it->second + "\"]";

	}
	result += "],[";
	for (auto it = lexicalUnit.begin(); it != lexicalUnit.end(); ++it) {
		if (it != lexicalUnit.begin())
			result += ",";
		result += "[\"" + it->first + "\",";
		result += '\"' + to_string(it->second) + "\"]";

	}
	result += "]]";
	cout << result << endl;
}
void showIdentifierTable(const map<int, string>& identifierTable) {
	cout << "Identifier Table:" << endl;
	cout << "==================" << endl;
	for (const auto& entry : identifierTable) {
		cout << "| " << entry.first << "\t| " << entry.second << " |" << endl;
	}
}

void showLexicalUnit(const vector<pair<string, int>>& lexicalUnit) {
	cout << "Lexical Unit Table:" << endl;
	cout << "===================" << endl;
	for (const auto& entry : lexicalUnit) {
		cout << "| " << entry.first << "\t| " << entry.second << " |" << endl;
	}
}
