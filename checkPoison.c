#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checkPoison.h"

PBOTTLE getTail_bottle(PBOTTLE pHead){
	if(NULL != pHead->pNext)
		return getTail_bottle(pHead->pNext);
	else
		return pHead;
}
PSURVCASE getTail_status(PSURVCASE pHead){
	if(NULL != pHead->pNext)
		return getTail_status(pHead->pNext);
	else
		return pHead;
}
PFEEDSAMPLE getTail_feed(PFEEDSAMPLE pHead){
	if(NULL != pHead->pNext)
		return getTail_feed(pHead->pNext);
	else
		return pHead;
}
PDEADMOUSE getTail_mouse(PDEADMOUSE pHead){
	if(NULL != pHead->pNext)
		return getTail_mouse(pHead->pNext);
	else
		return pHead;
}

PBOTTLE gpBottle = NULL;			//瓶子链表 	全局指针，指向链表头
PFEEDSAMPLE gpScheme = NULL;		//混药方案 	全局指针，指向链表头
PSURVCASE gpStatus = NULL;			//生存状态 	全局指针，指向链表头
PDEADMOUSE gpDeadMice = NULL;		//死老鼠	全局指针，指向链表头

void freeBottle(){
	PBOTTLE iterBottle = gpBottle;
	PBOTTLE tmp = iterBottle;
	while(1){
		if(NULL == iterBottle){
			break;
		}
		else{
			tmp = iterBottle;
			iterBottle = iterBottle->pNext;
			free(tmp);
		}
	}
}
void freeFeedScheme(){
	PFEEDSAMPLE iterFeedSample = gpScheme;
	PFEEDSAMPLE tmp = iterFeedSample;
	while(1){
		if(NULL == iterFeedSample){
			break;
		}
		else{
			tmp = iterFeedSample;
			iterFeedSample = iterFeedSample->pNext;
			free(tmp);
		}
	}
}
void freeSurvStatus(){
	PSURVCASE iterSurv = gpStatus;
	PSURVCASE tmp = iterSurv;
	while(1){
		if(NULL == iterSurv){
			break;
		}
		else{
			tmp = iterSurv;
			iterSurv = iterSurv->pNext;
			free(tmp);
		}
	}
}
void freeDeadMice(){
	PDEADMOUSE iterDeadMouse = gpDeadMice;
	PDEADMOUSE tmp = iterDeadMouse;
	while(1){
		if(NULL == iterDeadMouse){
			break;
		}
		else{
			tmp = iterDeadMouse;
			iterDeadMouse = iterDeadMouse->pNext;
			free(tmp);
		}
	}
}


//形参：小鼠数量、瓶子数量；
//生成检药方案（链表）；
//正常返回0，异常返回-1；
int feedSample(int num_mouse, int num_bottle){
	int bottleMax = 1 << num_mouse;
	int i;

	if(bottleMax < num_bottle)
		return -1;

	//生存状态
	PSURVCASE pSurvCase;
	for(i=0; i<bottleMax; i++){
		pSurvCase = (PSURVCASE)calloc(1, sizeof(SURVCASE));
		pSurvCase->pSurviveStatus = i;
		if(0 == i)
			gpStatus = pSurvCase;
		else
			getTail_status(gpStatus)->pNext = pSurvCase;	//后插链表
	}

	//瓶子描述
	PBOTTLE pBottle = NULL;
	for(i=0; i<bottleMax;i++){
		pBottle = (PBOTTLE)calloc(1, sizeof(BOTTLE));
		pBottle->num = i;
		if(NULL == gpBottle)
			gpBottle = pBottle;
		else
			getTail_bottle(gpBottle)->pNext = pBottle;		//后插链表

		//实际检药需求没有达到上限
		if(i == num_bottle)
			break;
	}

	//“投喂样品”，有多少只小白鼠就有多少个“投喂样品”；
	PFEEDSAMPLE pFeedSample = NULL;
	for(i=0; i<num_mouse; i++){
		pFeedSample = (PFEEDSAMPLE)calloc(1, sizeof(PFEEDSAMPLE));
		//先开空间，具体有哪些瓶子，等会填；
		if(0 == i)
			gpScheme = pFeedSample;
		else
			getTail_feed(gpScheme)->pNext = pFeedSample;	//后插链表
	}

	//投喂样品 <== 映射 ==> 瓶子编号
	int comp = 0;
	PSURVCASE iter_status = gpStatus;
	PFEEDSAMPLE iter_scheme = gpScheme;
	for(i=num_mouse-1; i>=0; i--){
		iter_status = gpStatus;
		while(iter_status){
			comp = 1 << i;
			if(iter_status->pSurviveStatus & comp){
				//把这个编号加入投喂样品
				PBOTTLE pBottle = (PBOTTLE)calloc(1, sizeof(BOTTLE));
				pBottle->num = iter_status->pSurviveStatus;
				if(NULL == iter_scheme->pMedicine)
					iter_scheme->pMedicine = pBottle;
				else
					getTail_bottle(iter_scheme->pMedicine)->pNext = pBottle;
			}
			iter_status = iter_status->pNext;
		}
		iter_scheme = iter_scheme->pNext;
	}

	//调试代码，输出混药方案
	iter_scheme = gpScheme;
	PBOTTLE iter_bottle = NULL;
	for(i=0; i<num_mouse; i++){
		printf("喂给第%d只小白鼠的第%d份投喂样品，由下列编号的药瓶混合而成：\n\t", i, i);
		iter_bottle = iter_scheme->pMedicine;
		while(NULL != iter_bottle){
			printf("%d", iter_bottle->num);
			iter_bottle = iter_bottle->pNext;
			if(NULL != iter_bottle)
				printf(", ");
		}
		printf("\n");
		iter_scheme = iter_scheme->pNext;
	}

	return 0;
}

//返回有毒的瓶子编号
int checkPoison(int num_mouse, int num_bottle){
	int target_survStatus = 0;

	int i;
	PDEADMOUSE iterMouse = NULL;
	for(i=num_mouse-1; i>=0; i--){
		iterMouse = gpDeadMice;
		while(NULL != iterMouse){
			if(i == num_mouse - iterMouse->num -1){
				target_survStatus = target_survStatus | (1 << i);
			}
			iterMouse = iterMouse->pNext;
		}
	}

	printf("poison bottle num is %d\n", target_survStatus);

	return target_survStatus;
}

int main(){
	int num_mouse = 0;
	int num_bottle = 0;
	char ch = 0;
	char* pStr = calloc(1, 128);
	char* pRealloc = NULL;
	int len_input = 0;
	int len_buf = 128;
	printf("pls input num of mouse:\n");
	scanf("%d", &num_mouse);
	printf("ok, %d mice, can check %d bottles at most.\npls input the num of bottles you are going to check:\n", num_mouse, 1 << num_mouse);
	scanf("%d", &num_bottle);
	printf("ok, %d bottles to check.\n", num_bottle);
	feedSample(num_mouse, num_bottle);

	printf("Assume that experiment is done, pls input the serial_nums(count from 0) of dead mice, seperated by comma without any space:\n");
	printf("If no mouse dead, pls press enter.\n");
	getchar();//接收缓存残留
	while(1){
		ch = getchar();
		if(ch == EOF || ch == 0x0d || ch == 0x0a)
			break;
		//缓存用完，加128bytes重开
		if(len_input == len_buf-1){
			len_buf += 128;
			pRealloc = realloc(pStr, len_buf*sizeof(char));
			pStr = pRealloc;
		}
		pStr[len_input++] = ch;
	}
	pStr[len_input] = '\0';

	PDEADMOUSE pDeadMouse = NULL;
	char str_tmp[128] = {0};
	char* pComma = NULL;
	char* iterStr = pStr;
	while(1){
		if(0 == strlen(iterStr)){
			printf("No mouse dead.\n");
			break;
		}

		pDeadMouse = (PDEADMOUSE)calloc(1, sizeof(pDeadMouse));

		pComma = strstr(iterStr, ",");
		if(NULL == pComma){
			pDeadMouse->num = atoi(iterStr);
		}
		else{
			strncpy(str_tmp, iterStr, strlen(iterStr) - strlen(pComma));
			pDeadMouse->num = atoi(str_tmp);
		}

		if(NULL == gpDeadMice){
			gpDeadMice = pDeadMouse;
		}
		else{
			getTail_mouse(gpDeadMice)->pNext = pDeadMouse;
		}

		if(NULL == pComma)
			break;

		iterStr = pComma + 1;
	}

	checkPoison(num_mouse, num_bottle);

	freeBottle();
	freeFeedScheme();
	freeSurvStatus();
	freeDeadMice();

	return 0;
}
