#pragma once
#include <map>
#include <sstream>
#include "DeskTop.h"
#include "Pair.h"
class BattleScene;


/*编译执行机*/
class EffectExecutor
{
public:
	EffectExecutor(DeskTop* d);

	mString getCodeFromSegment(mString code, mString segement);

	//构造函数：绑定桌面
	LinkedList<Pair<int, int>>* execute(mString code, mString segment,Role * player, Role * enemy); //执行一整段代码
	bool isEnabled(mString allCode, Role* player, Role* enemy); //执行Enable段，判断XX是否>0
	void insertVirusInto(Effect* ef, Effect* virus); //将叠加效果（VIRUS段）植入到效果ef中
	~EffectExecutor();

private:
	DeskTop* d; //桌面对象
	/*指针语句处理辅助变量*/
	LinkedList<Card*>* PTRC_list[10]; 
	LinkedList<Effect*>* PTRE_list[10];
	Card* PTRC[10]; 
	Effect* PTRE[10];
	Role* player, *enemy ;
	int PTRC_index[10];
	int PTRE_index[10];
	bool LSF, LEF, GTF, GEF, EQF; //比较标志位
	int AX,CX,XX; //通用寄存器
	LinkedList<Pair<int,int>>* executionNotes; //执行日志
private:


	/*处理各语句的函数*/
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

