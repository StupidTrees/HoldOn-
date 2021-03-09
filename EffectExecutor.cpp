#include "pch.h"
#include "BattleScene.h"
#include "EffectExecutor.h"
#include "dimens.h"
int processNumber(mString number) {
	return number.asInt();
}
EffectExecutor::~EffectExecutor()
{

}
void EffectExecutor::exe_XCHG(mString para_str) {
	if (para_str.contains(":")) {
		auto para = para_str.supersplit(",");
		mString det_R_str, sou_R_str, det, sou;
		auto detL = para->get(0).supersplit(":");
		auto souL = para->get(1).supersplit(":");
		det_R_str = detL->get(0);
		det = detL->get(1);
		sou_R_str = souL->get(0);
		sou = souL->get(1);
		if (det.endswith("P") && sou.endswith("P")) { //如果为对牌堆操作
			/* 确定操作的两个牌堆 */
			LinkedList<Card*>* opr_det = nullptr, *opr_sou = nullptr; //最终要操作的牌堆
			Role* detRole, *souRole; //用来存要操作的两个角色

			if (det_R_str == "M")  detRole = player; //确定两个操作角色
			else detRole = enemy;
			if (sou_R_str == "M") souRole = player;
			else souRole = enemy;

			if (det == "HANDP") opr_det = detRole->getHandPile(); //来确定操作的牌堆
			else if (det == "DRCDP") opr_det = detRole->getDrawPile();
			else if (det == "DSCDP")   opr_det = detRole->getDiscardPile();

			if (sou == "HANDP") opr_sou = souRole->getHandPile();
			else if (sou == "DRCDP")   opr_sou = souRole->getDrawPile();
			else if (sou == "DSCDP")  opr_sou = souRole->getDiscardPile();

			if (opr_sou->size() > 0) {
				int n = opr_sou->size();
				if (opr_sou == souRole->getHandPile()) executionNotes->add(Pair<int, int>(souRole == player ? EXE_NOTE_M_HANDP_REMOVE : EXE_NOTE_E_HANDP_REMOVE, n));
				else if (opr_sou == souRole->getDrawPile()) executionNotes->add(Pair<int, int>(souRole == player ? EXE_NOTE_M_DRCDP_REMOVE : EXE_NOTE_E_DRCDP_REMOVE, n));
				else if (opr_sou == souRole->getDiscardPile()) executionNotes->add(Pair<int, int>(souRole == player ? EXE_NOTE_M_DSCDP_REMOVE : EXE_NOTE_E_DSCDP_REMOVE, n));
				if (opr_det == detRole->getHandPile()) executionNotes->add(Pair<int, int>(detRole == player ? EXE_NOTE_M_HANDP_INSERT : EXE_NOTE_E_HANDP_INSERT, n));
				else if (opr_det == detRole->getDrawPile()) executionNotes->add(Pair<int, int>(detRole == player ? EXE_NOTE_M_DRCDP_INSERT : EXE_NOTE_E_DRCDP_INSERT, n));
				else if (opr_det == detRole->getDiscardPile()) executionNotes->add(Pair<int, int>(detRole == player ? EXE_NOTE_M_DSCDP_INSERT : EXE_NOTE_E_DSCDP_INSERT, n));
			}

			if (opr_det->size() > 0) {
				int n = opr_det->size();
				if (opr_det == detRole->getHandPile()) executionNotes->add(Pair<int, int>(detRole == player ? EXE_NOTE_M_HANDP_REMOVE : EXE_NOTE_E_HANDP_REMOVE, n));
				else if (opr_det == detRole->getDrawPile()) executionNotes->add(Pair<int, int>(detRole == player ? EXE_NOTE_M_DRCDP_REMOVE : EXE_NOTE_E_DRCDP_REMOVE, n));
				else if (opr_det == detRole->getDiscardPile()) executionNotes->add(Pair<int, int>(detRole == player ? EXE_NOTE_M_DSCDP_REMOVE : EXE_NOTE_E_DSCDP_REMOVE, n));
				if (opr_sou == souRole->getHandPile()) executionNotes->add(Pair<int, int>(souRole == player ? EXE_NOTE_M_HANDP_INSERT : EXE_NOTE_E_HANDP_INSERT, n));
				else if (opr_sou == souRole->getDrawPile()) executionNotes->add(Pair<int, int>(souRole == player ? EXE_NOTE_M_DRCDP_INSERT : EXE_NOTE_E_DRCDP_INSERT, n));
				else if (opr_sou == souRole->getDiscardPile()) executionNotes->add(Pair<int, int>(souRole == player ? EXE_NOTE_M_DSCDP_INSERT : EXE_NOTE_E_DSCDP_INSERT, n));
			}

			/*最后，完成交换操作*/
			if (opr_det != nullptr&&opr_sou != nullptr) {
				auto temp = new LinkedList<Card*>;
				temp->transportAll(opr_det);
				opr_det->transportAll(opr_sou);
				opr_sou->transportAll(temp);
			}


		}
		else if (det.endswith("Q") && sou.endswith("Q")) {//如果为对效果队列操作
			LinkedList<Effect*>* opr_det = nullptr, *opr_sou = nullptr; //最终要操作的效果
			Role* detRole, *souRole; //用来存要操作的两个角色

			if (det_R_str == "M")  detRole = player; //确定两个操作角色
			else detRole = enemy;
			if (sou_R_str == "M") souRole = player;
			else souRole = enemy;

			opr_det = detRole->getEffectList();
			opr_sou = souRole->getEffectList();


			/*最后，完成交换操作*/
			if (opr_det != nullptr&&opr_sou != nullptr) {
				auto temp = new LinkedList<Effect*>;
				temp->transportAll(opr_det);
				opr_det->transportAll(opr_sou);
				opr_sou->transportAll(temp);
			}
		}
	}
	else if(para_str.contains("PTR")) {
		auto para = para_str.supersplit(",");
		auto p1 = para->get(0);
		auto p2 = para->get(1);
		int point1, point2;
		point1 = p1[4] - '0';
		point2 = p2[4] - '0';
		if ((p1[3] == 'C') && (p2[3] == 'C')) {//对卡牌的操作
			/*卡牌交换*/
			if (PTRC[point2] != nullptr)PTRC_list[point2]->remove(PTRC_index[point2]);
			if (PTRC[point1] != nullptr) PTRC_list[point1]->remove(PTRC_index[point1]);
			if (PTRC[point2] != nullptr)PTRC_list[point1]->add(PTRC_index[point1], PTRC[point2]);
			if (PTRC[point1] != nullptr) PTRC_list[point2]->add(PTRC_index[point2], PTRC[point1]);
			Card* tempC;
			/*卡牌指针的变动*/
			tempC = PTRC[point1];
			PTRC[point1] = PTRC[point2];
			PTRC[point2] = tempC;


			/*拿小本本记下*/
			if (PTRC[point2] != nullptr) {
				if (PTRC_list[point2] == player->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_HANDP_REMOVE,1));
				else if (PTRC_list[point2] == player->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DRCDP_REMOVE,1));
				else if (PTRC_list[point2] == player->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DSCDP_REMOVE,1));
				else if (PTRC_list[point2] == enemy->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_HANDP_REMOVE,1));
				else if (PTRC_list[point2] == enemy->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DRCDP_REMOVE,1));
				else if (PTRC_list[point2] == enemy->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DSCDP_REMOVE,1));

				if (PTRC_list[point1] == player->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_HANDP_INSERT,1));
				else if (PTRC_list[point1] == player->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DRCDP_INSERT,1));
				else if (PTRC_list[point1] == player->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DSCDP_INSERT,1));
				else if (PTRC_list[point1] == enemy->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_HANDP_INSERT,1));
				else if (PTRC_list[point1] == enemy->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DRCDP_INSERT,1));
				else if (PTRC_list[point1] == enemy->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DSCDP_INSERT,1));
			}
			if (PTRC[point1] != nullptr) {
				if (PTRC_list[point1] == player->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_HANDP_REMOVE,1));
				else if (PTRC_list[point1] == player->getDrawPile()) executionNotes->add(Pair<int, int>(EXE_NOTE_M_DRCDP_REMOVE,1));
				else if (PTRC_list[point1] == player->getDiscardPile()) executionNotes->add(Pair<int, int>(EXE_NOTE_M_DSCDP_REMOVE,1));
				else if (PTRC_list[point1] == enemy->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_HANDP_REMOVE,1));
				else if (PTRC_list[point1] == enemy->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DRCDP_REMOVE,1));
				else if (PTRC_list[point1] == enemy->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DSCDP_REMOVE,1));

				if (PTRC_list[point2] == player->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_HANDP_INSERT,1));
				else if (PTRC_list[point2] == player->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DRCDP_INSERT,1));
				else if (PTRC_list[point2] == player->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DSCDP_INSERT,1));
				else if (PTRC_list[point2] == enemy->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_HANDP_INSERT,1));
				else if (PTRC_list[point2] == enemy->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DRCDP_INSERT,1));
				else if (PTRC_list[point2] == enemy->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DSCDP_INSERT,1));
			}
		}
		else if ((p1[3] == 'E') && (p2[3] == 'E')) {//对效果的操作
			Effect* tempE;
			/*效果交换*/
			if (PTRE[point1] != nullptr)PTRE_list[point1]->remove(PTRE_index[point1]);
			if (PTRE[point2] != nullptr)PTRE_list[point1]->add(PTRE_index[point1], PTRE[point2]);
			if (PTRC[point2] != nullptr)PTRE_list[point2]->remove(PTRE_index[point2]);
			if (PTRC[point1] != nullptr)PTRE_list[point2]->add(PTRE_index[point2], PTRE[point1]);
			/*卡牌指针的变动*/
			tempE = PTRE[point1];
			PTRE[point1] = PTRE[point2];
			PTRE[point2] = tempE;

			/*拿小本本记下*/
			if (PTRE[point1] != nullptr) {
				if (PTRE_list[point1] == player->getEffectList()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_EFFECTQ_REMOVE,1));
				else if (PTRE_list[point1] == enemy->getEffectList()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_EFFECTQ_REMOVE,1));

				if (PTRE_list[point2] == player->getEffectList()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_EFFECTQ_INSERT,1));
				else if (PTRE_list[point2] == enemy->getEffectList()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_EFFECTQ_INSERT,1));
			}
			if (PTRE[point2] != nullptr) {
				if (PTRE_list[point2] == player->getEffectList()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_EFFECTQ_REMOVE,1));
				else if (PTRE_list[point2] == enemy->getEffectList()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_EFFECTQ_REMOVE,1));
				if (PTRE_list[point1] == player->getEffectList()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_EFFECTQ_INSERT,1));
				else if (PTRE_list[point1] == enemy->getEffectList()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_EFFECTQ_INSERT,1));
			}
		}
	}
}
void EffectExecutor::exe_INC(mString para_str) {
	int point, position;
	mString aim = para_str;
	if (aim.contains("PTRC") ) {//如果对卡牌操作
		point = aim[4] - '0';//表明为第几号指针寄存器
		position = PTRC_index[point];
		auto tool = PTRC_list[point];
		PTRC[point] = tool->get(position+1);
		PTRC_index[point] = position+1;

	}else if (aim.contains("PTRE")) {//如果对效果操作
		point = aim[4];
		position = PTRE_index[point];
		auto tool = PTRE_list[point];
		PTRE[point] = tool->get((position+1));
		PTRE_index[point] = position+1;
	}else if (aim.contains(":")) {
		auto res3 = aim.supersplit(":");
		Role* faction = res3->get(0) == "E" ? enemy : player;
		/*同时拿小本本记下*/
		if (res3->get(1) == "HP") {
			faction->setHP(faction->getHP() + 1);
			if(faction==enemy)executionNotes->add(Pair<int,int>(EXE_NOTE_E_RECOVER,1));
			else executionNotes->add(Pair<int,int>(EXE_NOTE_M_RECOVER,1));
		}
		else if (res3->get(1) == "MAXHP") faction->setMaxHP(faction->getMaxHP() + 1);
		else if (res3->get(1) == "MANA1") {
			faction->setMANA1(faction->getMANA1() + 1);
			if (faction == enemy)executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA1_UP,1));
			else executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA1_UP,1));
		}
		else if (res3->get(1) == "MANA2") {
			faction->setMANA2(faction->getMANA2() + 1);
			if (faction == enemy)executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA2_UP,1));
			else executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA2_UP,1));
		}
		else if (res3->get(1) == "MAXMANA1") faction->setMaxMANA1(faction->getMaxMANA1() + 1);
		else if (res3->get(1) == "MAXMANA2") faction->setMaxMANA2(faction->getMaxMANA2() + 1);
		else if (res3->get(1) == "LV") faction->setLevel(faction->getLevel() + 1);
	}
	else if (aim == "AX") AX ++;
	else if (aim == "CX") CX ++;
	else if (aim == "XX") CX ++;
}
void EffectExecutor::exe_DEC(mString aim) {
	if (aim.contains(":")) {
		auto res3 = aim.supersplit(":");
		Role* faction = res3->get(0) == "E" ? enemy : player;
		/*同时拿小本本记下*/
		if (res3->get(1) == "HP") {
			faction->setHP(faction->getHP() - 1);
			if (faction == enemy)executionNotes->add(Pair<int,int>(EXE_NOTE_E_HURT,1));
			else executionNotes->add(Pair<int,int>(EXE_NOTE_M_HURT,1));
		}
		else if (res3->get(1) == "MAXHP") faction->setMaxHP(faction->getMaxHP() - 1);
		else if (res3->get(1) == "MANA1") {
			faction->setMANA1(faction->getMANA1() - 1);
			if (faction == enemy)executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA1_DOWN,1));
			else executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA1_DOWN,1));
		}
		else if (res3->get(1) == "MANA2") {
			faction->setMANA2(faction->getMANA2() - 1);
			if (faction == enemy)executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA2_DOWN,1));
			else executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA2_DOWN,1));
		}
		else if (res3->get(1) == "MAXMANA1") faction->setMaxMANA1(faction->getMaxMANA1() - 1);
		else if (res3->get(1) == "MAXMANA2") faction->setMaxMANA2(faction->getMaxMANA2() - 1);
		else if (res3->get(1) == "LV") faction->setLevel(faction->getLevel() - 1);
	}
	else if (aim == "AX") AX--;
	else if (aim == "CX") CX--;
	else if (aim == "XX") {
		XX--;
		if (XX < 0) XX = 0;
	}
}
void EffectExecutor::exe_MOV(mString para_str) {
	auto para = para_str.supersplit(",");
	mString para1 = para->get(0);
	mString para2 = para->get(1);
	if (para1.contains("DP")&&para2.contains("DP")) {//对牌堆操作（会导致PTRC失去意义）
		Role* R1;
		Role* R2;
		if (para1[0] == 'E') R1 = enemy;
		else R1 = player;
		if (para2[0] == 'E') R2 = enemy;
		else R2 = player;
		//确定牌堆
		LinkedList<Card*>* pile1 = nullptr;
		LinkedList<Card*>* pile2 = nullptr;
		if (para1.contains("HANDP")) pile1 = R1->getHandPile();
		else if (para1.contains("DRCDP")) pile1 = R1->getDrawPile();
		else if (para1.contains("DSCDP")) pile1 = R1->getDiscardPile();
		if (para2.contains("HANDP")) pile2 = R2->getHandPile();
		else if (para2.contains("DRCDP")) pile2 = R2->getDrawPile();
		else if (para2.contains("DSCDP")) pile2 = R2->getDiscardPile();


		if (para->size() == 3) {
			//获取数值
			mString para3 = para->get(2);
			int n = 0;
			if (para3.contains("PTR")) {
				char type = para3[3];
				int point = para3[4] - '0';
				if (type == 'C') {
					int fun = para3.supersplit(".")->get(1).asInt();
					switch (fun) {
					case 0:n = PTRC[point]->getId(); break;
					case 1:n = PTRC[point]->getType(); break;
					case 2:n = PTRC[point]->getConsume(); break;
					case 3:n = PTRC[point]->getConsume_type(); break;
					case -1:n = PTRC_list[point]->size(); break;
					case -2:n = PTRC_index[point]; break;
					}
				}
				else if (type == 'E') {
					int fun = para3.supersplit(".")->get(1).asInt();
					switch (fun) {
					case 0:n = PTRE[point]->getId(); break;
					case 1:n = PTRE[point]->getType(); break;
					case 2:n = PTRE[point]->getCountdown(); break;
					case -1:n = PTRE_list[point]->size(); break;
					case -2:n = PTRE_index[point]; break;
					}
				}
			}
			else if (para3.contains(":")) {
				auto temp2 = para3.supersplit(":");
				Role* roleTemp;
				roleTemp = temp2->get(0) == "E" ? enemy : player;
				mString stt = temp2->get(1);
				if (stt == "HP") n = roleTemp->getHP();
				else if (stt == "MAXHP") n = roleTemp->getMaxHP();
				else if (stt == "MANA1") n = roleTemp->getMANA1();
				else if (stt == "MANA2") n = roleTemp->getMANA2();
				else if (stt == "MAXMANA1") n = roleTemp->getMaxMANA1();
				else if (stt == "MAXMANA2") n = roleTemp->getMaxMANA2();
				else if (stt == "LV") n = roleTemp->getLevel();
			}
			else if (para3 == "AX") {
				n = AX;
			}
			else if (para3 == "CX") {
				n = CX;
			}
			else if (para3 == "XX") {
				n = XX;
			}
			else {
				n = para3.asInt();
			}
			auto temp = new LinkedList<Card*>;
			int min = n > pile2->size() ? pile2->size() : n;

			/*拿小本本记下*/
			if (pile2->size() > 0) {
				if (pile2 == R2->getHandPile()) executionNotes->add(Pair<int, int>(R2 == player ? EXE_NOTE_M_HANDP_REMOVE : EXE_NOTE_E_HANDP_REMOVE, min));
				else if (pile2 == R2->getDrawPile()) executionNotes->add(Pair<int, int>(R2 == player ? EXE_NOTE_M_DRCDP_REMOVE : EXE_NOTE_E_DRCDP_REMOVE, min));
				else if (pile2 == R2->getDiscardPile()) executionNotes->add(Pair<int, int>(R2 == player ? EXE_NOTE_M_DSCDP_REMOVE : EXE_NOTE_E_DSCDP_REMOVE, min));
				if (pile1 == R1->getHandPile()) executionNotes->add(Pair<int, int>(R1 == player ? EXE_NOTE_M_HANDP_INSERT : EXE_NOTE_E_HANDP_INSERT, min));
				else if (pile1 == R1->getDrawPile()) executionNotes->add(Pair<int, int>(R1 == player ? EXE_NOTE_M_DRCDP_INSERT : EXE_NOTE_E_DRCDP_INSERT, min));
				else if (pile1 == R1->getDiscardPile()) executionNotes->add(Pair<int, int>(R1 == player ? EXE_NOTE_M_DSCDP_INSERT : EXE_NOTE_E_DSCDP_INSERT, min));
			}
			for (int i = 0; i < min; i++) {
				temp->add(pile2->remove(0));
			}
			pile1->addAll(temp);
		}
		else {
			int n = pile2->size();
			if (pile2->size() > 0) {
				if (pile2 == R2->getHandPile()) executionNotes->add(Pair<int, int>(R2 == player ? EXE_NOTE_M_HANDP_REMOVE : EXE_NOTE_E_HANDP_REMOVE, n));
				else if (pile2 == R2->getDrawPile()) executionNotes->add(Pair<int, int>(R2 == player ? EXE_NOTE_M_DRCDP_REMOVE : EXE_NOTE_E_DRCDP_REMOVE, n));
				else if (pile2 == R2->getDiscardPile()) executionNotes->add(Pair<int, int>(R2 == player ? EXE_NOTE_M_DSCDP_REMOVE : EXE_NOTE_E_DSCDP_REMOVE, n));
				if (pile1 == R1->getHandPile()) executionNotes->add(Pair<int, int>(R1 == player ? EXE_NOTE_M_HANDP_INSERT : EXE_NOTE_E_HANDP_INSERT, n));
				else if (pile1 == R1->getDrawPile()) executionNotes->add(Pair<int, int>(R1 == player ? EXE_NOTE_M_DRCDP_INSERT : EXE_NOTE_E_DRCDP_INSERT, n));
				else if (pile1 == R1->getDiscardPile()) executionNotes->add(Pair<int, int>(R1 == player ? EXE_NOTE_M_DSCDP_INSERT : EXE_NOTE_E_DSCDP_INSERT, n));
			}
			pile1->transportAll(pile2);
			/*拿小本本记下*/

		}


	}
	else if (para1.contains("DP") && para2.contains("PTR")|| para1.contains("EQ") && para2.contains("PTR")) { //移动后，指针指向原位置的元素
		Role* faction = NULL;
		int point;
		char aim;
		auto tar_ope = para->get(0).supersplit(":");
		aim = (tar_ope->get(0))[0];//指定角色
		if (aim == 'E') faction = enemy;
		else if (aim == 'M') faction = player;
		auto address = tar_ope->get(1);//目标
		auto org_ope = para->get(1);//源操作数
		if (org_ope.contains("PTRC") ||org_ope.contains("PTRE")) {
			point = org_ope[4] - '0';
		}
		if (org_ope.contains("PTRC")) {
			if (address == "HANDP") {
				if (PTRC[point] != nullptr) {
					int pos = PTRC_index[point];
					Card* c = PTRC_list[point]->remove(pos);
					faction->getHandPile()->add(c);//移出原卡组放入目标卡组
					if (pos < PTRC_list[point]->size())PTRC[point] = PTRC_list[point]->get(pos);
					else PTRC[point] = PTRC_list[point]->get(PTRC_list[point]->size() - 1);

					/*拿小本本记下*/
					executionNotes->add(Pair<int,int>(faction == player ? EXE_NOTE_M_HANDP_INSERT : EXE_NOTE_E_HANDP_INSERT,1));
					if (PTRC_list[point] == player->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_HANDP_REMOVE,1));
					else if (PTRC_list[point] == player->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DRCDP_REMOVE,1));
					else if (PTRC_list[point] == player->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DSCDP_REMOVE,1));
					else if (PTRC_list[point] == enemy->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_HANDP_REMOVE,1));
					else if (PTRC_list[point] == enemy->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DRCDP_REMOVE,1));
					else if (PTRC_list[point] == enemy->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DSCDP_REMOVE,1));
				}
			}
			else if (address == "DRCDP") {
				if (PTRC[point] != nullptr) {
					int pos = PTRC_index[point];
					faction->getDrawPile()->add(PTRC_list[point]->remove(pos));//移出原卡组放入目标卡组
					//改变相关参数
					if (pos < PTRC_list[point]->size())PTRC[point] = PTRC_list[point]->get(pos);
					else PTRC[point] = PTRC_list[point]->get(PTRC_list[point]->size() - 1);

					/*拿小本本记下*/
					executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_DRCDP_INSERT : EXE_NOTE_E_DRCDP_INSERT,1));
					if (PTRC_list[point] == player->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_HANDP_REMOVE,1));
					else if (PTRC_list[point] == player->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DRCDP_REMOVE,1));
					else if (PTRC_list[point] == player->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DSCDP_REMOVE,1));
					else if (PTRC_list[point] == enemy->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_HANDP_REMOVE,1));
					else if (PTRC_list[point] == enemy->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DRCDP_REMOVE,1));
					else if (PTRC_list[point] == enemy->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DSCDP_REMOVE,1));
				}
			}
			else if (address == "DSCDP") {
				if (PTRC[point] != nullptr) {
					int pos = PTRC_index[point];
					faction->getDiscardPile()->add(PTRC_list[point]->remove(pos));//移出原卡组放入目标卡组

					//改变相关参数
					if (pos < PTRC_list[point]->size())PTRC[point] = PTRC_list[point]->get(pos);
					else PTRC[point] = PTRC_list[point]->get(PTRC_list[point]->size() - 1);

					/*拿小本本记下*/
					executionNotes->add(Pair<int,int>(faction == player ? EXE_NOTE_M_DSCDP_INSERT : EXE_NOTE_E_DSCDP_INSERT,1));
					if (PTRC_list[point] == player->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_HANDP_REMOVE,1));
					else if (PTRC_list[point] == player->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DRCDP_REMOVE,1));
					else if (PTRC_list[point] == player->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_DSCDP_REMOVE,1));
					else if (PTRC_list[point] == enemy->getHandPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_HANDP_REMOVE,1));
					else if (PTRC_list[point] == enemy->getDrawPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DRCDP_REMOVE,1));
					else if (PTRC_list[point] == enemy->getDiscardPile()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_DSCDP_REMOVE,1));
				}
			}
		}
		else if (org_ope.contains("PTRE")) {
			if (address == "EQ") {
				if (PTRE[point] != nullptr) {
					int index = PTRE_index[point];
					faction->getEffectList()->add(PTRE_list[point]->remove(index));
					if(index< PTRE_list[point]->size())PTRE[point] = PTRE_list[point]->get(index);
					else PTRE[point] = PTRE_list[point]->get(PTRE_list[point]->size()-1);

					executionNotes->add(Pair<int,int>(faction == player ? EXE_NOTE_M_EFFECTQ_INSERT : EXE_NOTE_E_EFFECTQ_INSERT,1));
					if (PTRE_list[point] == player->getEffectList()) executionNotes->add(Pair<int,int>(EXE_NOTE_M_EFFECTQ_REMOVE,1));
					else if (PTRE_list[point] == enemy->getEffectList()) executionNotes->add(Pair<int,int>(EXE_NOTE_E_EFFECTQ_REMOVE,1));
				}

			}
		}
	}
	else {
		int sourse = 0;
		if (para2.contains(":")) {
			Role* fromR;
			auto y = para2.supersplit(":");
			mString space = y->get(1);
			fromR = y->get(0) == "E" ? enemy : player;
			if (space == "HP") sourse = fromR->getHP();
			else if (space == "MAXHP") sourse = fromR->getMaxHP();
			else if (space == "MANA1") sourse = fromR->getMANA1();
			else if (space == "MANA2") sourse = fromR->getMANA2();
			else if (space == "MAXMANA1") sourse = fromR->getMaxMANA1();
			else if (space == "MAXMANA2") sourse = fromR->getMaxMANA2();
			else if (space == "LV") sourse = fromR->getLevel();
		}
		else if (para2.contains("PTR")) {
			char type = para2[3];
			int point = para2[4] - '0';
			if (type == 'C') {
				int fun = para2.supersplit(".")->get(1).asInt();
				switch (fun) {
				case 0:sourse = PTRC[point]->getId(); break;
				case 1:sourse = PTRC[point]->getType(); break;
				case 2:sourse = PTRC[point]->getConsume(); break;
				case 3:sourse = PTRC[point]->getConsume_type(); break;
				case -1:sourse = PTRC_list[point]->size(); break;
				case -2:sourse = PTRC_index[point]; break;
				}
			}
			else if (type == 'E') {
				int fun = para2.supersplit(".")->get(1).asInt();
				switch (fun) {
				case 0:sourse = PTRE[point]->getId(); break;
				case 1:sourse = PTRE[point]->getType(); break;
				case 2:sourse = PTRE[point]->getCountdown(); break;
				case -1:sourse = PTRE_list[point]->size(); break;
				case -2:sourse = PTRE_index[point]; break;
				}
			}
		}
		else if (para2 == "AX") {
			sourse = AX;
		}
		else if (para2 == "CX") {
			sourse = CX;
		}
		else if (para2 == "XX") {
			sourse = XX;
		}
		else sourse = para2.asInt();
		if (para1.contains(":")) {
			Role* roleTemp;
			auto x = para1.supersplit(":");
			roleTemp = x->get(0) == "E" ? enemy : player;
			mString stt = x->get(1);
			if (stt == "HP") {
				int cmp = roleTemp->getHP() - sourse;
				roleTemp->setHP(sourse);
				/*拿小本本记下*/
				if (roleTemp == player) {
					if (cmp < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_RECOVER,-cmp));
					else if(cmp>0)executionNotes->add(Pair<int,int>(EXE_NOTE_M_HURT,cmp));
				}else {
					if (cmp < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_RECOVER,-cmp));
					else if (cmp > 0)executionNotes->add(Pair<int,int>(EXE_NOTE_E_HURT,cmp));
				}
			}
			else if (stt == "MAXHP")  roleTemp->setMaxHP(sourse);
			else if (stt == "MANA1") {
				int cmp = roleTemp->getMANA1() - sourse;
				roleTemp->setMANA1(sourse);
				/*拿小本本记下*/
				if (roleTemp == player) {
					if (cmp < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA1_UP,-cmp));
					else if (cmp > 0)executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA1_DOWN,cmp));
				}
				else {
					if (cmp < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA1_UP,-cmp));
					else if (cmp > 0)executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA1_DOWN,cmp));
				}
			}
			else if (stt == "MANA2") {
				int cmp = roleTemp->getMANA2() - sourse;
				roleTemp->setMANA2(sourse);
				/*拿小本本记下*/
				if (roleTemp == player) {
					if (cmp < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA2_UP,-cmp));
					else if (cmp > 0)executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA2_DOWN,-cmp));
				}
				else {
					if (cmp < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA2_UP,-cmp));
					else if (cmp > 0)executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA2_DOWN,cmp));
				}
			}
			else if (stt == "MAXMANA1") roleTemp->setMaxMANA1(sourse);
			else if (stt == "MAXMANA2")roleTemp->setMaxMANA2(sourse);
			else if (stt == "LV") roleTemp->setLevel(sourse);
		}
		else if (para1 == "AX") {
			AX = sourse;
		}
		else if (para1 == "CX") {
			CX = sourse;
		}
		else if (para1 == "XX") {

			XX = sourse;
			if (XX < 0) XX = -sourse;
		}
	}


}
void EffectExecutor::exe_ADD(mString para_str) {
	auto para = para_str.supersplit(",");
	mString para1 = para->get(0);
	mString para2 = para->get(1);
	int cmp2;
	if (para2.contains("PTR")) {
		char type = para2[3];
		int point = para2[4] - '0';
		if (type == 'C') {
			int fun = para2.supersplit(".")->get(1).asInt();
			switch (fun) {
			case 0:cmp2 = PTRC[point]->getId(); break;
			case 1:cmp2 = PTRC[point]->getType(); break;
			case 2:cmp2 = PTRC[point]->getConsume(); break;
			case 3:cmp2 = PTRC[point]->getConsume_type(); break;
			case -1:cmp2 = PTRC_list[point]->size(); break;
			case -2:cmp2 = PTRC_index[point]; break;
			}
		}
		else if (type == 'E') {
			int fun = para2.supersplit(".")->get(1).asInt();
			switch (fun) {
			case 0:cmp2 = PTRE[point]->getId(); break;
			case 1:cmp2 = PTRE[point]->getType(); break;
			case 2:cmp2 = PTRE[point]->getCountdown(); break;
			case -1:cmp2 = PTRE_list[point]->size(); break;
			case -2:cmp2 = PTRE_index[point]; break;
			}
		}
	}
	else if (para2.contains(":")) {
		auto temp2 = para2.supersplit(":");
		Role* roleTemp;
		roleTemp = temp2->get(0) == "E" ? enemy : player;
		mString stt = temp2->get(1);
		if (stt == "HP") cmp2 = roleTemp->getHP();
		else if (stt == "MAXHP") cmp2 = roleTemp->getMaxHP();
		else if (stt == "MANA1") cmp2 = roleTemp->getMANA1();
		else if (stt == "MANA2") cmp2 = roleTemp->getMANA2();
		else if (stt == "MAXMANA1") cmp2 = roleTemp->getMaxMANA1();
		else if (stt == "MAXMANA2") cmp2 = roleTemp->getMaxMANA2();
		else if (stt == "LV") cmp2 = roleTemp->getLevel();
	}
	else if (para2 == "AX") {
		cmp2 = AX;
	}
	else if (para2 == "CX") {
		cmp2 = CX;
	}
	else if (para2 == "XX") {
		cmp2 = XX;
	}
	else {
		cmp2 = para2.asInt();
	}
	if (para1.contains("PTRC"))
	{
		int point = para1[4] - '0';
		PTRC_index[point] += cmp2;
		auto tool = PTRC_list[point];
		PTRC[point] = tool->get(PTRC_index[point]);
	}
	else if (para1.contains("PTRE"))
	{
		int point = para1[4] - '0';
		PTRE_index[point] += cmp2;
		auto tool = PTRE_list[point];
		PTRE[point] = tool->get(PTRE_index[point]);
	}
	else if(para1.contains(":")){
		auto res3 = para->get(0).supersplit(":");
		Role* faction = res3->get(0) == "E" ? enemy : player;
		if (res3->get(1) == "HP") {
			faction->setHP(faction->getHP() + cmp2);
			if (faction == player) {
				if (cmp2 > 0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_RECOVER,cmp2));
				else if (cmp2 < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_HURT,-cmp2));
			}
			else {
				if (cmp2 > 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_RECOVER,cmp2));
				else if (cmp2 < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_HURT,-cmp2));
			}
		}
		else if(res3->get(1) == "MAXHP") faction->setMaxHP(faction->getMaxHP() + cmp2);
		else if (res3->get(1) == "MANA1") {
			faction->setMANA1(faction->getMANA1() + cmp2);
			if (faction == player) {
				if (cmp2 > 0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA1_UP,cmp2));
				else if(cmp2<0)  executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA1_DOWN,-cmp2));
			}
			else {
				if (cmp2 > 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA1_UP,cmp2));
				else if (cmp2 < 0)  executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA1_DOWN,-cmp2));
			}
		}
		else if (res3->get(1) == "MANA2") {
			faction->setMANA2(faction->getMANA2() + cmp2);
			if (faction == player) {
				if (cmp2 > 0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA2_UP,cmp2));
				else if (cmp2 < 0)  executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA2_DOWN,-cmp2));
			}
			else {
				if (cmp2 > 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA2_UP,cmp2));
				else if (cmp2 < 0)  executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA2_DOWN,-cmp2));
			}
		}
		else if (res3->get(1) == "MAXMANA1")	faction->setMaxMANA1(faction->getMaxMANA1() + cmp2);
		else if (res3->get(1) == "MAXMANA2")	faction->setMaxMANA2(faction->getMaxMANA2() + cmp2);
		else if (res3->get(1) == "LV")	faction->setLevel(faction->getLevel() + cmp2);
	}
	else if (para1 == "AX") AX += cmp2;
	else if (para1 == "CX") CX += cmp2;
	else if (para1 == "XX") {
		XX += cmp2;
		if (XX < 0) XX = 0;
	}
}
void EffectExecutor::exe_LEA(mString para_str) {
	int point; char aim;
	Role *faction = NULL;
	auto tool = para_str.supersplit(",");
	//决定使用的指针
	point = (tool->get(0))[4] - '0';

	//决定操作Effect还是Card
	aim = (tool->get(0))[3];
	tool = (tool->get(1)).supersplit(":");
	//方向(己方还是对方)
	if (tool->get(0) == "E")	faction = enemy;
	else if ((tool->get(0) == "M")) faction = player;
	if (aim == 'C')//目标指针为卡牌
	{
		mString pile = tool->get(1);//指令中的指定牌堆
		if (pile.endswith("E")) {//实际操作数为效果区
			//cout << "类型不匹配" << endl;
			return;
		}
		else if (pile.endswith("P")) {
			if (pile == "HANDP") {
				PTRC[point] = faction->getHandPile()->get(0);//获取手牌首地址
				PTRC_index[point] = 0;
				PTRC_list[point] = faction->getHandPile();
			}
			else if (pile == "DRCDP") {
				PTRC[point] = faction->getDrawPile()->get(0);//获取抽牌堆首地址
				PTRC_index[point] = 0;
				PTRC_list[point] = faction->getDrawPile();
			}
			else if (pile == "DSCDP") {
				PTRC[point] = faction->getDiscardPile()->get(0);//获得弃牌堆首地址
				PTRC_index[point] = 0;
				PTRC_list[point] = faction->getDiscardPile();
			}
			else {
				//cout << "???" << endl;
			}
		}
		else {
			//cout << "???" << endl;
			return;
		}
	}
	else if (aim == 'E') {//目标为效果
		mString consequent = tool->get(1);
		if (consequent.endswith("P")) {
		//	cout << "类型不匹配" << endl;
			return;
		}
		else if (consequent.endswith("E")) {
			if (consequent == "EQ") {
				PTRE[point] = faction->getEffectList()->get(0);//效果链表
				PTRE_index[point] = 0;
				PTRE_list[point] = faction->getEffectList();
			}
			else {
				//cout << "???" << endl;
			}
		}
		else {
			//cout << "???" << endl;
		}
	}

}
void EffectExecutor::exe_MUL(mString para_str) {
	auto para = para_str.supersplit(",");
	mString para1 = para->get(0);
	mString para2 = para->get(1);
	if (para1.contains(":")) {
		auto res3 = para1.supersplit(":");
		mString number = para2;
		Role* faction = res3->get(0) == "E" ? enemy : player;
		if (res3->get(1) == "HP") {
			int cmp = faction->getHP() - faction->getHP() * processNumber(number);
			faction->setHP(faction->getHP() * processNumber(number));
			/*小本本记下*/
			if (cmp > 0) executionNotes->add(Pair<int,int>(faction == player ? EXE_NOTE_M_RECOVER : EXE_NOTE_E_RECOVER,cmp));
			else if (cmp < 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_HURT: EXE_NOTE_E_HURT,-cmp));
		}
		else if (res3->get(1) == "MANA1") {
			int cmp = faction->getMANA1() - faction->getMANA1() * processNumber(number);
			faction->setMANA1(faction->getMANA1() * processNumber(number));
			/*小本本记下*/
			if (cmp > 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_MANA1_UP : EXE_NOTE_E_MANA1_UP,cmp));
			else if (cmp < 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_MANA1_DOWN : EXE_NOTE_E_MANA1_DOWN,-cmp));
		}
		else if (res3->get(1) == "MANA2") {
			int cmp = faction->getMANA2() - faction->getMANA2() * processNumber(number);
			faction->setMANA2(faction->getMANA2() * processNumber(number));
			/*小本本记下*/
			if (cmp > 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_MANA2_UP : EXE_NOTE_E_MANA2_UP,cmp));
			else if (cmp < 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_MANA2_DOWN : EXE_NOTE_E_MANA2_DOWN,-cmp));
		}
		else if (res3->get(1) == "MAXMANA1")	faction->setMaxMANA1(faction->getMaxMANA1() * processNumber(number));
		else if (res3->get(1) == "MAXMANA2") faction->setMaxMANA2(faction->getMaxMANA2() * processNumber(number));
		else if (res3->get(1) == "LV") faction->setLevel(faction->getLevel() * processNumber(number));
		
	}
	else if (para1 == "AX") {
		AX *= processNumber(para2);
	}
	else if (para1 == "CX") {
		CX *= processNumber(para2);
	}
	else if (para1 == "XX") {
		XX *= processNumber(para2);
		if (XX < 0) XX = -XX;
	}

}
void EffectExecutor::exe_SUB(mString para_str) {
	auto para = para_str.supersplit(",");
	mString para1 = para->get(0);
	mString para2 = para->get(1);
	int cmp2 = 0;
	if (para2.contains("PTR")) {
		char type = para2[3];
		int point = para2[4] - '0';
		if (type == 'C') {
			int fun = para2.supersplit(".")->get(1).asInt();
			switch (fun) {
			case 0:cmp2 = PTRC[point]->getId(); break;
			case 1:cmp2 = PTRC[point]->getType(); break;
			case 2:cmp2 = PTRC[point]->getConsume(); break;
			case 3:cmp2 = PTRC[point]->getConsume_type(); break;
			case -1:cmp2 = PTRC_list[point]->size(); break;
			case -2:cmp2 = PTRC_index[point]; break;
			}
		}
		else if (type == 'E') {
			int fun = para2.supersplit(".")->get(1).asInt();
			switch (fun) {
			case 0:cmp2 = PTRE[point]->getId(); break;
			case 1:cmp2 = PTRE[point]->getType(); break;
			case 2:cmp2 = PTRE[point]->getCountdown(); break;
			case -1:cmp2 = PTRE_list[point]->size(); break;
			case -2:cmp2 = PTRE_index[point]; break;
			}
		}
	}
	else if (para2.contains(":")) {
		auto temp2 = para2.supersplit(":");
		Role* roleTemp;
		roleTemp = temp2->get(0) == "E" ? enemy : player;
		mString stt = temp2->get(1);
		if (stt == "HP") cmp2 = roleTemp->getHP();
		else if (stt == "MAXHP") cmp2 = roleTemp->getMaxHP();
		else if (stt == "MANA1") cmp2 = roleTemp->getMANA1();
		else if (stt == "MANA2") cmp2 = roleTemp->getMANA2();
		else if (stt == "MAXMANA1") cmp2 = roleTemp->getMaxMANA1();
		else if (stt == "MAXMANA2") cmp2 = roleTemp->getMaxMANA2();
		else if (stt == "LV") cmp2 = roleTemp->getLevel();
	}
	else if (para2 == "AX") {
		cmp2 = AX;
	}
	else if (para2 == "CX") {
		cmp2 = CX;
	}
	else if (para2 == "XX") {
		cmp2 = XX;
	}
	else {
		cmp2 = para2.asInt();
	}

	if (para1.contains("PTR")) {
	//	cout << "you cannot modify the parameter of an card/effect!" << endl;
	}
	else if (para1.contains(":")) {
		auto temp1 = para1.supersplit(":");
		Role* roleTemp;
		roleTemp = temp1->get(0) == "E" ? enemy : player;
		mString stt = temp1->get(1);
		if (stt == "HP") {
			roleTemp->setHP(roleTemp->getHP() - cmp2);
			if (roleTemp == player) {
				if (cmp2 < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_RECOVER,-cmp2));
				else if (cmp2 > 0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_HURT,cmp2));
			}
			else {
				if (cmp2 < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_RECOVER,-cmp2));
				else if (cmp2 > 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_HURT,cmp2));
			}
		}
		else if (stt == "MAXHP") roleTemp->setMaxHP(roleTemp->getMaxHP()-cmp2);
		else if (stt == "MANA1") {
			roleTemp->setMANA1(roleTemp->getMANA1() - cmp2);
			if (roleTemp == player) {
				if (cmp2 <0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA1_UP,-cmp2));
				else if (cmp2 > 0)  executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA1_DOWN,cmp2));
			}
			else {
				if (cmp2 < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA1_UP,-cmp2));
				else if (cmp2 > 0)  executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA1_DOWN,cmp2));
			}
		}
		else if (stt == "MANA2") {
			roleTemp->setMANA2(roleTemp->getMANA2() - cmp2);
			if (roleTemp == player) {
				if (cmp2 < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA2_UP,-cmp2));
				else if (cmp2 > 0)  executionNotes->add(Pair<int,int>(EXE_NOTE_M_MANA2_DOWN,cmp2));
			}
			else {
				if (cmp2 < 0) executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA2_UP,-cmp2));
				else if (cmp2 > 0)  executionNotes->add(Pair<int,int>(EXE_NOTE_E_MANA2_DOWN,cmp2));
			}
		}
		else if (stt == "MAXMANA1") roleTemp->setMaxMANA1(roleTemp->getMaxMANA1() - cmp2);
		else if (stt == "MAXMANA2")  roleTemp->setMaxMANA2(roleTemp->getMaxMANA2() - cmp2);
		else if (stt == "LV")  roleTemp->setLevel(roleTemp->getLevel() - cmp2);
	}
	else if (para1 == "AX") {
		AX -= cmp2;
	}
	else if (para1 == "CX") {
		CX -= cmp2;
	}
	else if (para1 == "XX") {
		XX -= cmp2;
		if (XX < 0) XX = 0;
	}
}
void EffectExecutor::exe_DIV(mString para_str) {
	auto para = para_str.supersplit(",");
	mString para1 = para->get(0);
	mString para2 = para->get(1);
	if (para1.contains(":")) {
		auto res3 = para1.supersplit(":");
		mString number = para2;
		Role* faction = res3->get(0) == "E" ? enemy : player;
		if (res3->get(1) == "HP") {
			int cmp = faction->getHP() - faction->getHP()/ processNumber(number);
			faction->setHP(faction->getHP()/ processNumber(number));
			/*小本本记下*/
			if (cmp > 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_RECOVER : EXE_NOTE_E_RECOVER,cmp));
			else if (cmp < 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_HURT : EXE_NOTE_E_HURT,-cmp));
		}
		else if (res3->get(1) == "MANA1") {
			int cmp = faction->getMANA1() - faction->getMANA1() / processNumber(number);
			faction->setMANA1(faction->getMANA1() / processNumber(number));
			/*小本本记下*/
			if (cmp > 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_MANA1_UP : EXE_NOTE_E_MANA1_UP,cmp));
			else if (cmp < 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_MANA1_DOWN : EXE_NOTE_E_MANA1_DOWN,-cmp));
		}
		else if (res3->get(1) == "MANA2") {
			int cmp = faction->getMANA2() - faction->getMANA2() / processNumber(number);
			faction->setMANA2(faction->getMANA2()/ processNumber(number));
			/*小本本记下*/
			if (cmp > 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_MANA2_UP : EXE_NOTE_E_MANA2_UP,cmp));
			else if (cmp < 0) executionNotes->add(Pair<int, int>(faction == player ? EXE_NOTE_M_MANA2_DOWN : EXE_NOTE_E_MANA2_DOWN,-cmp));
		}
		else if (res3->get(1) == "MAXMANA1")	faction->setMaxMANA1(faction->getMaxMANA1()/ processNumber(number));
		else if (res3->get(1) == "MAXMANA2") faction->setMaxMANA2(faction->getMaxMANA2() / processNumber(number));
		else if (res3->get(1) == "LV") faction->setLevel(faction->getLevel() / processNumber(number));

	}
	else if (para1 == "AX") {
		AX /= processNumber(para2);
	}
	else if (para1 == "CX") {
		CX /= processNumber(para2);
	}
	else if (para1 == "XX") {
		XX /= processNumber(para2);
		if (XX < 0) XX = -XX;
	}

}
void EffectExecutor::exe_CMP(mString para_str)
{
	auto para = para_str.supersplit(",");
	mString para1 = para->get(0);
	mString para2 = para->get(1);
	int cmp1 = -1 , cmp2 = -1;

	if (para1.contains("PTR")) {
		char type = para1[3];
		int point = para1[4] - '0';
		if (type == 'C') {
			int fun = para1.supersplit(".")->get(1).asInt();
			
			switch (fun) {
			case 0:cmp1 = PTRC[point]->getId(); break;
			case 1:cmp1 = PTRC[point]->getType(); break;
			case 2:cmp1 = PTRC[point]->getConsume(); break;
			case 3:cmp1 = PTRC[point]->getConsume_type(); break;
			case -1:cmp1 = PTRC_list[point]->size(); break;
			case -2:cmp1 = PTRC_index[point];break;
			}
		}
		else if (type == 'E') {
			int fun = para1.supersplit(".")->get(1).asInt();
			switch (fun) {
			case 0:cmp1 = PTRE[point]->getId(); break;
			case 1:cmp1 = PTRE[point]->getType(); break;
			case 2:cmp1 = PTRE[point]->getCountdown(); break;
			case -1:cmp1 = PTRE_list[point]->size(); break;
			case -2:cmp1 = PTRE_index[point]; break;
			}
		}
	}
	else if (para1.contains(":")){
		auto temp1 = para1.supersplit(":");
		Role* roleTemp;
		roleTemp = temp1->get(0) == "E" ? enemy : player;
		mString stt = temp1->get(1);
		if (stt == "HP") cmp1 = roleTemp->getHP();
		else if (stt == "MAXHP") cmp1 = roleTemp->getMaxHP();
		else if (stt == "MANA1") cmp1 = roleTemp->getMANA1();
		else if (stt == "MANA2") cmp1 = roleTemp->getMANA2();
		else if (stt == "MAXMANA1") cmp1 = roleTemp->getMaxMANA1();
		else if (stt == "MAXMANA2") cmp1 = roleTemp->getMaxMANA2();
		else if (stt == "LV") cmp1 = roleTemp->getLevel();
	}
	else if (para1 == "AX") {
		cmp1 = AX;
	}
	else if (para1 == "CX") {
		cmp1 = CX;
	}
	else if (para1 == "XX") {
		cmp1 = XX;
	}
	else{
		cmp1 = para1.asInt();
	}

	if (para2.contains("PTR")) {
		char type = para2[3];
		int point = para2[4] - '0';
		if (type == 'C') {
			int fun = para2.supersplit(".")->get(1).asInt();
			switch (fun) {
			case 0:cmp2 = PTRC[point]->getId(); break;
			case 1:cmp2 = PTRC[point]->getType(); break;
			case 2:cmp2 = PTRC[point]->getConsume(); break;
			case 3:cmp2 = PTRC[point]->getConsume_type(); break;
			case -1:cmp2 = PTRC_list[point]->size(); break;
			case -2:cmp2 = PTRC_index[point]; break;
			}
		}
		else if (type == 'E') {
			int fun = para2.supersplit(".")->get(1).asInt();
			switch (fun) {
			case 0:cmp2 = PTRE[point]->getId(); break;
			case 1:cmp2 = PTRE[point]->getType(); break;
			case 2:cmp2 = PTRE[point]->getCountdown(); break;
			case -1:cmp2 = PTRE_list[point]->size(); break;
			case -2:cmp2 = PTRE_index[point]; break;
			}
		}
	}
	else if (para2.contains(":")) {
		auto temp2 = para2.supersplit(":");
		Role* roleTemp;
		roleTemp = temp2->get(0) == "E" ? enemy : player;
		mString stt = temp2->get(1);
		if (stt == "HP") cmp2 = roleTemp->getHP();
		else if (stt == "MAXHP") cmp2 = roleTemp->getMaxHP();
		else if (stt == "MANA1") cmp2 = roleTemp->getMANA1();
		else if (stt == "MANA2") cmp2 = roleTemp->getMANA2();
		else if (stt == "MAXMANA1") cmp2 = roleTemp->getMaxMANA1();
		else if (stt == "MAXMANA2") cmp2 = roleTemp->getMaxMANA2();
		else if (stt == "LV") cmp2 = roleTemp->getLevel();
	}
	else if (para2 == "AX") {
		cmp2 = AX;
	}else if (para2 == "CX") {
		cmp2 = CX;
	}
	else if (para2 == "XX") {
		cmp2 = XX;
	}
	else{
		cmp2 = para2.asInt();
	}
	//cocos2d::log( "compare:%d,%d", cmp1 , cmp2 );
	LSF = cmp1 < cmp2;
	LEF = cmp1 <= cmp2;
	GTF = cmp1 > cmp2;
	GEF = cmp1 >= cmp2;
	EQF = cmp1 == cmp2;
	//cout << "FLAGS:" << "LSF:" << LSF << ",LEF:" << LEF << ",GTF:" << GTF << ",GEF:" << GEF << ",EQF:" << EQF << endl;
}
void EffectExecutor::exe_JMP(map<mString, int>* labelMap, int &ip, mString label) {
	ip = labelMap->at(label);
}
void EffectExecutor::exe_JE(map<mString, int>* labelMap, int &ip, mString label) {
	if(EQF)ip = labelMap->at(label);
}
void EffectExecutor::exe_JNE(map<mString, int>* labelMap, int &ip, mString label) {
	if (!EQF)ip = labelMap->at(label);
}
void EffectExecutor::exe_JLE(map<mString, int>* labelMap, int &ip, mString label) {
	if (LEF)ip = labelMap->at(label);
}
void EffectExecutor::exe_JL(map<mString, int>* labelMap, int &ip, mString label) {
	if (LSF)ip = labelMap->at(label);
}
void EffectExecutor::exe_JG(map<mString, int>* labelMap, int &ip, mString label) {
	if (GTF)ip = labelMap->at(label);
}
void EffectExecutor::exe_JGE(map<mString, int>* labelMap, int &ip, mString label) {
	if (GEF)ip = labelMap->at(label);
}
void EffectExecutor::exe_LOOP(map<mString, int>* labelMap, int &ip, mString label) {
	CX--;
	if (CX>0)ip = labelMap->at(label);
}
void EffectExecutor::execute_line(mString codeLine,map<mString,int>* labelMap,int &ip) {
	auto first = codeLine.supersplit(" ");
	mString opr = first->get(0);
	mString para = first->get(1);
	if (opr == "XCHG") exe_XCHG(para);
	else if (opr == "INC") exe_INC(para);
	else if (opr == "MOV") exe_MOV(para);
	else if (opr == "ADD") exe_ADD(para);
	else if (opr == "LEA") exe_LEA(para);
	else if (opr == "DEC") exe_DEC(para);
	else if (opr == "MUL") exe_MUL(para);
	else if (opr == "SUB") exe_SUB(para);
	else if (opr == "DIV") exe_DIV(para);
	else if (opr == "CMP") exe_CMP(para);
	else if (opr == "JMP") exe_JMP(labelMap,ip,para);
	else if (opr == "JE") exe_JE(labelMap, ip, para);
	else if (opr == "JNE") exe_JNE(labelMap, ip, para);
	else if (opr == "JLE") exe_JLE(labelMap, ip, para);
	else if (opr == "JL") exe_JLE(labelMap, ip, para);
	else if (opr == "JG") exe_JG(labelMap, ip, para);
	else if (opr == "JGE") exe_JGE(labelMap, ip, para);
	else if (opr == "LOOP") exe_LOOP(labelMap, ip, para);
}
EffectExecutor::EffectExecutor(DeskTop* d)
{
	this->d = d;
	executionNotes = new LinkedList<Pair<int,int>>;
}

mString EffectExecutor::getCodeFromSegment(mString code,mString segment) {
	auto segments = code.supersplit(" ENDS");
	for (int i = 0; i < segments->size(); i++) {
		mString segementRaw = segments->get(i);
		auto segmentSl = segementRaw.supersplit("SEGMENT");
		if (segmentSl->get(0).contains(segment)) {
			mString segCode = segmentSl->get(1);
			segCode.replaceAll(segment, "");
			segCode.replaceAll("SEGMENT", "");
			return segCode;
		}
	}
	return "";
}
//void EffectExecutor::execute(mString allCode,mString segment,Role* player,Role* enemy) {
//	//isEnabled(allCode);
//	AX = CX = XX = 0;
//	mString code = getCodeFromSegment(allCode, "EXECUTE");
//	map<mString,int>* labelMap = new map<mString,int>;
//	LinkedList<mString> pureLines;
//	this->player = player;
//	this->enemy = enemy; 
//	code.replaceAll("\r\n", "\n");
//	code.replaceAll("\r", "\n");
//	code.replaceAll("\n\n", "\n");
//	auto codelines = code.supersplit_strict("\n");
//
//
//
//	int i = 0;
//	LinkedList<mString>::Iterator iat(*codelines);
//	while (iat.hasNext()) {
//		mString line = iat.next();
//		if (line.contains("XX<")) {
//			int from = line.indexOf("XX<");
//			auto tag = line.subString(line.indexOf("<", from), line.indexOf(">", from));
//			string s = tag;
//			//log("tag:%s", s.c_str());
//			line.replaceAll(tag, "");
//		}
//		if (line.contains("#")) {
//			auto x = line.supersplit("#");
//			labelMap->insert(pair<mString, int>(x->get(0), i));
//			pureLines.add(x->get(1));
//		}
//		
//		else pureLines.add(line);
//		i++;
//	}
//
//	i = 0;
//	while (i < pureLines.size()) {
//		int beforeI = i;
//		execute_line(pureLines.get(i), labelMap, i);
//		string s = pureLines.get(beforeI);
//		cocos2d::log("execute:%s  ip=%d->%d", s.c_str(), beforeI, i);
//
//		if (i == beforeI) i++;
//	}
//
//}


LinkedList<Pair<int,int>>* EffectExecutor::execute(mString allCode, mString segment, Role* player, Role* enemy) {
	executionNotes->clear();
	AX = CX = XX = 0;
	mString code = getCodeFromSegment(allCode,segment);
	map<mString, int>* labelMap = new map<mString, int>;
	LinkedList<mString> pureLines;
	this->player = player;
	this->enemy = enemy;
	code.replaceAll("\r\n", "\n");
	code.replaceAll("\r", "\n");
	code.replaceAll("\n\n", "\n");
	auto codelines = code.supersplit("\n");
	int i = 0;
	LinkedList<mString>::Iterator iat(codelines);
	while (iat.hasNext()) {
		mString line = iat.next();
		if (line.contains("XX<")) {
			int from = line.indexOf("XX<");
			auto tag = line.subString(line.indexOf("<", from), line.indexOf(">", from));
			string s = tag;
			//log("tag:%s", s.c_str());
			line.replaceAll(tag, "");
		}
		if (line.contains("#")) {
			auto x = line.supersplit("#");
			labelMap->insert(pair<mString, int>(x->get(0), i));
			pureLines.add(x->get(1));
		}

		else pureLines.add(line);
		i++;
	}

	i = 0;
	while (i < pureLines.size()) {
		int beforeI = i;
		execute_line(pureLines.get(i), labelMap, i);
		string s = pureLines.get(beforeI);
		cocos2d::log("execute:%s  ip=%d->%d", s.c_str(), beforeI, i);

		if (i == beforeI) i++;
	}
	LinkedList<Pair<int,int>>* exeResult = new LinkedList<Pair<int,int>>;
	exeResult->transportAll(executionNotes);
	return exeResult;

}
bool EffectExecutor::isEnabled(mString allCode,Role* player,Role* enemy) {
	this->player = player;
	this->enemy = enemy;
	//cocos2d::log("IS_ENABLE:BEGIN");
	AX = CX = XX = 0;
	mString code = getCodeFromSegment(allCode, "ENABLE");
	map<mString, int>* labelMap = new map<mString, int>;
	LinkedList<mString> pureLines;
	this->player = player;
	this->enemy = enemy;
	code.replaceAll("\r\n", "\n");
	code.replaceAll("\r", "\n");
	code.replaceAll("\n\n", "\n");
	auto codelines = code.supersplit("\n");



	int i = 0;
	LinkedList<mString>::Iterator iat(codelines);
	while (iat.hasNext()) {
		mString line = iat.next();
		if (line.contains("#")) {
			auto x = line.supersplit("#");
			labelMap->insert(pair<mString, int>(x->get(0), i));
			pureLines.add(x->get(1));
		}
		else pureLines.add(line);
		i++;
	}

	i = 0;
	while (i < pureLines.size()) {
		int beforeI = i;
		execute_line(pureLines.get(i), labelMap, i);
		string s = pureLines.get(beforeI);
		//cocos2d::log("execute:%s  ip=%d->%d", s.c_str(), beforeI, i);
		if (i == beforeI) i++;
	}
	//cocos2d::log("IS_ENABLE:%d",XX);
	return XX!=0;
}

void EffectExecutor::insertVirusInto(Effect* ef, Effect* virus) {
	string name = virus->getName();
	log("insert_virus:%s", name.c_str());
	auto virusWholeCode = virus->getCode();
	int from = virusWholeCode.indexOf("VIRUS<");
	mString virusTarget = virusWholeCode.subString(virusWholeCode.indexOf("<", from), virusWholeCode.indexOf(">", from));
	mString virusCode = getCodeFromSegment(virus->getCode().replaceAll(virusTarget,""), "VIRUS");
	mString code = ef->getCode();
	code.replaceAll("\r\n", "\n");
	code.replaceAll("\r", "\n");
	code.replaceAll("\n\n", "\n");
	virusCode.replaceAll("\r\n", "\n");
	virusCode.replaceAll("\r", "\n");
	virusCode.replaceAll("\n\n", "\n");
	auto codelines = code.supersplit("\n");
	auto viruslines = virusCode.supersplit("\n");
	auto insertPoints = new LinkedList<int>;
	bool isEnableNow = false;
	for (int i = 0; i < codelines->size(); i++) {
		auto x = codelines->get(i);
		if (x.contains("ENABLE SEGMENT")) isEnableNow = true;
		else if (x.contains("ENABLE ENDS")) isEnableNow = false;
		else if ((!isEnableNow)&&x.contains("MOV XX<")) {
			int from = x.indexOf("MOV XX");
			mString tag = x.subString(x.indexOf("<", from), x.indexOf(">", from));
			if(virusTarget==tag||virusTarget=="<all>")insertPoints->add( i + 1);
		}
		else if ((!isEnableNow) && x.contains("MOV XX")&& virusTarget == "<all>") {
			insertPoints->add(i + 1);
		}
	}
	/*从大的下标开始插入，防止错误*/
	for(int i=insertPoints->size()-1;i>=0;i--){
		codelines->addAll(insertPoints->get(i), viruslines);
	}
	mString result = "";
	for (int i = 0; i < codelines->size(); i++) {
		mString slash = i == codelines->size() - 1 ? "" : "\n";
		result = result + codelines->get(i)+slash;
	}
	ef->updateCode(result);
	
}