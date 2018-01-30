#pragma once
#ifndef defines_H
#define defines_H

const int JobNum = 40;//工件的数量
const int C = 10;//批的最大容量
const int kmax = 2;//阶段数
const int Tmax = 150;//迭代的次数
const int antNum = 100;// 蚂蚁的数量
const float q0 = 0.68;
const int alpha = 1;
const int beta = 3;
const float pl = 0.1;//局部挥发系数
const float pg = 0.2;//全局挥发系数
const float taoMin = 1;//信息素最小值
const float taoMax = 100;//信息素最大值
const float tao = 1;//初始信息素

#endif // !defines_H
