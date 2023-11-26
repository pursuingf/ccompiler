#include "lexer.h"

/// <summary>
/// ��������Դ�����ַ���
/// </summary>
/// <param name="fstring"></param>
void lexer::setCodeString(string fstring)
{
	sourceCode = fstring;
	cout<<sourceCode.size()<<endl;
}

/// <summary>
/// ʹ��״̬����ɨ��Դ�����ַ�����ʶ��ʷ���Ԫ
/// </summary>
/// <returns></returns>
pair<string, int> lexer::lexicalAnalysis()
{
	string buf;
	while (pos < sourceCode.size())
	{
		//��ǰ�ַ���λ�õĵ�һ���ַ�
		string first_ch;
		first_ch = sourceCode[pos];
		//��ǰ�ַ���λ�õĵڶ����ַ������Խ����ȡ�ַ�Ϊ\0
		string second_ch;
		second_ch = pos + 1 < sourceCode.size() ? sourceCode[pos + 1] : '\0';
		//��¼���ܵ��������ַ����ɵ������
		string double_op = first_ch + second_ch;
		//״̬��
		switch (S)
		{
		//INIT״̬
		case INIT:
			//��鵱ǰ�ַ��Ƿ��� skip �����У����ո��Ʊ����س�
			if (find(skip.begin(), skip.end(), first_ch) != skip.end())
			{
				pos++;
				S = INIT;
			}
			//�ж��Ƿ��ǻ��з�
			else if (first_ch == "\n")
			{
				pos++;
				S = INIT;
				codeline++;//������1
				//��ӻ��з���Ӧ��token������
				lexicalUnit.push_back(strToken.find(first_ch)->second);
				return strToken.find(first_ch)->second;
			}
			//�����ǰ��/
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
			//�ж�������
			else if (first_ch[0] >= '0' && first_ch[0] <= '9')
			{
				buf.erase();
				buf += first_ch;
				pos++;
				S = Num;
			}
			//�ж�һԪ������
			else if (find(unaryOP.begin(), unaryOP.end(), first_ch) != unaryOP.end())
			{
				S = Op;
			}
			//�ж�����ĸ�����»���
			else if (first_ch[0] >= 'a' && first_ch[0] <= 'z' || first_ch[0] >= 'A' && first_ch[0] <= 'Z' || first_ch[0] == '_')
			{
				buf.erase();
				buf += first_ch;
				pos++;
				S = Str;
			}
			//������������������㣬���׳��쳣
			else
			{
				S = MYERROR;
			}
			break;
		//singleLineComments����ע��״̬
		case singleLineComments:
			//������ǻ��з������ʾע����δ����
			if (first_ch != "\n")
			{
				pos++;
				S = singleLineComments;
			}
			//��֮��ע�ͽ���,��\n��¼
			else
			{
				pos++;
				S = INIT;
				codeline++;
				lexicalUnit.push_back(strToken.find(first_ch)->second);
				return strToken.find(first_ch)->second;
			}
			break;
		//multiLineComments����ע��״̬
		case multiLineComments:
			//������ע���Ƿ����
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
		//Num״̬
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
				//���������ּ���ʷ���Ԫ
				lexicalUnit.push_back(pair<string, int>("NUM", atoi(buf.c_str())));
				return pair<string, int>("NUM", atoi(buf.c_str()));
			}
			break;
		//Op״̬
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
		//Str״̬
		case Str:
			//��鵱ǰ�ַ��Ƿ�����ĸ���»��߻�����,�ж��ַ���
			if (first_ch[0] >= 'a' && first_ch[0] <= 'z' || first_ch[0] >= 'A' && first_ch[0] <= 'Z' || first_ch[0] == '_' || first_ch[0] >= '0' && first_ch[0] <= '9')
			{
				buf += first_ch;
				pos++;
				S = Str;
			}
			//�ַ�������
			else
			{
				//�ж��Ƿ��ǹؼ���
				if (find(keyword.begin(), keyword.end(), buf) != keyword.end())
				{
					S = INIT;
					lexicalUnit.push_back(strToken.find(buf)->second);
					return strToken.find(buf)->second;
				}
				//���ǹؼ��֣����Ǳ�ʶ��
				else
				{
					S = INIT;
					bool exist = false;
					int in_ID = 0;
					//�жϱ�ʶ���Ƿ��Ѿ������ڷ��ű���
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
						//symbol_count��ʾ�Ѿ��������ı�ʶ���ĸ���
						//��ӱ�ʶ��
						identifierTable.insert(pair<int, string>(symbol_count, buf));
						//��ӱ�ʶ����Ӧ��token
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
		//ERROR״̬
		case MYERROR:
			//cerr << "ERROR: An error has occured in lexcal analyser: wrong charactor " << first_ch << "of line" << codeline << endl;
			//cerr << "ERROR: �ʷ��������޷�����λ�ڵ�"<<codeline<<"�е��ַ���" << first_ch << endl;
			string expmsg = string("ERROR: �ʷ��������޷�����λ�ڵ�") + to_string(codeline) + string("�е��ַ���") + first_ch;
			throw expmsg;
			lexicalUnit.push_back(pair<string, int>("ERROR", -1));
			return pair<string, int>("ERROR", -1);
			break;
		}
	}
	//��ʶ��������
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
