#pragma once

//“瓶子”
typedef struct bottle{
	int num;
	struct bottle* pNext;
}BOTTLE;
typedef BOTTLE* PBOTTLE;

//“投喂样品”
typedef struct feedSample{
	PBOTTLE pMedicine;
	struct feedSample* pNext;
}FEEDSAMPLE;
typedef FEEDSAMPLE* PFEEDSAMPLE;

//“存活情况”
typedef struct survCase{
	int pSurviveStatus;
	struct survCase* pNext;
}SURVCASE;
typedef SURVCASE* PSURVCASE;

//"死老鼠"
typedef struct deadMouse{
	int num;
	struct deadMouse* pNext;
}DEADMOUSE;
typedef DEADMOUSE* PDEADMOUSE;

