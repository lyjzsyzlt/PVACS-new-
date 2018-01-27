// PVACS2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include"structs.h"
#include"functions.h"
#include"ant.h"
#include<iostream>
#include<vector>
#include<fstream>
#include<random>
#include<time.h>
extern vector<batch> B;
int main()
{
	srand((unsigned)time(NULL));
	/*vector<job> J;
	initJob(J);
	BFLPT(J);
	displayBatch();
	computeSIb();*/
	initPh();
	initMachine();
	ant Ant1,Ant2;
	//Ant.generateV();
	//Ant.createBatchSeq();

	batch tempB;
	initBatch(tempB, -1);
	tempB.freeSpace = -10;
	tempB.flag = 0;
	B.push_back(tempB);
	ifstream in;
	in.open("batch.txt", ios::in | ios::binary);
	for (int i = 1;i < 6; i++)
	{
		batch tempB;
		in >> tempB.BId;
		in >> tempB.BP[1];
		in >> tempB.BP[2];
		tempB.flag = 1;
		tempB.BC[0] = 0;
		B.push_back(tempB);
	}
	Ant1.sol = { -1,1,2,3,4,5 };
	Ant1.LS();
	Ant2.sol = { -1,1,2,3,4,5 };
	Ant2.LS();
	//cout<<"EPC="<<Ant.getEPC1(0, 5, 1, 3);

   return 0;
}

