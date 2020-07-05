// BlockAllocate.cpp : DLL을 위해 내보낸 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "BlockAllocate.h"
#include <windows.h>

bool autoAssign();



//유니온 보드판
const int xSize = 27;
const int ySize = 21;
int board[xSize * ySize] = { 0, };//★★★입력받아야함★★★
/* 유니온 x 24칸 + 판정용도 3칸
유니온 y 20 칸 + 판정용도 1칸
	x는 0부터 23까지
	y는 0부터 19까지 존재하며 *26하여 표현됨
	ex board[22+18*26] 는 x좌표 22 y좌표 18*/
int crtLocation; //현재 블럭을 놓을것을 고려중인 칸


//방향을 고려하지 않았을때의 블럭 유형 정보, 총14가지
	/*총14종류 블럭, 방향까지 고려하면 51가지.
형태고유넘버: R레드전사 P퍼플도적 B블루마법사 W화이트해적 G그린궁수*/
const int RSSS = 0, PSSS = 1, BSSS = 2, WSSS = 3, GSSS = 4;
const int RSS = 5, PSS = 6, BSS = 7, WSS = 8, GSS = 9;
const int RS = 10, PS = 11; // 전사와 해적이 형태가 같음. 도적,마법사,궁수가 형태가 같음.
const int RA = 12; //모든 직업이 형태가 같음.
const int RB = 13; //모든 직업이 형태가 같음.
const int blockKind = RB + 1;
int extraBlockList[blockKind] = { 0, }; //블록 유형별 남은 개수 리스트 //★★★입력받아야함★★★



//방향을 고려했을때의 블럭 정보, 총51가지
const int blockListSize = 51;
typedef struct unionBlock {
	int uniqueNumber; //고유넘버,방향이포함됨
	int shapeType; //형태고유넘버
	int shapeSize;
	int Detail1; //반드시 0임
	int Detail2;
	int Detail3;
	int Detail4;
	int Detail5;
}unionBlock;
unionBlock blockList[blockListSize] =
{
	//전사SSS블록
	{ 0,0, 5 ,0,xSize,xSize + 1,xSize * 2,xSize * 2 + 1 },
	{ 1,0, 5 ,0,1,2,xSize,xSize + 1},
	{ 2,0, 5 ,0,1,xSize,xSize + 1,xSize * 2 + 1},
	{ 3,0, 5 ,0,1,xSize - 1,xSize,xSize + 1},
	{ 4,0, 5 ,0,1,xSize,xSize + 1,xSize * 2},
	{ 5,0, 5,0,1,2,xSize + 1,xSize + 2 },
	{ 6,0, 5 ,0,xSize - 1,xSize,xSize * 2 - 1,xSize * 2},
	{ 7,0, 5,0,1,xSize,xSize + 1,xSize + 2},

	//도적SSS블록
	{8,1,5,0, xSize,xSize + 1,xSize + 2,xSize * 1},
	{9,1,5,0,1,2,xSize + 1,xSize * 2 + 1 },
	{10,1,5,0,xSize,xSize * 2 - 1,xSize * 2,xSize * 2 + 1 },
	{11,1,5,0,xSize - 2,xSize - 1,xSize,xSize * 2 },

	//법사SSS블록
	{12,2,5,0,xSize - 1,xSize,xSize + 1,xSize * 2},

	//해적SSS블록
	{13,3,5,0,xSize,xSize * 2,xSize * 2 + 1,xSize * 3 + 1},
	{14,3,5,0,1,2,xSize - 1,xSize},
	{15,3,5,0,xSize,xSize + 1,xSize * 2 + 1,xSize * 3 + 1},
	{16,3,5,0,1,xSize - 2,xSize - 1,xSize},
	{17,3,5,0,1,2,xSize + 2,xSize + 3},
	{18,3,5,0,xSize - 1,xSize,xSize * 2 - 1,xSize * 3 - 1},
	{19,3,5,0,1,xSize + 1,xSize + 2,xSize + 3},
	{20,3,5,0,xSize,xSize * 2 - 1,xSize * 2,xSize * 3 - 1},

	//궁수SSS블록
	{21,4,5,0,1,2,3,4},
	{22,4,5,0,xSize,xSize * 2,xSize * 3,xSize * 4},

	//전사SS블록
	{23,5,4,0,1,xSize,xSize + 1,0},

	//도적SS블록
	{24,6,4,0,xSize,xSize + 1,xSize + 2,0},
	{25,6,4,0,1,xSize,xSize * 2,0},
	{26,6,4,0,1,2,xSize + 2,0},
	{27,6,4,0,xSize,xSize * 2 - 1,xSize * 2,0},
	{28,6,4,0,xSize,xSize * 2,xSize * 2 + 1,0},
	{29,6,4,0,1,2,xSize,0},
	{30,6,4,0,1,xSize + 1,xSize * 2 + 1,0},
	{31,6,4,0,xSize - 2,xSize - 1,xSize,0},

	//법사SS블록
	{32,7,4,0,xSize - 1,xSize,xSize + 1,0},
	{33,7,4,0,xSize,xSize + 1,xSize * 2,0},
	{34,7,4,0,1,2,xSize + 1,0},
	{35,7,4,0,xSize - 1,xSize,xSize * 2,0},

	//해적SS블록
	{36,8,4,0,xSize,xSize + 1,xSize * 2 + 1,0},
	{37,8,4,0,1,xSize - 1,xSize,0},
	{38,8,4,0,xSize - 1,xSize,xSize * 2 - 1},
	{39,8,4,0,1,xSize,xSize + 1,0},

	//궁수SS블록
	{40,9,4,0,1,2,3,0},
	{41,9,4,0,xSize,xSize * 2,xSize * 3,0},

	//전사,해적S블록
	{42,10,3,0,xSize - 1,xSize,0,0},
	{43,10,3,0,xSize,xSize + 1,0,0},
	{44,10,3,0,1,xSize,0,0},
	{45,10,3,0,1,xSize + 1,0,0},

	//도적,마법사,궁수S블록
	{46,11,3,0,1,2,0,0},
	{47,11,3,0,xSize,xSize * 2,0,0},

	//모든직업A블록
	{48,12,2,0,1,0,0,0},
	{49,12,2,0,xSize,0,0,0},

	//모든직업B블록
	{50,13,1,0,0,0,0,0}
};
const int startOfUniqueInBlockList[blockKind] = { 0,8,12,13,21,23,24,32,36,40,42,46,48,50 };//방향만 다른경우의 각 첫번째 블럭
int crtBlock; //현재 놓을것을 고려중인 블럭

int targetPlacedBlockCount = 0; //목표 점령수에 도달하면 리턴될것임 //★★★입력받아야함★★★
int placedBlockCount = 0; //지금까지 배치된 블록의 수

	//dps를 위한 변수 선언 및 초기화
int stack[44] = { 0, };
int stackCrtPoint = 0;


#include <iostream>
extern "C" __declspec(dllexport)int* temp(int* iBoard, int* iExtraBlockList, int iTargetPlacedBlockCount) {
	targetPlacedBlockCount = iTargetPlacedBlockCount;

	int temp = 0;

	//블럭수에 문제가 없을때만 실행
	for (int i = 0; i < xSize*ySize; ++i) {
		board[i] = iBoard[i];
	}
	for (int i = 0; i < blockKind; ++i) {
		extraBlockList[i] = *(iExtraBlockList + i);
		temp += extraBlockList[i];
	}
	if (targetPlacedBlockCount <= temp) { 
		autoAssign();
	}
	return stack;
};


bool autoAssign()
{
	bool isContinue = false;
	bool isBlockExist = true;
	while (true) {
		//블록선택
		isBlockExist = false;
		if (isContinue) {
			for (int i = crtBlock + 1; i < blockListSize; ++i) {
				if (extraBlockList[blockList[i].shapeType] > 0) {
					crtBlock = i;
					isBlockExist = true;
					break;
				}
			}
		}
		else {
			for (int i = 0; i < blockKind; ++i) {
				if (extraBlockList[i] > 0) {
					crtBlock = startOfUniqueInBlockList[i];
					isBlockExist = true;
					break;
				}
			}
		}

		if (!isBlockExist) { //선택가능한 블록이 없음
			//모든 경우의수를 다 해본 경우
			if (stackCrtPoint == 0) {
				stack[0] = blockListSize + 1;
				return false;
			}
			//최근 놨던 블록 취소
			else {
				bool first = true;
				for (int i = 0; i < xSize * ySize; ++i) {
					if (board[i] == placedBlockCount) {
						board[i] = 0;
						if (first == true) {
							crtLocation = i;
							first = false;
						}
					}
				}
				--placedBlockCount;
				crtBlock = stack[stackCrtPoint];
				++extraBlockList[blockList[crtBlock].shapeType];
				--stackCrtPoint;
				isContinue = true;
				continue;
			}
		}
		else {
			if (isContinue == false) {
				for (int i = 0; i < xSize * ySize; ++i) {
					if (board[i] == 0) {
						crtLocation = i;
						break;
					}
					else if (i == xSize * ySize - 1) {
						//사용가능한 빈칸이 없음
						return true;					
					}
				}
			}
			//블록과 빈칸이 정해진 후 블록 대보기
			if ((board[blockList[crtBlock].Detail2 + crtLocation] == 0)
				& (board[blockList[crtBlock].Detail3 + crtLocation] == 0)
				& (board[blockList[crtBlock].Detail4 + crtLocation] == 0)
				& (board[blockList[crtBlock].Detail5 + crtLocation] == 0)) {

				//블록놓기
				++placedBlockCount;
				board[blockList[crtBlock].Detail1 + crtLocation] = placedBlockCount;
				board[blockList[crtBlock].Detail2 + crtLocation] = placedBlockCount;
				board[blockList[crtBlock].Detail3 + crtLocation] = placedBlockCount;
				board[blockList[crtBlock].Detail4 + crtLocation] = placedBlockCount;
				board[blockList[crtBlock].Detail5 + crtLocation] = placedBlockCount;

				stack[stackCrtPoint] = crtBlock;
				if (targetPlacedBlockCount == placedBlockCount) {
					return true;
				}
				++stackCrtPoint;
				--extraBlockList[blockList[crtBlock].shapeType];
				isContinue = false;
			}
			else {
				isContinue = true;
			}
		}
	}

}
