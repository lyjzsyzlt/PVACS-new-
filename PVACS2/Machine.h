#pragma once
#ifndef Machine_H
#define Machine_H


#include"defines.h"
#include"structs.h"
#include<vector>
#include<list>

class Machine
{
public:
	int V;//�����׶β�ͬ�������ٶ�
	int PW;//���������ڲ�ͬ�׶��µĹ�������
	int avt;//��k�׶Σ�ÿ�������ĵ�ǰ����ʱ��
	list<batch> List;//��¼�����ϼӹ�����
	Machine();
	~Machine();
};


#endif // !Machine_H