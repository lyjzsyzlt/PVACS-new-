// PVACS2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include"structs.h"
#include"functions.h"
#include"ant.h"
#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<random>
#include<set>
#include<time.h>
extern vector<batch> B;
extern float  phmk[100][100];//mk对应的信息素矩阵
extern float  phepc[100][100];//epc对应的信息素矩阵
extern set<solution> NDS;
int main()
{
	srand((unsigned)time(NULL));
	vector<job> J;
	initJob(J);
	BFLPT(J);
	displayBatch();
	computeSIb();
	initPh();
	initMachine();
	ant* ANT = new ant[antNum+1];
	int t = Tmax;
	ofstream out,out1;
	out1.open("LS_Process.txt", ios_base::ate);
	out1 << "";
	out.open("LS_Process.txt", ios_base::out | ios_base::app);
	
	while (t--)
	{
		out << "***********************第" << (Tmax - t) << "次迭代**************************" << endl;
		for (int i = 1; i < antNum + 1; i++)
		{
			ANT[i].generateV();
			ANT[i].id = i;
			ANT[i].createBatchSeq();
			ANT[i].LS();
			ANT[i].localUpdate();
			ANT[i].updateNDS();
		}
		cout << "NDS.size()" << NDS.size() << endl;
		globalUpdate();
	}
	

	cout << "mk信息素：" << endl;
	for (int i = 1; i < B.size(); i++)
	{
		for (int j = 1; j < B.size(); j++)
			cout << setw(4) << phmk[i][j];
		cout << endl;
	}
	
	
   return 0;
}

