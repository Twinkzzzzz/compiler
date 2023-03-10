#ifndef TABLE_H
#define TABLE_H
#include <bits/stdc++.h>
using namespace std;
//按照通用规定，int占两个字节，float占4个字节
int TYPE[2] = {2, 4};
// xx_ele为符号表一行的元素
union mem;
struct name_ele;
struct btab_ele;
struct atab_ele;
////////////////////说明如下///////////////////////////////////////////
/*
table结构体是符号表。功能包括包括定义变量，数组，函数；查找某一变量是否登记。
自动维护：
嵌套层级，内情表，计算算栈帧内形参和局部变量的所占空间大小，给出相对栈帧的偏移量。
*/
/////////////////////////////////////////////////////////////////////
struct table
{
    vector<name_ele> nametab; //整个程序有一个名字表
    vector<btab_ele> btab;    //整个程序有一个函数表
    stack<int> display;       //整个程序有一个控制记录嵌套层次的栈
    vector<atab_ele> atab;    //数组内情表
    int count = 0;            //和display表配合使用，追踪嵌套层次

    void regis(string n, int t, int nor); //登记变量，传入参数依次为名称，类型（0为int,1为float），是否为形参（为形参则为1，其他为0）

    void regis(string n, int t, int nor, vector<int> par); //登记数组，调用时机为整个数组都翻译完时
                                                           //传进参数依次是数组名，数组元素类型（规定同上），是否为形参（规定同上），以及数组维数列表

    void regis(string n); //登记函数，调用时机应该是开始规约函数内的形式参数时。即T d (出现时调用

    void end();         //函数末尾调用，对函数完结登记并且更改嵌套层次。即return} 出现时调用
                        //更改display表，更改计数器，这个嵌套层次已经结束。
    int look(string n); //查找有没有定义，没有定义则返回-1,有定义则返回其在符号表nametab中的位置
};
union mem
{
    int adr;  //变量，数组填入其在栈帧中的相对地址
    int size; //过程名填入这一个栈帧所需要的存储单元数目
};
struct name_ele
{
    string name; //登记项的名字
    int kind;    //记录登记项的类型，包括0变量，1数组，2过程名
    int lev;     //记录登记项的嵌套层次，主函数为第0层
    int type;    //记录变量和数组的类型,0为int,1为float，-1为函数没有类型
    int normal;  //标记是否为形参，为形参则为1，其他为0
    int ref;     //指向登记项的子表的位置，如果是变量则为-1
    mem seg;
    int link; //指向同一程序体中定义的上一个名字的位置(下标)
};
struct atab_ele
{
    //数组下表类型为int，默认数组下标从0开始
    int eltyp;  //数组元素类型，0为int，1为float
    int elref;  //如果数组元素是数组的话，elref指向数组内情表下一项
    int elsize; //数组元素的体积
    int size;   //数组本身的体积
};
struct btab_ele
{
    int lastpar; //指向最后一个形参在name表中的位置,-1表示没有上一个了
    int last;    //指向最后一个名字在name表中的位置,-1表示没有上一个了
    int psize;   //所有形参所占空间大小
    int vsize;   //所有局部数据所占空间大小
};

void table::regis(string n, int t, int nor)
{
    //登记变量，传入参数依次为名称，类型，是否为形参
    //登记时机为变量规约为E时
    //取出当前层次
    int cur_layer = display.top();
    name_ele tmp;
    tmp.name = n;
    tmp.kind = 0;
    tmp.lev = cur_layer; //嵌套层次
    tmp.type = t;
    tmp.normal = nor;
    tmp.ref = -1;
    tmp.seg.adr = btab[cur_layer].psize + btab[cur_layer].vsize; //变量在数据段分配的相对地址
    tmp.link = btab[cur_layer].last;                             //指向name表中同层次的上一个变量
    nametab.push_back(tmp);
    //更改btab表中的指针的值
    if (nor == 1) //该变量为形参
    {
        btab[cur_layer].lastpar = btab[cur_layer].last = nametab.size() - 1;
        btab[cur_layer].psize += TYPE[t];
    }
    else
    {
        btab[cur_layer].last = nametab.size() - 1;
        btab[cur_layer].vsize += TYPE[t];
    }
}
void table::regis(string n, int t, int nor, vector<int> par)
{
    //登记数组，登记时机为整个数组都翻译完时
    //传进参数依次是数组名，数组元素类型，是否为形参，以及数组维数列表
    int cur_layer = display.top();
    int dim = par.size(); //数组维数
    name_ele tmp;
    tmp.name = n;
    tmp.kind = 1;
    tmp.lev = cur_layer; //嵌套层次
    tmp.type = t;
    tmp.normal = nor;
    tmp.ref = atab.size(); //指向数组内情表下一行
    //建立数组内情表
    for (int next = tmp.ref; (next - tmp.ref) < dim; next++)
    {
        atab_ele temp;
        temp.eltyp = t;
        if (next - tmp.ref < dim - 1) //不是最后一维，需要指向下一个数组内情表
        {
            temp.eltyp = t;
            temp.elref = next + 1;
            //计算elsize
            for (int i = next - tmp.ref + 1; i < dim; i++)
                temp.elsize *= par[i];
            temp.elsize *= TYPE[t];
            temp.size = temp.elsize * par[next - tmp.ref];
        }
        atab.push_back(temp);
    }
    tmp.seg.adr = btab[cur_layer].psize + btab[cur_layer].vsize; //变量在数据段分配的相对地址
    tmp.link = btab[cur_layer].last;                             //指向name表中同层次的上一个变量
    nametab.push_back(tmp);
    if (nor == 1) //该变量为形参
    {
        btab[cur_layer].lastpar = btab[cur_layer].last = nametab.size() - 1;
        btab[cur_layer].psize += atab[tmp.ref].size;
    }
    else
    {
        btab[cur_layer].last = nametab.size() - 1;
        btab[cur_layer].vsize += atab[tmp.ref].size;
    }
}
void table::regis(string n)
{
    //登记函数，调用时机应该是开始规约函数内的形式参数时
    //先在这一层登记这个函数
    int cur_layer = display.top();
    name_ele tt;
    tt.name = n;
    tt.kind = 2;
    tt.lev = cur_layer; //嵌套层次
    tt.type = -1;
    tt.normal = 0;
    tt.ref = cur_layer;
    tt.seg.size = 0;                //过程名填入这一个栈帧所需要的存储单元数目,随后函数翻译完进行更新
    tt.link = btab[cur_layer].last; //指向name表中同层次的上一个变量
    nametab.push_back(tt);
    //更改btab表中的指针的值
    btab[cur_layer].last = nametab.size() - 1;
    //登记下一层，进入新的嵌套层级
    display.push(count);
    count++;
    btab_ele tmp;
    tmp.last = -1;
    tmp.lastpar = -1;
    tmp.psize = tmp.vsize = 0;
    btab.push_back(tmp);
}
void table::end()
{
    //函数末尾调用
    int cur_layer = display.top();
    // seg.size项,首先要找到函数名在符号表中登记的位置
    int pt = btab[cur_layer].lastpar; //函数定义最后一个参数的位置，函数定义一定在前面
    while (nametab[pt].kind != 2)
        pt--;
    nametab[pt].seg.size = btab[cur_layer].psize + btab[cur_layer].vsize;
    //更改display表，更改计数器，这个嵌套层次已经结束。
    display.pop();
}
int table::look(string n)
{
    //查找有没有定义，没有定义则返回-1,有定义则返回其在符号表中的位置
    int layer;
    int pt;
    stack<int> temp_stack;
    while (!display.empty()) //在所有的嵌套层次里面寻找
    {
        layer = display.top();
        pt = btab[layer].last;
        while (pt != -1 && nametab[pt].name != n)
            pt = nametab[pt].link;
        if (pt != -1)
            break;
        else
        {
            temp_stack.push(display.top());
            display.pop();
        }
    }
    while (!temp_stack.empty())
    {
        display.push(temp_stack.top());
        temp_stack.pop();
    }
    if (pt != -1)
        return pt;
    else
        return -1;
}
#endif
