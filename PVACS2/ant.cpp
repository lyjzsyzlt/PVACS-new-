#include "stdafx.h"
#include "ant.h"
#include"defines.h"
#include"Machine.h"
#include<random>
#include<set>
#include<algorithm>
#include<iostream>

using namespace std;

extern vector<batch> B;
extern int info[100][100];
extern int pos[100];
extern float phmk[100][100];//mk对应的信息素矩阵
extern float phepc[100][100];//epc对应的信息素矩阵
extern int machineNum[kmax + 1];
extern Machine **M;
extern set<solution> NDS;

void ant::generateV()
{
	Vmk = rand() % 100 / 100.0;
	Vepc = 1 - Vmk;
	tempB = B;
	EPC = 0;
	sol.clear();
	sol.push_back(-1);//使得sol从1开始编号
}

void ant::createBatchSeq()
{
	vector<float> pb;// 记录蚂蚁选择每个批的概率
	pb.clear();
	pb.push_back(-1);//使得pb从1开始编号

	int sum = 0;
	int SIZE = tempB.size();//批的个数
	for (int i = 1; i < SIZE; i++)
		sum += (SIZE - pos[i] + 1)*(SIZE - pos[i] + 1);

	for (int i = 1; i < SIZE; i++)
		pb.push_back((float)(SIZE - pos[i] + 1)*(SIZE - pos[i] + 1) / sum);

	float m = 0;
	float r = rand() % 100 / 100.0;

	// 按照概率选择第一批
	for (int i = 1; i < SIZE; i++)
	{
		m += pb[i];
		if (r < m) {
			sol.push_back(i);
			tempB[i].flag = 0;//表示这个批已经加入了
			break;
		}
	}
	// 继续选择后面的解
	for (int i = 2; i < SIZE; i++)// 还有bNum-1个批需要加入
	{
		float q = rand() % 100 / 100.0;
		float max = -999999.0;
		int select = -1;//选择的批的编号
		if (q <= q0)
		{
			for (int j = 1; j < SIZE; j++)
			{
				if (tempB[j].flag == 1)// 从未加入的集合里面选
				{
					float temp = pow((pow(Vmk, alpha)*phmk[sol.back()][j] + pow(Vepc, alpha)*phepc[sol.back()][j]), alpha)*pow(info[sol.back()][j], beta);
					if (temp > max)
					{
						max = temp;
						select = j;
					}
				}
			}
			//此时已经找出最大的了
			sol.push_back(select);//把select放进去
			tempB[select].flag = 0;//标识赋值为0
		}
		//q>q0时,H
		else
		{
			float sum = 0;
			for (int l = 1; l < SIZE; l++)
			{
				if (tempB[l].flag == 1)
					sum += pow((pow(Vmk, alpha)*phmk[sol.back()][l] + pow(Vepc, alpha)*phepc[sol.back()][l]), alpha)*pow(info[sol.back()][l], beta);
			}
			float P[100];
			for (int p = 1; p <SIZE; p++)
			{
				if (tempB[p].flag == 1)
				{
					P[p] = pow((pow(Vmk, alpha)*phmk[sol.back()][p] + pow(Vepc, alpha)*phepc[sol.back()][p]), alpha)*pow(info[sol.back()][p], beta) / sum;
				}
				else
				{
					P[p] = 0;
				}
			}

			float r = rand() % 100 / 100.0;
			float temp = 0;
			for (int t = 1; t < SIZE; t++)
			{
				temp += P[t];
				if (r < temp)
				{
					sol.push_back(t);
					tempB[t].flag = 0;
					break;
				}
			}
		}
	}
	out << id << "蚂蚁的序列为：";
	for (int i = 1; i < sol.size(); i++)
	{
		out << sol[i] << "  ";
	}
	out << endl;
	/*for (int i = 1; i < sol.size(); i++)
	{
		cout << i << "   " << tempB[sol[i]].BId << "  " << tempB[sol[i]].JNum << "  " << tempB[sol[i]].BP[1] << endl;
	}*/
}

void ant::LS()
{
	vector<int> SOL;//存放每一阶段的批序列（后面的覆盖前面的）
	SOL = sol;
	tempB = B;
	//清除上只蚂蚁残留的机器信息
	for (int i = 1; i < kmax+1; i++)
	{
		for (int j = 1; j < machineNum[i]+1; j++)
		{
			M[i][j].avt = 0;
			M[i][j].List.clear();
		}
	}

	//对于每个阶段
	for (int k = 1; k < kmax+1; k++)
	{
		//遍历每一个批，使得每个批在当前阶段分配在合适的机器上
		for (int h = 1; h < tempB.size(); h++)
		{
			float min = 9999999;//整个式子的最小值
			int selectMachine = 1;//最终选择的机器
			float max1 = -9999;
			float max2 = -9999;

			int CT = tempB[SOL[h]].BC[k - 1];//上一阶段的完成时间

			for (int i = 1; i < machineNum[k]+1; i++)
			{
				int temp1;

				temp1 = max(CT, M[k][i].avt) + ceil((float)tempB[SOL[h]].BP[k]/M[k][i].V) - CT;
				if (temp1 > max1)
					max1 = temp1;

				float temp2 = getEPC1(max(CT, M[k][i].avt), ceil((float)tempB[SOL[h]].BP[k] / M[k][i].V), k, i);
				if (temp2 > max2)
					max2 = temp2;
			}

			for (int i = 1; i < machineNum[k]+1; i++)
			{
				float temp = Vmk * (max(CT, M[k][i].avt) + ceil((float)tempB[SOL[h]].BP[k] / M[k][i].V) - CT) / max1 + Vepc * getEPC1(max(CT, M[k][i].avt), ceil((float)tempB[SOL[h]].BP[k] / M[k][i].V), k, i) / max2;
				if (temp < min)
				{
					min = temp;
					selectMachine = i;
				}
			}
		
			//计算待机电费
			if (M[k][selectMachine].List.empty())
				EPC += getEPC1(0, max(CT, M[k][selectMachine].avt),0,0);
			else
				EPC+= getEPC1(M[k][selectMachine].List.back().BC[k], max(CT, M[k][selectMachine].avt) - M[k][selectMachine].List.back().BC[k],0,0);
			//计算工作电费
			EPC += getEPC1(max(CT, M[k][selectMachine].avt), ceil(float(tempB[SOL[h]].BP[k]) / M[k][selectMachine].V), k, selectMachine);
			
			tempB[SOL[h]].BC[k] = max(CT, M[k][selectMachine].avt) + ceil(float(tempB[SOL[h]].BP[k]) / M[k][selectMachine].V);//更新这个批的完成时间
			tempB[SOL[h]].ct = tempB[SOL[h]].BC[k];//批在当前阶段的完工时间，为了后面的排序
			tempB[SOL[h]].BS[k] = max(CT, M[k][selectMachine].avt);//更新批的开始时间
			tempB[SOL[h]].MID[k] = selectMachine;
			//更新选择的这台机器的可用时间
			M[k][selectMachine].avt = tempB[SOL[h]].BC[k];

			//将这个批加入相应的机器队列中
			M[k][selectMachine].List.push_back(tempB[SOL[h]]);

		}//当前阶段，所有的批都已经分配好了

		//将所有的批按照完成时间递增排序
		vector<batch>tempBB=tempB;
		sort(tempBB.begin()+1, tempBB.end(), cmp3);
		
		Cmax = tempBB.back().BC[k];
		//获取下一个阶段的批序列
		for (int j = 1; j < tempBB.size(); j++)
			SOL[j] = tempBB[j].BId;
	}//所有阶段都已经完成

	//前面计算电费时，由于没计算某台机器最后一个批完成后，还要待机多久（因为当时还不知道Cmax）
	for (int k = 1; k < kmax+1; k++)
	{
		for (int i = 1; i < machineNum[k]+1; i++)
		{
			//机器上没有任何一个批，一直处于待机状态
			if (M[k][i].List.empty())
				EPC += getEPC1(0, Cmax,0,0);
			else
				EPC += getEPC1(M[k][i].List.back().BC[k], Cmax - M[k][i].List.back().BC[k],0,0);
		}
	}
	out << "---------------------------------------" << endl;

	//输出每个阶段每台机器上的批情况
	for (int k = 1; k < kmax + 1; k++)
	{
		for (int i = 1; i < machineNum[k] + 1; i++)
		{
			out << "第" << k << "阶段，第" << i << "台机器：" << endl;
			list<batch>::iterator it = M[k][i].List.begin();
			for (; it != M[k][i].List.end(); it++)
				out << '\t' << "批：" << (*it).BId << '\t' << "开始时间：" << (*it).BS[k] << '\t' << "完成时间：" << (*it).BC[k]<<'\t'<<"机器："<<(*it).MID[k]<< endl;
		}
	}
	//输出总的电费，Cmax
	out << "EPC=" << EPC << '\t' << "Cmax=" << Cmax << endl;

	//Right_Shift改进
	Right_Shift();
}

int ant::getEPC1(int t, int detaT, int k, int i)
{
	int n = t / 20 + 1;
	if (t >= 20 * n - 20 && t < 20 * n - 10)
	{
		if (t + detaT >= 20 * n - 10)
			return (20 * n - 10 - t) * 10 * M[k][i].PW + getEPC1(20 * n - 10, detaT - (20 * n - 10 - t),k,i);
		else
			return detaT * 10 * M[k][i].PW;
	}
	if (t >= 20 * n - 10 && t < 20 * n)
	{
		if (t + detaT > 20 * n)
			return (20 * n - t) * 5 * M[k][i].PW + getEPC1(20 * n, detaT - (20 * n-t), k, i);
		else
			return detaT * 5 * M[k][i].PW;
	}
}

int ant::getEPC2(int t, int detaT)
{
	return 0.0f;
}

void ant::Right_Shift()
{
	for (int k = kmax; k >= 1; k--)
	{
		vector<batch> tempBB = tempB;
		vector<int> D;
		D.clear();
		D.push_back(-1);
		for (int i = 1; i < tempBB.size(); i++)
			tempBB[i].ct = tempBB[i].BC[k];

		sort(tempBB.begin() + 1, tempBB.end(), cmp4);
		for (int i = 1; i < tempB.size(); i++)
			D.push_back(tempBB[i].BId);

		for (int h = 1; h < tempB.size(); h++)
		{
			int tmin = tempB[D[h]].BS[k];
			int STnext;
			int ST;//即ST(k+1)
			list<batch>::iterator it = M[k][tempB[D[h]].MID[k]].List.begin();
			for (; it != M[k][tempB[D[h]].MID[k]].List.end(); it++)
				if ((*it).BId == tempB[D[h]].BId)
					break;
			it++;
			if (it == M[k][tempB[D[h]].MID[k]].List.end())
				STnext = 9999999;
			else 
				STnext = (*it).BS[k];
			if (k == kmax)
				ST = 9999999;				
			else
				ST = tempB[D[h]].BS[k + 1];
			int tmax = min(Cmax, STnext, ST) - ceil((float)tempB[D[h]].BP[k] / M[k][tempB[D[h]].MID[k]].V);

			//寻找最小的t
			int minEPC = 99999999;
			int t0;
			for (int t = tmin; t <= tmax; t++)
			{
				int tempEPC = getEPC1(t, ceil((float)tempB[D[h]].BP[k] / M[k][tempB[D[h]].MID[k]].V), k, tempB[D[h]].MID[k]);
				if (tempEPC <= minEPC)
				{
					minEPC = tempEPC;
					t0 = t;
				}
					
			}

			tempB[D[h]].BS[k] = t0;//更新开始加工时间
			tempB[D[h]].BC[k] = t0 + ceil((float)tempB[D[h]].BP[k] / M[k][tempB[D[h]].MID[k]].V);//更新完成时间

			//更新机器队列
			 it = M[k][tempB[D[h]].MID[k]].List.begin();
			for (; it != M[k][tempB[D[h]].MID[k]].List.end(); it++)
				if ((*it).BId == tempB[D[h]].BId)
				{
					(*it) = tempB[D[h]];
				}
		}
	}
	//输出改进后的情况
	out << endl;
	out << "----------Right_Shift之后-------------：" << endl;

	for (int k = 1; k < kmax + 1; k++)
	{
		for (int i = 1; i < machineNum[k] + 1; i++)
		{
			out << "第" << k << "阶段，第" << i << "台机器：" << endl;
			list<batch>::iterator it = M[k][i].List.begin();
			for (; it != M[k][i].List.end(); it++)
				out << '\t' << "批：" << (*it).BId << '\t' << "开始时间：" << (*it).BS[k] << '\t' << "完成时间：" << (*it).BC[k] << '\t' << "机器：" << (*it).MID[k] << endl;
		}
	}
	//计算改进后的EPC
	int newEPC=0;
	for (int k = 1; k < kmax+1; k++)
	{
		for (int i = 1; i < machineNum[k]+1; i++)
		{
			if (M[k][i].List.empty())
				newEPC += getEPC1(0, Cmax, 0, 0);
			else
			{
				list<batch>::iterator it = M[k][i].List.begin();
				list<batch>::iterator it1;
				for (; it != M[k][i].List.end(); it++)
				{
					if (it == M[k][i].List.begin())
					{
						newEPC += getEPC1(0, (*it).BS[k], 0, 0);
						newEPC += getEPC1((*it).BS[k], ceil((float)(*it).BP[k] / M[k][i].V), k, i);
					}
					else
					{
						it1 = (--it);
						it++;
						newEPC += getEPC1((*it1).BC[k], (*it).BS[k] - (*it1).BC[k], 0, 0);
						newEPC += getEPC1((*it).BS[k], ceil((float)(*it).BP[k] / M[k][i].V), k, i);
					}
				}
				newEPC += getEPC1(M[k][i].List.back().BC[k], Cmax - M[k][i].List.back().BC[k], 0, 0);
			}
			
			
		}
	}
	out << "newEPC=" << newEPC << '\t'<<"Cmax="<<Cmax<<endl;
	out << "=================================================" << endl << endl;
}

void ant::localUpdate()
{
	for (int i = 1; i < sol.size()-1; i++)
	{
		phmk[i][i + 1] = (1 - pl)*phmk[i][i + 1] + pl * tao;
		phepc[i][i + 1] = (1 - pl)*phepc[i][i + 1] + pl * tao;
	}
}

void ant::updateNDS()
{
	solution tempSol;
	tempSol.batchSeq = sol;
	tempSol.Cmax = Cmax;
	tempSol.EPC = newEPC;
	tempSol.Vmk = Vmk;
	tempSol.Vepc = Vepc;

	int flag = 0;
	set<solution>::iterator it = NDS.begin();
	for (; it != NDS.end(); )
	{
		if (tempSol.Cmax > (*it).Cmax&&tempSol.EPC > (*it).EPC) {
			flag = 1;
			break;
		}
		else if (tempSol.Cmax < (*it).Cmax&&tempSol.EPC < (*it).EPC) {
			it = NDS.erase(it);
		}
		else if (tempSol.Cmax == (*it).Cmax&&tempSol.EPC == (*it).EPC) {
			flag = 1;
			break;
		}
		else if (tempSol.Cmax == (*it).Cmax || (*it).EPC == tempSol.EPC) {
			if (tempSol.Cmax > (*it).Cmax || tempSol.EPC > (*it).EPC) {
				flag = 1;
				break;
			}
			else {
				it = NDS.erase(it);
			}
		}
		else
			it++;
	}
	if (flag == 0)
		NDS.insert(tempSol);
}

ant::~ant()
{

}
