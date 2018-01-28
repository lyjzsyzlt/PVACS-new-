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
extern float phmk[100][100];//mk��Ӧ����Ϣ�ؾ���
extern float phepc[100][100];//epc��Ӧ����Ϣ�ؾ���
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
	sol.push_back(-1);//ʹ��sol��1��ʼ���
}

void ant::createBatchSeq()
{
	vector<float> pb;// ��¼����ѡ��ÿ�����ĸ���
	pb.clear();
	pb.push_back(-1);//ʹ��pb��1��ʼ���

	int sum = 0;
	int SIZE = tempB.size();//���ĸ���
	for (int i = 1; i < SIZE; i++)
		sum += (SIZE - pos[i] + 1)*(SIZE - pos[i] + 1);

	for (int i = 1; i < SIZE; i++)
		pb.push_back((float)(SIZE - pos[i] + 1)*(SIZE - pos[i] + 1) / sum);

	float m = 0;
	float r = rand() % 100 / 100.0;

	// ���ո���ѡ���һ��
	for (int i = 1; i < SIZE; i++)
	{
		m += pb[i];
		if (r < m) {
			sol.push_back(i);
			tempB[i].flag = 0;//��ʾ������Ѿ�������
			break;
		}
	}
	// ����ѡ�����Ľ�
	for (int i = 2; i < SIZE; i++)// ����bNum-1������Ҫ����
	{
		float q = rand() % 100 / 100.0;
		float max = -999999.0;
		int select = -1;//ѡ������ı��
		if (q <= q0)
		{
			for (int j = 1; j < SIZE; j++)
			{
				if (tempB[j].flag == 1)// ��δ����ļ�������ѡ
				{
					float temp = pow((pow(Vmk, alpha)*phmk[sol.back()][j] + pow(Vepc, alpha)*phepc[sol.back()][j]), alpha)*pow(info[sol.back()][j], beta);
					if (temp > max)
					{
						max = temp;
						select = j;
					}
				}
			}
			//��ʱ�Ѿ��ҳ�������
			sol.push_back(select);//��select�Ž�ȥ
			tempB[select].flag = 0;//��ʶ��ֵΪ0
		}
		//q>q0ʱ,H
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
	out << id << "���ϵ�����Ϊ��";
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
	vector<int> SOL;//���ÿһ�׶ε������У�����ĸ���ǰ��ģ�
	SOL = sol;
	tempB = B;
	//�����ֻ���ϲ����Ļ�����Ϣ
	for (int i = 1; i < kmax+1; i++)
	{
		for (int j = 1; j < machineNum[i]+1; j++)
		{
			M[i][j].avt = 0;
			M[i][j].List.clear();
		}
	}

	//����ÿ���׶�
	for (int k = 1; k < kmax+1; k++)
	{
		//����ÿһ������ʹ��ÿ�����ڵ�ǰ�׶η����ں��ʵĻ�����
		for (int h = 1; h < tempB.size(); h++)
		{
			float min = 9999999;//����ʽ�ӵ���Сֵ
			int selectMachine = 1;//����ѡ��Ļ���
			float max1 = -9999;
			float max2 = -9999;

			int CT = tempB[SOL[h]].BC[k - 1];//��һ�׶ε����ʱ��

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
		
			//����������
			if (M[k][selectMachine].List.empty())
				EPC += getEPC1(0, max(CT, M[k][selectMachine].avt),0,0);
			else
				EPC+= getEPC1(M[k][selectMachine].List.back().BC[k], max(CT, M[k][selectMachine].avt) - M[k][selectMachine].List.back().BC[k],0,0);
			//���㹤�����
			EPC += getEPC1(max(CT, M[k][selectMachine].avt), ceil(float(tempB[SOL[h]].BP[k]) / M[k][selectMachine].V), k, selectMachine);
			
			tempB[SOL[h]].BC[k] = max(CT, M[k][selectMachine].avt) + ceil(float(tempB[SOL[h]].BP[k]) / M[k][selectMachine].V);//��������������ʱ��
			tempB[SOL[h]].ct = tempB[SOL[h]].BC[k];//���ڵ�ǰ�׶ε��깤ʱ�䣬Ϊ�˺��������
			tempB[SOL[h]].BS[k] = max(CT, M[k][selectMachine].avt);//�������Ŀ�ʼʱ��
			tempB[SOL[h]].MID[k] = selectMachine;
			//����ѡ�����̨�����Ŀ���ʱ��
			M[k][selectMachine].avt = tempB[SOL[h]].BC[k];

			//�������������Ӧ�Ļ���������
			M[k][selectMachine].List.push_back(tempB[SOL[h]]);

		}//��ǰ�׶Σ����е������Ѿ��������

		//�����е����������ʱ���������
		vector<batch>tempBB=tempB;
		sort(tempBB.begin()+1, tempBB.end(), cmp3);
		
		Cmax = tempBB.back().BC[k];
		//��ȡ��һ���׶ε�������
		for (int j = 1; j < tempBB.size(); j++)
			SOL[j] = tempBB[j].BId;
	}//���н׶ζ��Ѿ����

	//ǰ�������ʱ������û����ĳ̨�������һ������ɺ󣬻�Ҫ������ã���Ϊ��ʱ����֪��Cmax��
	for (int k = 1; k < kmax+1; k++)
	{
		for (int i = 1; i < machineNum[k]+1; i++)
		{
			//������û���κ�һ������һֱ���ڴ���״̬
			if (M[k][i].List.empty())
				EPC += getEPC1(0, Cmax,0,0);
			else
				EPC += getEPC1(M[k][i].List.back().BC[k], Cmax - M[k][i].List.back().BC[k],0,0);
		}
	}
	out << "---------------------------------------" << endl;

	//���ÿ���׶�ÿ̨�����ϵ������
	for (int k = 1; k < kmax + 1; k++)
	{
		for (int i = 1; i < machineNum[k] + 1; i++)
		{
			out << "��" << k << "�׶Σ���" << i << "̨������" << endl;
			list<batch>::iterator it = M[k][i].List.begin();
			for (; it != M[k][i].List.end(); it++)
				out << '\t' << "����" << (*it).BId << '\t' << "��ʼʱ�䣺" << (*it).BS[k] << '\t' << "���ʱ�䣺" << (*it).BC[k]<<'\t'<<"������"<<(*it).MID[k]<< endl;
		}
	}
	//����ܵĵ�ѣ�Cmax
	out << "EPC=" << EPC << '\t' << "Cmax=" << Cmax << endl;

	//Right_Shift�Ľ�
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
			int ST;//��ST(k+1)
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

			//Ѱ����С��t
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

			tempB[D[h]].BS[k] = t0;//���¿�ʼ�ӹ�ʱ��
			tempB[D[h]].BC[k] = t0 + ceil((float)tempB[D[h]].BP[k] / M[k][tempB[D[h]].MID[k]].V);//�������ʱ��

			//���»�������
			 it = M[k][tempB[D[h]].MID[k]].List.begin();
			for (; it != M[k][tempB[D[h]].MID[k]].List.end(); it++)
				if ((*it).BId == tempB[D[h]].BId)
				{
					(*it) = tempB[D[h]];
				}
		}
	}
	//����Ľ�������
	out << endl;
	out << "----------Right_Shift֮��-------------��" << endl;

	for (int k = 1; k < kmax + 1; k++)
	{
		for (int i = 1; i < machineNum[k] + 1; i++)
		{
			out << "��" << k << "�׶Σ���" << i << "̨������" << endl;
			list<batch>::iterator it = M[k][i].List.begin();
			for (; it != M[k][i].List.end(); it++)
				out << '\t' << "����" << (*it).BId << '\t' << "��ʼʱ�䣺" << (*it).BS[k] << '\t' << "���ʱ�䣺" << (*it).BC[k] << '\t' << "������" << (*it).MID[k] << endl;
		}
	}
	//����Ľ����EPC
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
