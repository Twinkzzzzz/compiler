#include<iostream>
#include<string>
#include<cstring>
#include<set>
using namespace std;
struct Item
{
	string item;
	char ac;
	int next;
	int from;
};
struct Newc
{
	set<int> newc;
	set<int> united;
};
string at[200][33];
int follow[11][22];
char en[22] = { ';','[',']','(',')','{','}','d','n','l','v',',','i','=','f','e','w','t','+','*','r','#' };
char nont[11] = { 'P','C','V','D','T','I','S','F','E','G','B' };
set<int> canon[200];
set<int>::iterator it;
set<int> te;
void init()
{
	for (int i = 0; i < 200; i++) for (int j = 0; j < 33; j++) at[i][j] = "*";
	for (int i = 0; i < 11; i++) for (int j = 0; j < 22; j++) follow[i][j] = 0;
	follow[0][21] = 1;
	follow[1][4] = follow[1][5] = follow[1][7] = follow[1][8] = follow[1][9] = follow[1][10] = follow[1][14] = follow[1][16] = follow[1][17] = 1;
	follow[2][5] = follow[2][6] = follow[2][7] = follow[2][14] = follow[2][16] = follow[2][17] = follow[2][21] = 1;
	follow[3][0] = 1;
	follow[4][7] = 1;
	follow[5][2] = follow[5][11] = 1;
	follow[6][5] = follow[6][6] =follow[6][7] =follow[6][14] =follow[6][16] =follow[6][15] =follow[6][17] =follow[6][21] = 1;
	follow[7][2] = follow[7][4] = follow[7][11] = 1;
	follow[8][0] = follow[8][2] = follow[8][4] = follow[8][11] = follow[8][18] = follow[8][20] = 1;
	follow[9][0] = follow[9][2] = follow[9][4] = follow[9][11] = follow[9][18] = follow[9][19] = follow[9][20] = 1;
	follow[10][4] = 1;
	return;
}
bool comp(set<int>b)
{
	if (te.size() != b.size()) return false;
	set<int>::iterator it1, it2;
	for (it1 = te.begin(), it2 = b.begin(); it1 != te.end(); it1++, it2++)
	{
		if (*it1 != *it2) return false;
	}
	return true;
}
void print(string a)
{
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] == 'n')
		{
			cout << "int";
		}
		else if (a[i] == 'l')
		{
			cout << "float";
		}
		else if (a[i] == 'v')
		{
			cout << "void";
		}
		else if (a[i] == 'f')
		{
			cout << "if";
		}
		else if (a[i] == 'e')
		{
			cout << "else";
		}
		else if (a[i] == 'w')
		{
			cout << "while";
		}
		else if (a[i] == 't')
		{
			cout << "return";
		}
		else
		{
			cout << a[i];
		}
	}
	return;
}
int main()
{
	init();
	Item status[200];
	string gra[32];
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
	int k = 0;
	string temp;
	for (int i = 0; i < 32; i++)
	{
		for (int j = 3; j < gra[i].length() + 1; j++)
		{
			temp = gra[i];
			temp.insert(j, ".");
			status[k].item = temp;
			status[k].from = i;
			if (j != status[k].item.length() - 1)
			{
				status[k].ac = status[k].item[j + 1];
				status[k].next = k + 1;
			}
			else
			{
				status[k].ac = '/';
				if (status[k].item[0] == 'A') status[k].next = -1;
				if (status[k].item[0] == 'P') status[k].next = 0;
				if (status[k].item[0] == 'C') status[k].next = 1;
				if (status[k].item[0] == 'V') status[k].next = 2;
				if (status[k].item[0] == 'D') status[k].next = 3;
				if (status[k].item[0] == 'T') status[k].next = 4;
				if (status[k].item[0] == 'I') status[k].next = 5;
				if (status[k].item[0] == 'S') status[k].next = 6;
				if (status[k].item[0] == 'F') status[k].next = 7;
				if (status[k].item[0] == 'E') status[k].next = 8;
				if (status[k].item[0] == 'G') status[k].next = 9;
				if (status[k].item[0] == 'B') status[k].next = 10;
			}
			k++;
		}
	}
	set<int> first[11];
	string fir[11] = { "P->.","C->.","V->.","D->.","T->.","I->.","S->.","F->.","E->.","G->.","B->." };
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < k; j++)
		{
			if (status[j].item[0] == fir[i][0] && status[j].item[1] == fir[i][1] && status[j].item[2] == fir[i][2] && status[j].item[3] == fir[i][3])
			{
				first[i].insert(j);
			}
		}
	}
	for (int i = 0; i < k; i++)
	{
		canon[i].insert(i);
		it = canon[i].begin();
		int j, pos = 1, tem;
		while (1)
		{
			for (j = 3; j < status[*it].item.length(); j++)
			{
				if (status[*it].item[j] == '.') break;
			}
			tem = canon[i].size();
			if (j != status[*it].item.length() - 1)
			{
				if (status[*it].item[j + 1] == 'P') canon[i].insert(first[0].begin(), first[0].end());
				if (status[*it].item[j + 1] == 'C') canon[i].insert(first[1].begin(), first[1].end());
				if (status[*it].item[j + 1] == 'V') canon[i].insert(first[2].begin(), first[2].end());
				if (status[*it].item[j + 1] == 'D') canon[i].insert(first[3].begin(), first[3].end());
				if (status[*it].item[j + 1] == 'T') canon[i].insert(first[4].begin(), first[4].end());
				if (status[*it].item[j + 1] == 'I') canon[i].insert(first[5].begin(), first[5].end());
				if (status[*it].item[j + 1] == 'S') canon[i].insert(first[6].begin(), first[6].end());
				if (status[*it].item[j + 1] == 'F') canon[i].insert(first[7].begin(), first[7].end());
				if (status[*it].item[j + 1] == 'E') canon[i].insert(first[8].begin(), first[8].end());
				if (status[*it].item[j + 1] == 'G') canon[i].insert(first[9].begin(), first[9].end());
				if (status[*it].item[j + 1] == 'B') canon[i].insert(first[10].begin(), first[10].end());
			}
			if (tem != canon[i].size()) it = canon[i].begin();
			else if (pos != canon[i].size())
			{
				it++;
				pos++;
			}
			else break;
		}
	}
	for (int i = 0; i < k; i++)
	{
		cout << i << ": ";
		for (it = canon[i].begin(); it != canon[i].end(); ++it)
		{
			cout << status[*it].item << "|";
		}
		cout << endl;
	}
	Newc nec[200];
	nec[0].newc.insert(canon[0].begin(), canon[0].end());
	nec[0].united.insert(0);
	int x = 0;
	bool judge = false;
	for (int i = 0; i < 200; i++)
	{
		for (int j = 0; j < 22; j++)
		{
			for (it = nec[i].newc.begin(); it != nec[i].newc.end(); ++it)
			{
				if (status[*it].ac == en[j]) te.insert(status[*it].next);
			}
			if (te.size() == 0) continue;
			for (int y = 0; y <= x; y++)
			{
				if (comp(nec[y].united) == true)
				{
					at[i][j] = "s" + to_string(y);
					judge = true;
					break;
				}
			}
			if (!judge)
			{
				x++;
				nec[x].united.insert(te.begin(), te.end());
				for (it = nec[x].united.begin(); it != nec[x].united.end(); it++)
				{
					nec[x].newc.insert(canon[*it].begin(), canon[*it].end());
				}
				at[i][j] = "s" + to_string(x);
			}
			te.clear();
			judge = false;
		}
		for (int j = 0; j < 11; j++)
		{
			for (it = nec[i].newc.begin(); it != nec[i].newc.end(); ++it)
			{
				if (status[*it].ac == nont[j])
				{
					te.insert(status[*it].next);
				}
			}
			if (te.size() == 0) continue;
			for (int y = 0; y <= x; y++)
			{
				if (comp(nec[y].united) == true)
				{
					at[i][j + 22] = to_string(y);
					judge = true;
					break;
				}
			}
			if (!judge)
			{
				x++;
				nec[x].united.insert(te.begin(), te.end());
				for (it = nec[x].united.begin(); it != nec[x].united.end(); it++)
				{
					nec[x].newc.insert(canon[*it].begin(), canon[*it].end());
				}
				at[i][j + 22] = to_string(x);
			}
			te.clear();
			judge = false;
		}
		for (it = nec[i].newc.begin(); it != nec[i].newc.end(); ++it)
		{
			if (status[*it].ac == '/')
			{
				if (status[*it].next == -1) at[i][21] = "acc";
				else
				{
					for (int j = 0; j < 22; j++)
					{
						if (follow[status[*it].next][j] == 1) at[i][j] = "r" + to_string(status[*it].from);
					}
				}
			}
		}
		if (i == x) break;
	}

	for (int i = 0; i <= x; i++)
	{
		cout << i << "	";
		for (int j = 0; j < 33; j++)
		{
			cout << at[i][j] << "	";
			cout << " ";
		}
		cout << endl;
	}
	for (int i = 0; i <= x; i++)
	{
		cout << i << ": ";
		for (it = nec[i].newc.begin(); it != nec[i].newc.end(); ++it)
		{
			print(status[*it].item);
			cout << "|";
		}
		cout << endl;
	}
	return 0;
}
