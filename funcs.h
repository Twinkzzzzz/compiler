#ifndef FUNCS_H
#define FUNCS_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <string.h>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>

int stringtoint(string str)
{
	int ans=0;
	char ch;
	for(int i=0;i<str.length();++i)
	{
		ch=str[i];
		ans=ans*10+((int)(ch)-48);
	}
	return ans;
}

string inttostring(int num)
{
	string ans="";
	if(!num)
	{
		ans="0";
		return ans;
	}
	else
	{
		if (num<0)
		{
			ans="-";
			num=-num;
		}
		while(num>0)
		{
			ans=(char)(num%10+48)+ans;
			num/=10;
		}
	}
	return ans;
}

void getplace(string k,int dims)
{
	char c;
	string mt="";
	int cal=0;
	for(int i=0;i<k.length();++i)
	{
		c=k[i];
		if (c=='|')
		{
			if(mt!="")
			{
				cal++;
				temp[cal]=mt;
				mt="";
			}
		}
		else
		{
			mt+=c;
		}
	}
	if(mt!="")
	{
		cal++;
		temp[cal]=mt;
	}
	if(cal!=dims) cout << "getplace error\n";
	return;
}

void inittable()
{
	gra[0] = "A->P";
	gra[1] = "P->CV";
	gra[2] = "C->";
	gra[3] = "C->CD;";
	gra[4] = "V->S";
	gra[5] = "V->V;S";
	gra[6] = "D->Td";
	gra[7] = "D->Td[I]";
	gra[8] = "D->Td(C){CV}";
	gra[9] = "T->n";
	gra[10] = "T->l";
	gra[11] = "T->v";
	gra[12] = "I->i";
	gra[13] = "I->I,i";
	gra[14] = "S->d=E";
	gra[15] = "S->f(B)S";
	gra[16] = "S->f(B)SeS";
	gra[17] = "S->w(B)S";
	gra[18] = "S->tE";
	gra[19] = "S->{V}";
	gra[20] = "S->d(F)";
	gra[21] = "F->E";
	gra[22] = "F->F,E";
	gra[23] = "E->i";
	gra[24] = "E->d";
	gra[25] = "E->d[F]";
	gra[26] = "E->d(F)";
	gra[27] = "E->E+E";
	gra[28] = "E->E*E";
	gra[29] = "B->ErE";
	gra[30] = "B->E";
	string ch;
	freopen("action.txt","r",stdin);
	for(int i=0;i<=67;++i)
	{
		for(int j=0;j<=21;++j)
		{
			cin >> ch;
			if (ch=="*")
			{
				action[i][ter[j]]=make_pair(3,0);
			}
			else if(ch=="acc")
			{
				action[i][ter[j]]=make_pair(0,0);
			}
			else if(ch[0]=='r')
			{
				action[i][ter[j]]=make_pair(2,stringtoint(ch.substr(1)));
			}
			else
			{
				action[i][ter[j]]=make_pair(1,stringtoint(ch.substr(1)));
			}
		}
	}
	freopen("goto.txt","r",stdin);
	for(int i=0;i<=67;++i)
	{
		for(int j=0;j<=9;++j)
		{
			cin >> ch;
			if (ch=="*")
			{
				gt[i][var[j]]=-1;
			}
			else
			{
				gt[i][var[j]]=stringtoint(ch);
			}
		}
	}
	return;
}

void printtable()
{
	for(int i=0;i<=67;++i)
	{
		for(int j=0;j<=21;++j)
		{
			cout << action[i][ter[j]].first << " ";
		}
		cout << endl;
	}
	cout << endl;
	for(int i=0;i<=67;++i)
	{
		for(int j=0;j<=9;++j)
		{
			cout << gt[i][var[j]] << " ";
		}
		cout << endl;
	}
	return;
}

int init()
{
	int k=0;
	string ch;
	freopen("mstr.in","r",stdin);
	while(cin >> ch)
	{
		inputsym[k]=ch;
		cin >> ch;
		inputpro[k]=ch;
		k++;
	}
	return k;
}

string getnewlabel()
{
	int p,t=labelnum;
	string ans="";
	while(t)
	{
		p=t%10;
		t/=10;
		ans=(char)(p+48)+ans;
	}
	if (!labelnum) ans="0";
	labelnum++;
	return "label"+ans;
}

string getnewvar()
{
	int p,t=varnum;
	string ans="";
	while(t)
	{
		p=t%10;
		t/=10;
		ans=(char)(p+48)+ans;
	}
	if (!varnum) ans="0";
	varnum++;
	return "T"+ans;
}

int lookup(string ch)
{
	
}

void printstack()
{
	//for(int i=0;i<=sp;++i) cout << state[i] << " ";
	//cout << endl;
	for(int i=0;i<=sp;++i) cout << state[i] << " ";
	cout << endl;
	for(int i=0;i<=sp;++i) cout << sym[i] << " ";
	cout << endl;
	for(int i=0;i<=sp;++i) cout << code[i] << " ";
	cout << endl;
	return;
}

void printcode(string code)
{
	int flag=0;
	char ch;
	for(int i=0;i<code.length();++i)
	{
		ch=code[i];
		if (ch=='|')
		{
			if(flag)
			{
				cout << endl;
				flag=0;
			}
		}
		else
		{
			cout << ch;
			flag=1;
		}
	}
	cout << endl;
	return;
}

#endif
