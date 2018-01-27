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
public:
	int id;
	vector<int> sol;//��Ž�
	vector<batch> tempB;
	ant()
	{
		Vmk = 0.8;
		Vepc = 0.2;
	};
	void generateV();//����ƫ������������ֵ
	void createBatchSeq();// ����⣨�����У�
	void LS();// LS�㷨
	int getEPC1(int t, int detaT, int k, int i);// ����EPC��tΪ�½磬detaTΪ�������̵�ʱ��,����Ϊ��k�׶εĵ�l̨����
	int getEPC2(int t, int detaT);//��������Ĵ������
	void Right_Shift();
	~ant();
};
#endif // !ant_H