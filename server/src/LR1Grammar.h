#pragma once
#pragma once
#ifndef LR1_GRAMMAR
#define LR1_GRAMMAR
#include"global.h"
#include"lexer.h"
#define GRAMMARFILE "grammar.txt"//���ļ�ֻ��

//�淶��
struct canonical {
	pair<string, vector<string>> p;//����ʽ���󲿣�����ʽ���Ҳ�
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

enum actionStatus { ACTION_ERROR, ACTION_STATE, ACTION_REDUCTION, ACTION_ACC };//��ʾ����״̬����Լ�ͽ���
struct actionItem {
public:

	actionStatus status = ACTION_ERROR;
	int nextState = -1;
	pair<string, vector<string>> p;
};


class LR1Grammar {
private:
	string S;//�洢������ַ������ı�
	map<string, vector<string>>FIRST; //�洢�ķ����ŵ�FIRST����
	map<int, closure>collection;//�洢 LR(1) �ķ��е���Ŀ���淶��
	closure genNext(closure& c, string A);//���ɸ�����Ŀ�� c ����һ���հ�
	vector<string> genX(closure& c);//���ݸ�����Ŀ�� c��������һ�����ܵ��ķ�����
	bool canonicalSetSame(vector<canonical>& a, vector<canonical>& b);//���������Ŀ���Ƿ���ͬ
public:
	vector<string>VN;//���ս������
	vector<string>VT;//�ս������
	map<string, vector<vector<string>>>P;//����ʽ����
	map<int, vector<actionItem>>ACTION; //�洢 LR(1) �������е� ACTION ����
	map<int, vector<int>>GOTO;// �洢 LR(1) �������е� GOTO ����
	void readGrammar();//��ȡ�ķ���Ϣ
	void showGrammar();//��ʾ�ı�����
	void genFIRST(); //�����ķ����ŵ� FIRST ����
	void showFIRST();//��ʾ FIRST ��������
	void genCLOSURE(closure& c);//��ʾ FIRST ��������
	void showCLOSURE(closure& c);//��ʾ��Ŀ���հ�����
	void analysisLR1();//ִ�� LR(1) ����
	void showLR1();//��ʾ LR(1) �����Ľ��
	void genLR1Table();//��ʾ LR(1) �����Ľ��
	void showTable();//��ʾ LR(1) ����������
};
#endif // !LR1_GRAMMAR