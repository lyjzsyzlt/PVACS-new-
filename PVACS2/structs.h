#pragma once
#ifndef structs_H
#define structs_H

#include"defines.h"
#include<vector>
using namespace std;
// ����
struct job
{
	int JobId;//�������
	int JobSize;//�����ߴ�
	int JobPT[kmax+1];//�����ٲ�ͬ�׶εĴ���ʱ��,��1��ʼ���
};

// ��
struct batch
{
	int BId;//����ţ���1��ʼ

	int BP[kmax+1];//���ٲ�ͬ�׶����崦��ʱ��,��1��ʼ
	int BC[kmax+1];//�������ڲ�ͬ�׶ε����ʱ��
	int JNum;//���й�������
	int freeSpace;//��¼����ʣ��ռ�
	vector<job> BJob;//�������ŵĹ���
	float SI;//������б��
	int flag ;//1��ʾ������û�м������
	int ct;//��ǰ�׶ε��깤ʱ��
	int BS[kmax+1];//����ͬ�׶εĿ�ʼʱ��
	int MID[kmax+1];//��ÿ���׶����ĸ�������
};

#endif // !structs_H
