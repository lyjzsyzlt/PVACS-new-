#pragma once
#ifndef ant_H
#define ant_H

#include"functions.h"
#include"structs.h"
#include"Machine.h"
class ant
{
private:
	// 用户偏好向量
	float Vmk;
	float Vepc;
	int Cmax;//记录最大makespan
	int EPC;//记录最终的EPC
	int newEPC;//记录，Right_Shift之后的EPC
	ofstream out;
public:
	int id;
	vector<int> sol;//存放解
	vector<batch> tempB;
	ant()
	{
		out.open("LS_Process.txt", ios_base::out | ios_base::app);
	};
	void generateV();//产生偏好向量的两个值
	void createBatchSeq();// 构造解（批序列）
	void LS();// LS算法
	int getEPC1(int t, int detaT, int k, int i);// 计算EPC，t为下界，detaT为整个过程的时间,机器为第k阶段的第l台机器
	int getEPC2(int t, int detaT);//计算机器的待机电费
	void Right_Shift();//Right_Shift改进
	void localUpdate(); 
	void updateNDS();
	~ant();
};
#endif // !ant_H