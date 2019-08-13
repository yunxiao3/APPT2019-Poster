#include<cstdio>
#include<iostream>
#include<algorithm>
#include<queue>
#include<map>
#include<vector>
#include<set>
#include<string>
#include<cmath>
#include<cstring>
#include<ctime>
#define ll long long

using namespace std;
const int MAX1 = 2e5 + 5;

double x1 = 2e10 + 5;

//将单精度浮点数转换为二进制---------分析原理
string FloatToByBinaryByAlgo1(float num)
{
	//	(-1) s表示符号位，当s = 0，V为正数；当s = 1，V为负数。  
	//	（2）M表示有效数字，大于等于1，小于2。
	//	（3）2^E表示指数位。
	//对于单精度 符号位 1 指数为8    有效数字 23
	string sign = (num < 0) ? "1" : "0";
	num = abs(num);
	//将float分解成正数部分和小数部分
	int Interger = int(num);
	float frac = num - Interger;
	//将其转换为分别转换为二进制的数字
	string IntStr("");
	string FracStr("");
	//转换正数部分
	while (Interger)
	{
		IntStr += (Interger % 2 + '0');
		Interger /= 2;
	}
	//翻转字符串
	int i = 0;
	int j = IntStr.size() - 1;
	while (i < j)
	{
		swap(IntStr[i], IntStr[j]);
		++i;
		--j;
	}
	string EStr(8, '0');             //指数域
	string MStr("");				//小数域
	//分解小数部分----由于小部分，可能会出现分解不完全的情况，并且float中仅有23位，所以仅仅循环23次------但是对于仅有小数的部分，还要截断一部分，所以需要补充
	for (int i = 1; i < 24+23; ++i)
	{
		if (frac == 0.0)       //如果等于0，那么退出循环
		{
			break;
		}
		float temp = pow(2, -i);
		if (frac >= temp)
		{
			FracStr += '1';
			frac -= temp;
		}
		else
		{
			FracStr += '0';
		}
	}
	if (num>=1.0)
	{
		//准备工作已经完成了，开始进行实际的工作 
		//偏置 BIAS=pow(2,8-1)-1=127;
		int Offset = IntStr.size() - 1 + 127;	
		int Index = 7;
		//指数域
		while (Offset)
		{
			EStr[Index] = Offset % 2 + '0';
			Offset /= 2;
			--Index;
		}
		//那么小数域为;
		MStr = IntStr.substr(1) + FracStr;
	}
	else
	{
		//找到小数的第一个1为位置
		int Index = 0;
		for (int i = 0; i < FracStr.size();++i)
		{
			if (FracStr[i]=='1')
			{
				Index = i + 1;
				break;
			}
		}
		unsigned char Offset = Index?(-Index + 127):0;//如果小数部分没有1的话，那么就是X.0 
		int temp = 7;
		//指数域
		while (Offset)
		{
			EStr[temp] = Offset % 2 + '0';
			Offset /= 2;
			--temp;
		}
		//那么小数域为;
		MStr = FracStr.substr(Index);
	}
	//对字符进行填充
	if (MStr.size() > 23)
	{
		MStr = MStr.substr(0, 23);
	}
	else
	{
		MStr.resize(23);
		for (int i = 0; i < 23; ++i)
		{
			if (MStr[i] == '\0')
			{
				MStr[i] = '0';
			}
		}
	}
	return sign + EStr + MStr;
}


