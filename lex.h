#ifndef LEX_H
#define LEX_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;

enum myType {key, id, num, sep, ope, fil};
string KEYWORD[7] = {"if", "else", "while", "return", "int", "float", "void"}; //�ؼ���0
char SEPARATER[8] = {';', ',', '{', '}', '[', ']', '(', ')'}; //�ָ���1
char FILTER[4] = {' ', '\t', '\r', '\n'};     //���˷�3
static int pre_type = sep; //ǰһ��ʶ��ʵ�״̬
static string pre_op = ""; //��¼ǰ��ķ��ţ�����д�������Ǹ���
static bool skip_char = 0; //�Ƿ���������+
static int errorflag = 0;

//�����ж�{{{
/**�ж��Ƿ�Ϊ�ؼ���**/
bool IsKeyword(string word) {
	for (int i = 0; i < 7; i++) {
		if (KEYWORD[i] == word) {
			return true;
		}
	}
	return false;
}
/**�ж��Ƿ�Ϊ���˷�**/
bool IsFilter(char ch) {
	for (int i = 0; i < 4; i++) {
		if (FILTER[i] == ch) {
			return true;
		}
	}
	return false;
}

/**�ж��Ƿ�ΪСд��ĸ**/
bool IsLowLetter(char ch) {
	if (ch >= 'a' && ch <= 'z')
		return true;
	return false;
}
/**�ж��Ƿ�Ϊ����**/
bool IsDigit(char ch) {
	if (ch >= '0' && ch <= '9')
		return true;
	return false;
}/*}}}*/

namespace Print {/*{{{*/
	char filename1[40]="CON";
	char filename2[40]="mstr.in";
	string output;//��¼�滻���봮��������
	string table;
	void init() {
		/*cout << "������ �滻���봮 ������ļ�����ͬĿ¼�²���д·���������������������CON��:";
		cin >> filename1;*/
		/*cout << "������ �������Ա�� ������ļ�����ͬĿ¼�²���д·���������������������CON��:";
		cin >> filename2;*/
		output = "";
		table = "";
		cout << endl;
		freopen(filename2, "w", stdout); //������Ա��
	}
	void output_str() {
		freopen(filename1, "w", stdout);
		cout << output << endl;
	}
	void print_table(string symbol,string value){
		cout<<symbol<<"\t"<<value<<endl;
	}
	void update(int t, string s = "", bool skp = 0) {
		::pre_type = t;
		::pre_op = s;
		::skip_char = skp;
	}
	void Keyword(string str) {
		//cout << "keyword\t" << str << endl;
		table=table+"keyword\t"+str;
		print_table(str,"#");
		output = output + str + " ";

		update(key);
	}
	void Id(string str) {
		/*cout << "ID\t"
		     << str << endl;*/
		table=table+"ID\t"+str;
		print_table("d",str);
		output = output + "d" + " ";

		update(id);
	}
	void Value(string op, string str, bool is_int) {
		if (op=="+"||op!="-")
			op="";
		if (is_int) {
			/*cout << "INT\t"
			     << op << str << endl;*/
			table=table+"INT\t"+op+str;
			print_table("i",op+str);
			output = output + "i" + " ";
		} else {
			/*cout << "FLOAT\t"
			     << op << str << endl;*/
			table=table+"FLOAT\t"+op+str;
			print_table("f",op+str);
			output = output + "f" + " ";
		}
		update(num);
	}
	void Plus() {
		/*cout << "ADD\t"
		     << "+" << endl;*/
		table=table+"ADD\t+";
		print_table("+","#");

		output = output + "+" + " ";
		update(ope,"+");
	}
	void PlusEqu() {
		/*cout << "ADC\t"
		     << "+=" << endl;*/
		table=table+"ADC\t+=";
		print_table("+=","#");

		output = output + "+=" + " ";
		update(ope,"+=");
	}
	void Positive() {
		//��������Ӻţ�pre_type����
		::pre_op = ""; //���Ų����
		::skip_char = 1;
	}
	void Negative() {
		//����������ţ�pre_type����
		::pre_op = "-";
		::skip_char = 1;
	}
	void Mul() {
		/*cout << "MUL\t"
		     << "*" << endl;*/
		table=table+"MUL\t*";
		print_table("*","#");

		output = output + "*" + " ";
		update(ope,"*");
	}
	void Smaller() {
		//cout << "ROP\t<" << endl;
		table=table+"ROP\t<";
		print_table("r","<");
		output = output + "r ";
		update(ope,"<");
	}
	void Equ2() {
		//cout << "ROP\t==" << endl;
		table=table+"ROP\t==";
		print_table("r","==");

		output = output + "r ";
		update(ope,"==");
	}
	void Equ1() {
		/*cout << "ASG\t"
		     << "=" << endl;*/
		table=table+"ASG\t=";
		print_table("=","#");

		output = output + "=" + " ";
		update(ope,"=");
	}
	void Nequ() {
		//cout << "ROP\t!=" << endl;
		table=table+"ROP\t!=";
		print_table("r","!=");

		output = output + "r " + " ";
		update(ope,"!=");
	}
	void Semi() {
		/*cout << "SEMI\t"
		     << ";" << endl;*/
		table=table+"SEMI\t;";
		print_table(";","#");

		output = output + ";" + " ";
		update(sep,";");
	}
	void Cma() {
		/*cout << "CMA\t"
		     << "," << endl;*/
		table=table+"CMA\t,";
		print_table(",","#");

		output = output + "," + " ";
		update(sep,",");
	}
	void Par(int which) {
		if (which) {
			/*cout << "RPAR\t"
			     << ")" << endl;*/
			table=table+"RPAR\t)";
			print_table(")","#");

			output = output + ")" + " ";
			update(sep,")");
		} else {
			/*cout << "LPAR\t"
			     << "(" << endl;*/
			table=table+"LPAR\t(";
			print_table("(","#");

			output = output + "(" + " ";
			update(sep,"(");
		}
	}
	void Bk(int which) {
		if (which) {
			//cout << "RBK\t]" << endl;
			table=table+"RBK\t]";
			print_table("]","#");

			output = output + "]" + " ";
			update(sep,"]");
		} else {
			//cout << "LBK\t[" << endl;
			table=table+"LBK\t[";
			print_table("[","#");

			output = output + "[" + " ";
			update(sep,"[");
		}
	}
	void Br(int which) {
		if (which) {
			//cout << "RBR\t}" << endl;
			table=table+"RBR\t}";
			print_table("}","#");

			output = output + "}" + " ";
			update(sep,"}");
		} else {
			//cout << "LBR\t{" << endl;
			table=table+"LBR\t{";
			print_table("{","#");

			output = output + "{" + " ";
			update(sep,"{");
		}
	}
}/*}}}*/

/**�ʷ�����**//*{{{*/
void error(char ch) {
	freopen("CON", "w", stdout);
	cout << "�����޷�ʶ����ַ�: " << ch << endl;
	errorflag = 1;
}
void analyse(FILE *fpin) {
	char ch = ' ';
	string arr = "";
	while ((ch = fgetc(fpin)) != EOF && !errorflag) {
		arr = "";
		if (IsFilter(ch)) {}     //�ж��Ƿ�Ϊ���˷�
		else if (IsLowLetter(ch)) {  //�ж��Ƿ�Ϊ�ؼ���
			while (IsLowLetter(ch)) {
				arr += ch;
				ch = fgetc(fpin);
			}
			//arr=ץ�����ĵ���
			if (IsKeyword(arr)) {
				Print::Keyword(arr);
			} else {
				while (IsDigit(ch)) {
					arr += ch;
					ch = fgetc(fpin);
				}
				Print::Id(arr);
			}
			fseek(fpin, -1L, SEEK_CUR); //���ˣ��µ�whileҪ���¶�
		} else if (IsDigit(ch)) {  //�ж��Ƿ�Ϊ����
			bool is_int = 1;
			while (IsDigit(ch)) {
				arr += ch;
				ch = fgetc(fpin);
			}//int
			if (ch == '.') { //��������
				is_int = 0;
				arr += ch;
				ch = fgetc(fpin);
				while (IsDigit(ch)) {
					arr += ch;
					ch = fgetc(fpin);
				}
			}
			Print::Value(pre_op, arr, is_int);
			fseek(fpin, -1L, SEEK_CUR); //���ˣ��µ�while���¶�
		} else
			switch (ch) {
				case '+': {
					if (pre_type == num || pre_type == id || pre_op == "]" || pre_op == ")") { //˵���������
						Print::Plus();
					} else { //˵�������ֵ�һ���ֻ�+=
						if (fgetc(fpin) == '=') { //˵����+=
							Print::PlusEqu();
						} else {
							Print::Positive();
							fseek(fpin, -1L, SEEK_CUR); //����
						}
					}
					break;
				}
				case '-': {
					Print::Negative();
					break;
				}
				case '*': {
					Print::Mul();
					break;
				}
				case '<': {
					Print::Smaller();
					break;
				}
				case '=': {
					if (fgetc(fpin) == '=') { //˵����'=='
						Print::Equ2();
					} else {
						Print::Equ1();
						fseek(fpin, -1L, SEEK_CUR); //����
					}
					break;
				}
				case '!': {
					if (fgetc(fpin) == '=') { //˵����'!='��û�������Ϸ����
						Print::Nequ();
					} else { //����
						error(ch);
						return;
					}
					break;
				}
				case ';': {
					Print::Semi();
					break;
				}
				case ',': {
					Print::Cma();
					break;
				}
				case '(': {
					Print::Par(0);
					break;
				}
				case ')': {
					Print::Par(1);
					break;
				}
				case '[': {
					Print::Bk(0);
					break;
				}
				case ']': {
					Print::Bk(1);
					break;
				}
				case '{': {
					Print::Br(0);
					break;
				}
				case '}': {
					Print::Br(1);
					break;
				}
				default : {
					error(ch);
					return;
				}
			}
	}

}/*}}}*/

void dolex() {
	char filename[40]="sourcecode.in";
	FILE *fpin;
	/*cout << "������Դ�ļ���������·���ͺ�׺��:";*/
	/*
	while (1) {
		if ((fpin = fopen(filename, "r")) != NULL)
			break;
		else {
			cout << "�ļ�������" << endl;
			cout << "������Դ�ļ���������·���ͺ�׺��:";
		}
		cin>>filename;
	}
	*/
	fpin = fopen(filename, "r");
	Print::init();
	//cout<<"------�ʷ���������------"<<endl;
	analyse(fpin);
	cout<<"#\t#"<<endl;
	//Print::output_str();
	freopen("CON","w",stdout);
	return;
}
#endif
