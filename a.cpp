#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
using namespace std;
enum myType{key,id,num,sep,ope,fil};
string KEYWORD[7]={"if","else","while","return","int","float","void"};//关键字0
char SEPARATER[8]={';',',','{','}','[',']','(',')'};  //分隔符1
char FILTER[4]={' ','\t','\r','\n'};          //过滤符3
static int pre_type=sep;//前一个识别词的状态
static string pre_op="";//记录前面的符号，用来写正数还是负数
static bool skip_char=0;//是否有跳过的+
static int errorflag=0;

//各种判断{{{
/**判断是否为关键字**/
bool IsKeyword(string word){
	for(int i=0;i<7;i++){
		if(KEYWORD[i]==word){
			return true;
		}
	}
	return false;
}
/**判断是否为过滤符**/
bool IsFilter(char ch){
	for(int i=0;i<4;i++){
		if(FILTER[i]==ch){
			return true;
		}
	}
	return false;
}

/**判断是否为小写字母**/
bool IsLowLetter(char ch){
	if(ch>='a' && ch<='z') return true;
	return false;
}
/**判断是否为数字**/
bool IsDigit(char ch){
	if(ch>='0' && ch<='9') return true;
	return false;
}/*}}}*/

namespace Print{/*{{{*/
	char filename1[40];
	char filename2[40];
	string output;//记录替换输入串，最后输出
	void init(){
		cout<<"请输入 替换输入串 的输出文件名（同目录下不需写路径，输出到命令行请输入CON）:";
		cin>>filename1;
		cout<<"请输入 保存属性表格 的输出文件名（同目录下不需写路径，输出到命令行请输入CON）:";
		cin>>filename2;
		output="";
		cout<<endl;
		freopen(filename2,"w",stdout);//输出属性表格
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
		//跳过这个加号，pre_type不变
		::pre_op="";//正号不输出
		::skip_char=1;
	}
	void Negative(){
		//跳过这个减号，pre_type不变
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

/**词法分析**//*{{{*/
void error(char ch){
	freopen("CON","w",stdout);
	cout<<"存在无法识别的字符: "<<ch<<endl;
	errorflag=1;
}
void analyse(FILE * fpin){
	char ch=' ';
	string arr="";
	while((ch=fgetc(fpin))!=EOF&&!errorflag){
		arr="";
		if(IsFilter(ch)){}       //判断是否为过滤符
		else if(IsLowLetter(ch)){    //判断是否为关键字
			while(IsLowLetter(ch)){
				arr += ch;
				ch=fgetc(fpin);
			}
			//arr=抓出来的单词
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
			fseek(fpin,-1L,SEEK_CUR);//回退，新的while要重新读
		}
		else if(IsDigit(ch)){      //判断是否为数字
			bool is_int=1;
			while(IsDigit(ch)){
				arr += ch;
				ch=fgetc(fpin);
			}//int
			if (ch=='.'){//处理浮点数
				is_int=0;
				arr+=ch;
				ch=fgetc(fpin);
				while (IsDigit(ch)){
					arr+=ch;
					ch=fgetc(fpin);
				}
			}
			Print::Value(pre_op,arr,is_int);
			fseek(fpin,-1L,SEEK_CUR);//回退，新的while重新读
		}
		else switch(ch){
			case '+':
				{
					if (pre_type==num||pre_type==id){//说明是运算符
						Print::Plus();
					}
					else{//说明是数字的一部分或+=
						if (fgetc(fpin)=='='){//说明是+=
							Print::PlusEqu();
						}
						else{
							Print::Positive();
							fseek(fpin,-1L,SEEK_CUR);//回退
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
					if (fgetc(fpin)=='='){//说明是'=='
						Print::Equ2();
					}
					else{
						Print::Equ1();
						fseek(fpin,-1L,SEEK_CUR);//回退
					}
					break;
				}
			case '!':
				{
					if (fgetc(fpin)=='='){//说明是'!='，没有其他合法情况
						Print::Nequ();
					}
					else{//报错
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
	cout<<"请输入源文件名（包括路径和后缀）:";
	while(1){
		cin>>filename;
		if((fpin=fopen(filename,"r"))!=NULL)
			break;
		else{
			cout<<"文件名错误！"<<endl;
			cout<<"请输入源文件名（包括路径和后缀）:";
		}

	}
	Print::init();
	//cout<<"------词法分析如下------"<<endl;
	analyse(fpin);
	Print::output_str();
	return 0;
}
