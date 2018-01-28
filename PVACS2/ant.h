#pragma once
#ifndef ant_H
#define ant_H

#include"functions.h"
#include"structs.h"
#include"Machine.h"
class ant
{
private:
	// �û�ƫ������
	float Vmk;
	float Vepc;
	int Cmax;//��¼���makespan
	int EPC;//��¼���յ�EPC
	int newEPC;//��¼��Right_Shift֮���EPC
	ofstream out;
public:
	int id;
	vector<int> sol;//��Ž�
	vector<batch> tempB;
	ant()
	{
		out.open("LS_Process.txt", ios_base::out | ios_base::app);
	};
	void generateV();//����ƫ������������ֵ
	void createBatchSeq();// ����⣨�����У�
	void LS();// LS�㷨
	int getEPC1(int t, int detaT, int k, int i);// ����EPC��tΪ�½磬detaTΪ�������̵�ʱ��,����Ϊ��k�׶εĵ�l̨����
	int getEPC2(int t, int detaT);//��������Ĵ������
	void Right_Shift();//Right_Shift�Ľ�
	void localUpdate(); 
	void updateNDS();
	~ant();
};
#endif // !ant_H