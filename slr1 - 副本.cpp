#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include "lex.h"
#include "table.h"
using namespace std;

map<string,pair<int,int> > action[200]; //action
map<string,int> gt[200]; //goto

vector<int> dimlen;
table globaltab;
table* ntab;

string sym[1010];
string place[1010],code[1010],prop[1010],tc[1010],fc[1010];
string ter[30]={";","[","]","(",")","{","}","d","int","float","void",",","i","=","if","else","while","return","+","*","r","#"};
string var[20]={"P","C","V","D","T","I","S","F","E","G","B"};
string inputsym[10100];
string inputpro[10100];
string gra[40],temp[101];
string l,t,mcode;
int i,j,sp,ip,pos,pt,kt,inputlength,flag,state[1010],dime[1010][50],varnuminfunc[1010],labelnum=0,varnum=0,vartype=0; //ip输入串指针,sp栈指针 

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
		int ff=0;
		if (num<0)
		{
			ff=1;
			num=-num;
		}
		while(num>0)
		{
			ans=(char)(num%10+48)+ans;
			num/=10;
		}
		if (ff) ans="-"+ans;
	}
	return ans;
}

int getplace(string k)
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
	return cal;
}

void inittable()
{
	gra[0] = "A->P";
	gra[1] = "P->CV";
	gra[2] = "C->";
	gra[3] = "C->CD;";
	gra[4] = "V->S";
	gra[5] = "V->VS";
	gra[6] = "D->Td";
	gra[7] = "D->Td[I]";
	gra[8] = "D->Td(C){CV}";
	gra[9] = "T->n";
	gra[10] = "T->l";
	gra[11] = "T->v";
	gra[12] = "I->i";
	gra[13] = "I->I,i";
	gra[14] = "S->d=E;";
	gra[15] = "S->f(B)S";
	gra[16] = "S->f(B)SeS";
	gra[17] = "S->w(B)S";
	gra[18] = "S->tE;";
	gra[19] = "S->{V}";
	gra[20] = "S->d(F);";
	gra[21] = "F->E";
	gra[22] = "F->F,E";
	gra[23] = "E->E+G";
	gra[24] = "E->G";
	gra[25] = "G->i";
	gra[26] = "G->d";
	gra[27] = "G->d[F]";
	gra[28] = "G->d(F)";
	gra[29] = "G->G*G";
	gra[30] = "B->ErE";
	gra[31] = "B->E";
	string ch;
	freopen("action.txt","r",stdin);
	for(int i=0;i<=70;++i)
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
	for(int i=0;i<=70;++i)
	{
		for(int j=0;j<=10;++j)
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
	cout << "  ";
	for(int i=0;i<=21;++i) cout << ter[i] << " ";
	cout << endl;
	for(int i=0;i<=70;++i)
	{
		cout << i << ":";
		for(int j=0;j<=21;++j)
		{
			cout << action[i][ter[j]].first << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "  ";
	for(int i=0;i<=10;++i) cout << var[i] << " ";
	cout << endl;
	for(int i=0;i<=70;++i)
	{
		cout << i << ":";
		for(int j=0;j<=10;++j)
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
	return "L"+ans;
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

void printstack()
{
	for(int i=0;i<=sp;++i) cout << state[i] << " ";
	cout << endl;
	for(int i=0;i<=sp;++i) cout << sym[i] << " ";
	cout << endl;
	for(int i=0;i<=sp;++i) cout << code[i] << " ";
	cout << endl;
	cout << endl;
	return;
}

void printcode(string code)
{
	freopen("code.out","w",stdout);
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

int main()
{
	dolex();
	inittable();
	//printtable();
	inputlength=init();
	memset(state,0,sizeof(state));
	state[0]=0;
	sym[0]="#";
	sp=0;
	ip=0;
	flag=0;
	//0:accept 1:push 2:reduce 3:error
	string ch,tpro;
	pair<int,int> act; //action+state
	while(ip<inputlength)
	{
		//printstack();
		ch=inputsym[ip];
		tpro=inputpro[ip];
		act=action[state[sp]][ch];
		//cout << "action: " << state[sp] << " " << ch << " " << act.first << " " << act.second << endl;
		if (act.first==3)
		{
			cout << "error\n";
			break;
		}
		else if(!act.first)
		{
			cout << "done\n";
			flag=1;
			printcode(code[1]);
			break;
		}
		else if (act.first==1)
		{
			sp++;
			state[sp]=act.second;
			sym[sp]=ch;
			prop[sp]=tpro; //数据，变量名，符号 
			place[sp]=code[sp]="";
			ip++;
			if (state[sp]==40)
			{
				if (prop[sp-2]=="void") globaltab.regis(prop[sp-1],0); else globaltab.regis(prop[sp-1],1);
				//cout << "function " << prop[sp-2] << " " << prop[sp-1] << " registered\n";
				vartype=1;
			}
			else if(state[sp]==66)
			{
				vartype=0;
			}
		}
		else	
		{
			switch(act.second)
			{
				case(0):
				{
					//A->P
					sym[sp]="A";
					break;
				}
				case(1):
				{
					//P->CV
					sym[sp-1]="P";
					code[sp-1]=code[sp-1]+"|"+code[sp];
					sp--;
					break;
				}
				case(2):
				{
					//C->e
					sp++;
					sym[sp]="C";
					code[sp]="";
					break;
				}
				case(3):
				{
					//C->CD;
					sym[sp-2]="C";
					code[sp-2]=code[sp-2]+"|"+code[sp-1];
					sp-=2;
					break;
				}
				case(4):
				{
					//V->S
					sym[sp]="V";
				    break;
				}
				case(5):
				{
					//V->VS
					sym[sp-1]="V";
					code[sp-1]=code[sp-1]+"|"+code[sp];
					sp-=1;
					break;
				}
				case(6):
				{
					//D->Td
					//cout << prop[sp] << " type= " << vartype << endl;
					if (prop[sp-1]=="int") globaltab.regis(prop[sp],0,vartype); else globaltab.regis(prop[sp],1,vartype);
					sym[sp-1]="D";
					sp--;
					break;
				}
				case(7):
				{
					//D->Td[I]
					dimlen.clear();
					for(int i=1;i<=dime[sp-1][0];++i) dimlen.push_back(dime[sp-1][i]);
					//cout << prop[sp-3] << " type= " << vartype << endl;
					if(prop[sp-4]=="int") globaltab.regis(prop[sp-3],0,vartype,dimlen); else globaltab.regis(prop[sp-3],1,vartype,dimlen);
					sym[sp-4]="D";
					sp-=4;
					break;
				}
				case(8):
				{
					//D->Td(C){CV}
					pos=globaltab.look(prop[sp-7]);
					pt=globaltab.nametab[pos].ref;
					kt=globaltab.btab[pt].lastpar;
					mcode="";
					while(kt!=-1)
					{
						mcode+="|pop "+globaltab.nametab[kt].name;
						kt=globaltab.nametab[kt].link;
					}
					sym[sp-8]="D";
					code[sp-8]=prop[sp-7]+" proc|"+mcode+"|"+code[sp-1]+"|"+prop[sp-7]+" endp";
					globaltab.end();
					//cout << "function " << prop[sp-7] << " end\n";
					sp-=8;
				    break;
				}
				case(9):
				{
					//T->int
					sym[sp]="T";
					prop[sp]="int";
					break;
				}
				case(10):
				{
					//T->float
					sym[sp]="T";
					prop[sp]="float";
					break;
				}
				case(11):
				{
					//T->void
					sym[sp]="T";
					prop[sp]="void";
					break;
				}
				case(12):
				{
					//I->i
					sym[sp]="I";
					dime[sp][0]=1;
					dime[sp][1]=stringtoint(prop[sp]);
				    break;
				}
				case(13):
				{
					//I->I,i
					dime[sp-2][0]++;
					dime[sp-2][dime[sp-2][0]]=stringtoint(prop[sp]);
					sp-=2;
					break;
				}
				case(14):
				{
					//S->d=E;
					if (globaltab.look(prop[sp-3])==-1)
					{
						cout << "varname error\n";
						flag=0;
						break; 
					}
					sym[sp-3]="S";
					code[sp-3]=code[sp-1]+"|"+prop[sp-3]+"="+place[sp-1];
					sp-=3;
					break;
				}
				case(15):
				{
					//S->if(B)S
					sym[sp-4]="S";
					code[sp-4]=code[sp-2]+"|"+tc[sp-2]+":|"+code[sp]+"|"+fc[sp-2]+":";
					sp-=4;
					break;
				} 
				case(16):
				{
					//S->if(B)SelseS
					sym[sp-6]="S";
					l=getnewlabel();
					code[sp-6]=code[sp-4]+"|"+tc[sp-4]+":|"+code[sp-2]+"|jmp "+l+"|"+fc[sp-4]+":|"+code[sp]+"|"+l;
					sp-=6;
				    break;
				} 
				case(17):
				{ 
					//S->while(B)S
					sym[sp-4]="S";
					l=getnewlabel();
					code[sp-4]=l+":|"+code[sp-2]+"|"+tc[sp-2]+":|"+code[sp]+"|jmp "+l+"|"+fc[sp-2]+":";
					sp-=4;
					break;
				} 
				case(18):
				{
					//S->returnE; 
					sym[sp-2]="S";
					code[sp-2]=code[sp-2]+"|push "+place[sp-1];
					sp-=2;
					break;
				} 
				case(19):
				{ 
					//S->{V}
					sym[sp-2]="S";
					code[sp-2]=code[sp-1];
					sp-=2;
					break;
				} 
				case(20):
				{ 
					//S->d(F); (仅调用过程)
					pos=globaltab.look(prop[sp-4]);
					if (pos==-1)
					{
						cout << "function name error\n";
						flag=0;
						break; 
					}
					int number=getplace(place[sp-2]);
					mcode="";
					for(int i=1;i<=number;++i)
					{
						mcode+="|push "+temp[i];
					}
					sym[sp-4]="S";
					code[sp-4]=code[sp-2]+"|"+mcode+"|call "+prop[sp-4];
					//如果有返回值要弹出
					pt=globaltab.nametab[pos].type;
					if (pt)
					{
						t=getnewvar();
						code[sp-4]+="|pop "+t;
					}
					sp-=4; 
					break;
				} 
				case(21):
				{
					//F->E
					sym[sp]="F"; 
				    break;
				} 
				case(22):
				{
					//F->F,E 
					sym[sp-2]="F";
					code[sp-2]=code[sp-2]+"|"+code[sp];
					place[sp-2]=place[sp-2]+"|"+place[sp];
					sp-=2;
					break;
				} 
				case(23):
				{ 
					//E->E+G
					t=getnewvar();
					code[sp-2]=code[sp-2]+"|"+code[sp]+"|"+t+"="+place[sp-2]+"+"+place[sp];
					place[sp-2]=t;
					sp-=2;
					break;
				} 
				case(24):
				{ 
					//E->G
					sym[sp]="E";
					break;
				} 
				case(25):
				{ 
					//G->i
					sym[sp]="G";
					t=getnewvar();
					code[sp]=t+"="+prop[sp];
					place[sp]=t;
					break;
				} 
				case(26):
				{ 
					//G->d
					sym[sp]="G";
					code[sp]="";
					place[sp]=prop[sp];
					break;
				} 
				case(27):
				{
					//G->d[F]
					sym[sp-3]="G";
					string t1;
					pos=globaltab.look(prop[sp-3]);
					pt=globaltab.nametab[pos].ref;
					dime[sp-1][0]=globaltab.atab[pt].v.size();
					int number=getplace(place[sp-1]);
					if (num!=dime[sp-1][0])
					{
						cout << "getplace error\n";
						break;
					}
					t=getnewvar();
					mcode=t+"="+temp[1];
					for(int i=1;i<=dime[sp-1][0];++i)
					{
						dime[sp-1][i]=globaltab.atab[pt].v[i-1];
					}
					for(int i=2;i<=dime[sp-1][0];++i)
					{
						t1=getnewvar();
						mcode+="|"+t1+"="+t+"*"+inttostring(dime[sp-1][i]);
						t=getnewvar();
						mcode+="|"+t+"="+t1+"+"+temp[i];
					}
					t1=getnewvar();
					mcode+="|"+t1+"="+prop[sp-3]+"["+t+"]";
					place[sp-3]=t1;
					code[sp-3]=code[sp-1]+"|"+mcode;
					sp-=3;
				    break;
				}
				case(28):
				{
					//G->d(F) (用于赋值)
					pos=globaltab.look(prop[sp-3]);
					pt=globaltab.nametab[pos].type;
					if (!pt)
					{
						cout << "function type error\n";
						break;
					}
					else if(pos==-1)
					{
						cout << "function name error\n";
						break;
					}
					if (globaltab.look(prop[sp-3])==-1)
					{
						cout << "var name error\n";
						break; 
					}
					t=getnewvar();
					mcode=code[sp-1];
					int number=getplace(place[sp-1]);
					for(int i=1;i<=number;++i) mcode+="|push "+temp[i];
					mcode+="|call "+prop[sp-3];
					mcode+="|pop "+t;
					sym[sp-3]="G";
					place[sp-3]=t;
					code[sp-3]=mcode;
					sp-=3;
					break;
				}
				case(29):
				{ 
					//G->G*G
					t=getnewvar();
					code[sp-2]+="|"+code[sp]+"|"+t+"="+place[sp-2]+"*"+place[sp];
					place[sp-2]=t;
					sp-=2;
					break;
				} 
				case(30):
				{ 
					//B->ErE
					sym[sp-2]="B";
					tc[sp-2]=getnewlabel();
					fc[sp-2]=getnewlabel();
					code[sp-2]=code[sp-2]+"|"+code[sp]+"|"+"if "+place[sp-2]+prop[sp-1]+place[sp]+" then jmp "+tc[sp-2]+" else jmp "+fc[sp-2]; 
				    sp-=2;
					break;
				} 
				case(31):
				{ 
					//B->E
					sym[sp]="B";
					tc[sp]=getnewlabel();
					fc[sp]=getnewlabel();
					code[sp]=code[sp]+"|"+"if "+place[sp]+"!=0 then jmp "+tc[sp]+" else jmp "+fc[sp]; 
					break;
				} 
			}
			state[sp]=gt[state[sp-1]][sym[sp]];
			//if(state[sp]==54 && inputsym[ip]==")") vartype=0;
			if (state[sp]==-1)
			{
				cout << "state error\n";
				break;
			}
		}
	}
	return 0;
}
