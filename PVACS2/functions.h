#pragma once
#ifndef functions_H
#define functions_H


#include"structs.h"
#include<algorithm>
#include<iostream>
#include<vector>
#include<fstream>
using namespace std;
int max(int a, int b);
int min(int a, int b, int c);
bool cmp(const job &J1, const job &J2);
bool cmp2(const batch &b1, const batch &b2);
bool cmp3(const batch &b1, const batch &b2);
bool cmp4(const batch &b1, const batch &b2);
void initJob(vector<job> &J);
void initBatch(batch &B, int id);
void getPbk(vector<job> J, vector<batch> &B);
void BFLPT(vector<job> J/*, vector<batch> &B*/);
void displayBatch(/*vector<batch> B*/);

void initPh();
void computeSIb(/*vector<batch> &B*/);
void initMachine();//初始化机器的信息

#endif // !functions_H