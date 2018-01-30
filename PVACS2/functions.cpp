#include"stdafx.h"
#include"functions.h"
#include"structs.h"
#include"Machine.h"
#include<algorithm>
#include<set>
#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include<time.h>
#include<random>
using namespace std;

vector<batch> B;

int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b, int c)
{
	int m;
	if (a < b)
		m = a;
	else
		m = b;
	if (c < m)
		m = c;
	return m;
}
bool cmp(const job &J1, const job &J2)
{
	return J1.JobPT[1] > J2.JobPT[1];//���յ�һ�׶εļӹ�ʱ������
}

bool cmp2(const batch &b1, const batch &b2)
{
	return b1.SI > b2.SI;//����������б�Ƚ�������
}

bool cmp3(const batch &b1, const batch &b2)
{
	return b1.ct < b2.ct;//�������ʱ������
}

bool cmp4(const batch &b1, const batch &b2)
{
	return b1.ct > b2.ct;//�������ʱ�併��
}

// ��ʼ����������Ϣ
void initJob(vector<job> &J)
{
	/*// ��job.txt�ļ��ж�ȡ�����ı�ţ���С�������׶ε�����ӹ�ʱ��
	ifstream in;
	in.open("job.txt", ios::in | ios::binary);
	for (int i = 1; i < JobNum+1; i++)
	{
		job tempj;
		batch b;
		in >> tempj.JobId;
		in >> tempj.JobSize;
		for (int j = 1; j < kmax+1; j++) {
			in >> tempj.JobPT[j];
		}
		J.push_back(tempj);
	}*/
	J.clear();
	ofstream out1("jobInfo.txt", ios::ate);
	out1 << "";

	ofstream out("jobInfo.txt", ios::app);
	default_random_engine dre((unsigned)time(NULL));// ��ʼ�����������
	uniform_int_distribution<int> uid(1, 10);// ʹ�ø����ķ�Χ��ʼ���ֲ���
	out << "����id" << '\t' << "������С" << '\t';
	for (int i = 1; i < kmax + 1; i++)
		out << "��" << i << "�׶�����ӹ�ʱ��" << '\t';
	out << endl;

	for (int i = 1; i < JobNum + 1; i++)
	{
		job tempj;
		tempj.JobId = i;
		tempj.JobSize = uid(dre);
		out << tempj.JobId << '\t' << tempj.JobSize << '\t';

		uniform_int_distribution<int> pt(10, 50);
		for (int j = 1; j < kmax + 1; j++) {
			tempj.JobPT[j] = pt(dre);
			out << tempj.JobPT[j] << '\t';
		}
		out << endl;
			
		J.push_back(tempj);
		
	}
}

// ��ʼ�����Ļ�����Ϣ
void initBatch(batch &B, int id)
{
	for (int i = 1; i < kmax+1; i++)
	{
		B.BId = id;
		B.BP[i] = -1;
		B.freeSpace = C;
		B.JNum = 0;
		B.flag = 1;
		B.BC[0] = 0;
	}
}

// ���������������ӹ�ʱ��
void getPbk(vector<job> J, vector<batch> &B)
{
	for (int i = 1; i < B.size(); i++) //��i����
	{
		for (int p = 1; p < kmax+1; p++)// ��p���׶�
		{
			int maxPT = -999999;
			for (int j = 0; j < B[i].JNum; j++)// ��j������
			{
				if (B[i].BJob[j].JobPT[p] > maxPT)
				{
					maxPT = B[i].BJob[j].JobPT[p];
				}
			}
			B[i].BP[p] = maxPT;
		}
	}
}

// ����
void BFLPT(vector<job> J/*, vector<batch> &B*/)
{
	B.clear();
	sort(J.begin(), J.end(), cmp);//�Թ������ռӹ�ʱ��ǵ���˳������

	//��ҪΪ��ʹBʹ��1��ʼ���
	batch tempB;
	initBatch(tempB, -1);
	tempB.freeSpace = -10;
	tempB.flag = 0;
	B.push_back(tempB);

	vector<job>::iterator it2 = J.begin();//���������ĵ�����
	int initBid = 1;//���ĳ�ʼ���

	for (it2; it2 != J.end(); it2++)//�������еĹ���
	{
		int min = 9999999;//��С�ĵ�ʣ��ռ�
		int minBid = -1;//��Ӧ�������
		vector<batch>::iterator it = B.begin()+1;//�������ĵ�����
		for (it; it != B.end(); it++)//Ϊÿ����������һ�ε�ǰ��������
		{
			if ((*it).freeSpace >= (*it2).JobSize)//��ǰ��������㹻�����������
			{
				if ((*it).freeSpace - (*it2).JobSize < min)//�жϷ���֮���Ƿ�ʹ�����������������������ʣ��ռ���С
				{
					min = (*it).freeSpace - (*it2).JobSize;
					minBid = (*it).BId;
				}
			}
		}
		//���½���һ����
		if (minBid == -1)
		{
			batch tempB;
			initBatch(tempB, initBid);
			tempB.freeSpace -= (*it2).JobSize;
			tempB.JNum++;
			tempB.BJob.push_back((*it2));
			initBid++;
			B.push_back(tempB);
		}
		// ������������������������
		else
		{
			B[minBid].freeSpace -= (*it2).JobSize;
			B[minBid].JNum++;
			B[minBid].BJob.push_back((*it2));
		}
	}
	getPbk(J, B);//��ȫ�����������׶ε�����ӹ�ʱ��
}

// ��������������Ĺ���
void displayBatch(/*vector<batch> B*/)//������const vector<batch> B
{
	for (int i = 1; i<B.size(); i++) {
		cout << "��" << i << "�����ı�ţ�" << B[i].BId << endl;;
		cout << "��" << i << "����������������" << B[i].JNum << endl;
		cout << "��" << i << "�����������Ĺ���Ϊ(��ţ�:";

		for (int p = 0; p < B[i].JNum; p++) {
			cout << B[i].BJob[p].JobId << "  ";
		}
		cout << endl;

		for (int j = 1; j < kmax+1; j++) {//k���׶�
			cout << "    ��" << j  << "�׶ε�����ӹ�ʱ��:" << B[i].BP[j] << endl;
		}
	}
}


int info[100][100];//����ʽ��Ϣ����
int pos[100];//����λ��
void computeSIb(/*vector<batch> &B*/)
{
	memset(info, 0, sizeof(info));
	memset(pos, 0, sizeof(pos));

	vector<batch> tempB = B;

	for (int i = 1; i < B.size(); i++)
	{
		float sum = 0;
		for (int j = 1; j < kmax+1; j++)
		{
			sum += ((kmax - 2 * j + 1) / 2.0*B[i].BP[j]);
		}
		B[i].SI = (-1)*sum;
		tempB[i].SI = (-1)*sum;
	}
	sort(tempB.begin()+1, tempB.end(), cmp2);

	// ����pos
	for (int i = 1; i < B.size(); i++)
	{
		for (int j = 1; j < B.size(); j++)
		{
			if (tempB[j].BId == B[i].BId)
			{
				pos[i] = j;
				break;
			}
		}
	}

	// ��������ʽ��Ϣ����
	for (int i = 0; i < B.size(); i++)
	{
		for (int j = 0; j < B.size(); j++)
		{
			info[i][j] = B.size()-1 - abs(pos[i] - pos[j]);
		}
	}
}

float phmk[100][100];//mk��Ӧ����Ϣ�ؾ���
float phepc[100][100];//epc��Ӧ����Ϣ�ؾ���

// ��ʼ����Ϣ�ؾ���
void initPh()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			phmk[i][j] = tao;
			phepc[i][j] = tao;
		}
	}
}

//��ʼ��������Ϣ
Machine **M;
int machineNum[kmax + 1];//���ÿ���׶εĻ�������

void initMachine()
{
	ifstream in,in2;
	in.open("machine.txt", ios::in | ios::binary);

	//��ʼ��ÿ���׶λ���������
	in2.open("machineNum.txt", ios::in | ios::binary);
	for(int i=1;i<kmax+1;i++)
		in2 >> machineNum[i];
	M = new Machine*[kmax+1];
	for (int i = 0; i < kmax+1; i++)
		M[i] = new Machine[10];
	M[0][0].PW = 1;

	default_random_engine dre((unsigned)time(NULL));// ��ʼ�����������
	uniform_int_distribution<int> v(1, 5);// ʹ�ø����ķ�Χ��ʼ���ֲ���
	uniform_int_distribution<int> pw(5, 10);
	ofstream out1("machineInfo.txt", ios::ate);
	out1 << "";
	ofstream out("machineInfo.txt", ios::app);
	out << "�׶�" << '\t' << "������" << '\t' << "�ٶ�" << '\t' << "����" << endl;

	for (int i = 1; i < kmax+1; i++)
	{
		
		for (int j = 1; j < machineNum[i]+1; j++)
		{
			out <<  i << '\t';
			//in >> M[i][j].V >> M[i][j].PW;
			out <<j<< '\t';
			M[i][j].V = v(dre);
			M[i][j].PW = pw(dre);
			M[i][j].avt = 0;
			M[i][j].List.clear();
			
			out << M[i][j].V << '\t' << M[i][j].PW << endl;
		}
	}
}

set<solution> NDS;

void globalUpdate()
{
	//�ֱ����NDS��EPC��Cmax����Сֵ
	set<solution>::iterator it = NDS.begin();
	int minEPC = 99999999;
	int minCmax = 99999999;
	for (; it != NDS.end(); it++)
	{
		if ((*it).EPC < minEPC)
			minEPC = (*it).EPC;
		if ((*it).Cmax < minCmax)
			minCmax = (*it).Cmax;
	}

	float detaPhmk[100][100];
	float detaPhepc[100][100];
	memset(detaPhmk, 0, sizeof(detaPhmk));
	memset(detaPhepc, 0, sizeof(detaPhepc));

	for (it = NDS.begin(); it != NDS.end(); it++)
	{
		float detaMk = (float)minCmax / (*it).Cmax;
		float detaEpc = (float)minEPC / (*it).EPC;

		for (int i = 1; i < (*it).batchSeq.size() - 1; i++)
		{
			detaPhmk[(*it).batchSeq[i]][(*it).batchSeq[i + 1]] += detaMk;
			detaPhepc[(*it).batchSeq[i]][(*it).batchSeq[i + 1]] += detaEpc;
		}
	}
	it = NDS.begin();
	if (!NDS.empty())
	{
		for (int i = 1; i < (*it).batchSeq.size() ; i++)
		{
			for (int j = 1; j < (*it).batchSeq.size(); j++)
			{
				phmk[i][j] = (1 - pg)*phmk[i][j] + pg * detaPhmk[i][j];
				phepc[i][j] = (1 - pg)*phepc[i][j] + pg * detaPhepc[i][j];
				if (phmk[i][j] < taoMin)
					phmk[i][j] = taoMin;
				if (phmk[i][j] > taoMax)
					phmk[i][j] = taoMax;
				if (phepc[i][j] < taoMin)
					phepc[i][j] = taoMin;
				if (phepc[i][j] > taoMax)
					phepc[i][j] = taoMax;
			}
		}
	}
	
}
