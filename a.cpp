#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
using namespace std;
enum myType{key,id,num,sep,ope,fil};
string KEYWORD[7]={"if","else","while","return","int","float","void"};//�ؼ���0
char SEPARATER[8]={';',',','{','}','[',']','(',')'};  //�ָ���1
char FILTER[4]={' ','\t','\r','\n'};          //���˷�3
static int pre_type=sep;//ǰһ��ʶ��ʵ�״̬
static string pre_op="";//��¼ǰ��ķ��ţ�����д�������Ǹ���
static bool skip_char=0;//�Ƿ���������+
static int errorflag=0;

//�����ж�{{{
/**�ж��Ƿ�Ϊ�ؼ���**/
bool IsKeyword(string word){
	for(int i=0;i<7;i++){
		if(KEYWORD[i]==word){
			return true;
		}
	}
	return false;
}
/**�ж��Ƿ�Ϊ���˷�**/
bool IsFilter(char ch){
	for(int i=0;i<4;i++){
		if(FILTER[i]==ch){
			return true;
		}
	}
	return false;
}

/**�ж��Ƿ�ΪСд��ĸ**/
bool IsLowLetter(char ch){
	if(ch>='a' && ch<='z') return true;
	return false;
}
/**�ж��Ƿ�Ϊ����**/
bool IsDigit(char ch){
	if(ch>='0' && ch<='9') return true;
	return false;
}/*}}}*/

namespace Print{/*{{{*/
	char filename1[40];
	char filename2[40];
	string output;//��¼�滻���봮��������
	void init(){
		cout<<"������ �滻���봮 ������ļ�����ͬĿ¼�²���д·���������������������CON��:";
		cin>>filename1;
		cout<<"������ �������Ա�� ������ļ�����ͬĿ¼�²���д·���������������������CON��:";
		cin>>filename2;
		output="";
		cout<<endl;
		freopen(filename2,"w",stdout);//������Ա��
	}
	void output_str(){
		freopen(filename1,"w",stdout);
		cout<<output<<endl;
	}
	void update(int t,string s="",bool skp=0){
		::pre_type=t;
		::pre_op=s;
		::skip_char=skp;
	}
	void Keyword(string str){
		cout<<"keyword\t"<<str<<endl;
		output=output+str+" ";

		update(key);
	}
	void Id(string str){
		cout<<"ID\t"
			<<str<<endl;
		output=output+"d"+" ";

		update(id);
	}
	void Value(string op,string str,bool is_int){
		if (is_int){
			cout<<"INT\t"
				<<op<<str<<endl;
			output=output+"i"+" ";
		}
		else{
			cout<<"FLOAT\t"
				<<op<<str<<endl;
			output=output+"f"+" ";
		}
		update(num);
	}
	void Plus(){
		//cout<<"ADD: +"<<endl;
		output=output+"+"+" ";
		update(ope);
	}
	void PlusEqu(){
		//cout<<"AADG: +="<<endl;
		output=output+"+="+" ";
		update(ope);
	}
	void Positive(){
		//��������Ӻţ�pre_type����
		::pre_op="";//���Ų����
		::skip_char=1;
	}
	void Negative(){
		//����������ţ�pre_type����
		::pre_op="-";
		::skip_char=1;
	}
	void Mul(){
		//cout<<"MUL: *"<<endl;
		output=output+"*"+" ";
		update(ope);
	}
	void Smaller(){
		cout<<"ROP\t<"<<endl;
		output=output+"r ";
		update(ope);
	}
	void Equ2(){
		cout<<"ROP\t=="<<endl;
		output=output+"r ";
		update(ope);
	}
	void Equ1(){
		//cout<<"ASG: ="<<endl;
		output=output+"="+" ";
		update(ope);
	}
	void Nequ(){
		cout<<"ROP\t!="<<endl;
		output=output+"r "+" ";
		update(ope);
	}
	void Semi(){
		//cout<<"SEMI: ;"<<endl;
		output=output+";"+" ";
		update(sep);
	}
	void Cma(){
		//cout<<"CMA: ,"<<endl;
		output=output+","+" ";
		update(sep);
	}
	void Par(int which){
		if (which)
			//cout<<"RPAR: )"<<endl;
			output=output+")"+" ";
		else
			//cout<<"LPAR: ("<<endl;
			output=output+"("+" ";
		update(sep);
	}
	void Bk(int which){
		if (which)
			//cout<<"RBK: ]"<<endl;
			output=output+"]"+" ";
		else
			//cout<<"LBK: ["<<endl;
			output=output+"["+" ";
		update(sep);
	}
	void Br(int which){
		if (which)
			//cout<<"RBR: }"<<endl;
			output=output+"}"+" ";
		else
			//cout<<"LBR: {"<<endl;
			output=output+"{"+" ";
		update(sep);
	}
}/*}}}*/

/**�ʷ�����**//*{{{*/
void error(char ch){
	freopen("CON","w",stdout);
	cout<<"�����޷�ʶ����ַ�: "<<ch<<endl;
	errorflag=1;
}
void analyse(FILE * fpin){
	char ch=' ';
	string arr="";
	while((ch=fgetc(fpin))!=EOF&&!errorflag){
		arr="";
		if(IsFilter(ch)){}       //�ж��Ƿ�Ϊ���˷�
		else if(IsLowLetter(ch)){    //�ж��Ƿ�Ϊ�ؼ���
			while(IsLowLetter(ch)){
				arr += ch;
				ch=fgetc(fpin);
			}
			//arr=ץ�����ĵ���
			if(IsKeyword(arr)){
				Print::Keyword(arr);
			}
			else
			{
				while (IsDigit(ch)){
					arr+=ch;
					ch=fgetc(fpin);
				}
				Print::Id(arr);
			}
			fseek(fpin,-1L,SEEK_CUR);//���ˣ��µ�whileҪ���¶�
		}
		else if(IsDigit(ch)){      //�ж��Ƿ�Ϊ����
			bool is_int=1;
			while(IsDigit(ch)){
				arr += ch;
				ch=fgetc(fpin);
			}//int
			if (ch=='.'){//��������
				is_int=0;
				arr+=ch;
				ch=fgetc(fpin);
				while (IsDigit(ch)){
					arr+=ch;
					ch=fgetc(fpin);
				}
			}
			Print::Value(pre_op,arr,is_int);
			fseek(fpin,-1L,SEEK_CUR);//���ˣ��µ�while���¶�
		}
		else switch(ch){
			case '+':
				{
					if (pre_type==num||pre_type==id){//˵���������
						Print::Plus();
					}
					else{//˵�������ֵ�һ���ֻ�+=
						if (fgetc(fpin)=='='){//˵����+=
							Print::PlusEqu();
						}
						else{
							Print::Positive();
							fseek(fpin,-1L,SEEK_CUR);//����
						}
					}
					break;
				}
			case '-':
				{
					Print::Negative();
					break;
				}
			case '*':
				{
					Print::Mul();
					break;
				}
			case '<':
				{
					Print::Smaller();
					break;
				}
			case '=':
				{
					if (fgetc(fpin)=='='){//˵����'=='
						Print::Equ2();
					}
					else{
						Print::Equ1();
						fseek(fpin,-1L,SEEK_CUR);//����
					}
					break;
				}
			case '!':
				{
					if (fgetc(fpin)=='='){//˵����'!='��û�������Ϸ����
						Print::Nequ();
					}
					else{//����
						error(ch);
						return;
					}
					break;
				}
			case ';':
				{
					Print::Semi();
					break;
				}
			case ',':
				{
					Print::Cma();
					break;
				}
			case '(':
				{
					Print::Par(0);
					break;
				}
			case ')':
				{
					Print::Par(1);
					break;
				}
			case '[':
				{
					Print::Bk(0);
					break;
				}
			case ']':
				{
					Print::Bk(1);
					break;
				}
			case '{':
				{
					Print::Br(0);
					break;
				}
			case '}':
				{
					Print::Br(1);
					break;
				}
			default :
				{
					error(ch);
					return;
				}
		}
	}

}/*}}}*/

int main(){
	char filename[40];
	FILE *fpin;
	cout<<"������Դ�ļ���������·���ͺ�׺��:";
	while(1){
		cin>>filename;
		if((fpin=fopen(filename,"r"))!=NULL)
			break;
		else{
			cout<<"�ļ�������"<<endl;
			cout<<"������Դ�ļ���������·���ͺ�׺��:";
		}

	}
	Print::init();
	//cout<<"------�ʷ���������------"<<endl;
	analyse(fpin);
	Print::output_str();
	return 0;
}
