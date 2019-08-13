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
const int MAX = 2e5 + 5;

double x = 2e10 + 5;

//将单精度浮点数转换为二进制---------分析原理
string FloatToByBinaryByAlgo(float num)
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



//将单精度浮点数转换为二进制---------分析原理
//将二进制的数据，转换为float浮点数
float BinaryTofloat(string str)
{
	if (str.size()!=32)
	{
		return NAN;
	}
	//	(-1) s表示符号位，当s = 0，V为正数；当s = 1，V为负数。  
	//	（2）M表示有效数字，大于等于1，小于2。
	//	（3）2^E表示指数位。
	//对于单精度 符号位 1 指数为8    有效数字 23
	//  （1）E不全为0或不全为1。这时，浮点数就采用上面的规则表示，即指数E的计算值减去127（或1023），得到真实值，再将有效数字M前加上第一位的1。
	//	（2）E全为0。这时，浮点数的指数E等于1 - 127（或者1 - 1023），有效数字M不再加上第一位的1，而是还原为0.xxxxxx的小数。这样做是为了表示±0，以及接近于0的很小的数字。
	//	（3）E全为1。这时，如果有效数字M全为0，表示±无穷大（正负取决于符号位s）；如果有效数字M不全为0，表示这个数不是一个数（NaN）。
	int sign = (str[0] == '0') ? 1 : -1;
	//分解有效数数字，因为，对于E来说比较复杂，所以我们最后在分解
	float frac = 0.0;
	for (int i = 9; i < str.size();++i)
	{
		frac += pow(2, -i + 8)*(str[i]-'0');
	}
	//分解指数位
	//假如全为0的情况
	bool mark = true;
	for (int i = 1; i < 9;++i)
	{
		if (str[i]=='1')
		{
			mark = false;
			break;
		}
	}
	if (mark)
	{
		return sign*frac*pow(2, 1 - 127);
	}
	//假如全为1的情况
	mark = true;
	for (int i = 1; i < 9;++i)
	{
		if (str[i]=='0')
		{
			mark = false;
			break;
		}
	}
	if (mark)
	{
		if (0.0==frac) //如果M全为0 
		{
			if (1==sign)//那么如果符号为1 的话，返回负无穷
			{
				return -INFINITY;
			}
			else //如果符号为0的话，返回正无穷
			{
				return INFINITY;
			}
		}
		else         //不然的话，返回NAN 不是一个数字
		{
			return NAN;
		}
	}
	//最后一种情况，指数不全为0或者1
	unsigned char p = 0;
	for (int i = 8,Index=0; i >= 1;--i,++Index)
	{
		p += pow(2, Index)*(str[i] - '0');
	}
	float E = pow(2, p - 127);
	return sign*E*(1 + frac);
}


int main(int argc, char const *argv[]){
    
   cout << FloatToByBinaryByAlgo(1.5);
   cout << BinaryTofloat("00111110011100010100000100100000");

    return 0;
}