#pragma once
#include <map>
#include <sstream>
#include "DeskTop.h"
#include "Pair.h"
class BattleScene;


/*����ִ�л�*/
class EffectExecutor
{
public:
	EffectExecutor(DeskTop* d);

	mString getCodeFromSegment(mString code, mString segement);

	//���캯����������
	LinkedList<Pair<int, int>>* execute(mString code, mString segment,Role * player, Role * enemy); //ִ��һ���δ���
	bool isEnabled(mString allCode, Role* player, Role* enemy); //ִ��Enable�Σ��ж�XX�Ƿ�>0
	void insertVirusInto(Effect* ef, Effect* virus); //������Ч����VIRUS�Σ�ֲ�뵽Ч��ef��
	~EffectExecutor();

private:
	DeskTop* d; //�������
	/*ָ����䴦��������*/
	LinkedList<Card*>* PTRC_list[10]; 
	LinkedList<Effect*>* PTRE_list[10];
	Card* PTRC[10]; 
	Effect* PTRE[10];
	Role* player, *enemy ;
	int PTRC_index[10];
	int PTRE_index[10];
	bool LSF, LEF, GTF, GEF, EQF; //�Ƚϱ�־λ
	int AX,CX,XX; //ͨ�üĴ���
	LinkedList<Pair<int,int>>* executionNotes; //ִ����־
private:


	/*��������ĺ���*/
	void exe_XCHG(mString para_str);
	void exe_INC(mString para_str);
	void exe_DEC(mString para_str);
	void exe_MOV(mString para_str);
	void exe_ADD(mString para_str);
	void exe_LEA(mString para_str);
	void exe_MUL(mString para_str);
	void exe_SUB(mString para_str);
	void exe_DIV(mString para_str); 
	void exe_CMP(mString para_str);
	void exe_JMP(map<mString, int>* labelMap, int & ip, mString label);
	void exe_JE(map<mString, int>* labelMap, int & ip, mString label);
	void exe_JNE(map<mString, int>* labelMap, int & ip, mString label);
	void exe_JLE(map<mString, int>* labelMap, int & ip, mString label);
	void exe_JL(map<mString, int>* labelMap, int & ip, mString label);
	void exe_JG(map<mString, int>* labelMap, int & ip, mString label);
	void exe_JGE(map<mString, int>* labelMap, int & ip, mString label);
	void exe_LOOP(map<mString, int>* labelMap, int & ip, mString label);
	void execute_line(mString codeLine, map<mString, int>* labelMap, int & ip);
	
};

