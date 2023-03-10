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
    vector<int> display;      //整个程序有一个控制记录嵌套层次的表
    vector<atab_ele> atab;    //数组内情表
    int count = 0;            //和display表配合使用，追踪定义
    int lll = 0;              //追踪嵌套层次

    table();                              //初始化函数
    void regis(string n, int t, int nor); //登记变量，传入参数依次为名称，类型（0为int,1为float），是否为形参（为形参则为1，其他为0）

    void regis(string n, int t, int nor, vector<int> par); //登记数组，调用时机为整个数组都翻译完时
                                                           //传进参数依次是数组名，数组元素类型（规定同上），是否为形参（规定同上），以及数组维数列表

    void regis(string n, int t); //登记函数，调用时机应该是开始规约函数内的形式参数时。即T d (出现时调用

    void end();         //函数末尾调用，对函数完结登记并且更改嵌套层次。即return} 出现时调用
                        //更改display表，更改计数器，这个嵌套层次已经结束。
    int look(string n); //查找有没有定义，没有定义则返回-1,有定义则返回其在符号表nametab中的位置
    void out(string n); //传入函数的名字，打印该函数对应的符号表的所有信息
    void out_all();     //打印nametab表全体
    int count_real(string n);
    int count_par(string n);
};
struct lev_outer
{
    int lev;   //记录登记项的嵌套层次
    int outer; //记录登记项的嵌套定义的直接外层的嵌套层次
};
struct name_ele
{
    string name;            //登记项的名字
    int kind;               //记录登记项的类型，包括0变量，1数组，2过程名
    lev_outer l_o;          // l_o.lev记录登记项的嵌套层次，主函数为第0层；l_o.outer记录登记项的嵌套定义的直接外层的层次
    int type;               //记录变量和数组的类型,0为int,1为float，为函数0是int，1是float,2是void
    int normal;             //标记是否为形参，为形参则为1，其他为0
    int ref;                //指向登记项的子表的位置，如果是变量则为-1
    int seg;                //变量和数组存储偏移地址，函数存储栈帧大小
    int link;               //指向同一程序体中定义的上一个名字的位置(下标)
    void print(ofstream &o) //打印这一个元素
    {
        o << setw(8) << setiosflags(ios::left) << name;
        if (kind == 0)
            o << setw(8) << setiosflags(ios::left) << "var";
        else if (kind == 1)
            o << setw(8) << setiosflags(ios::left) << "array";
        else
            o << setw(8) << setiosflags(ios::left) << "func";

        o << setw(8) << setiosflags(ios::left) << l_o.lev
          << setw(8) << setiosflags(ios::left) << l_o.outer;
        if (type == 0)
            o << setw(8) << setiosflags(ios::left) << "int";
        else if (type == 1)
            o << setw(8) << setiosflags(ios::left) << "float";
        else
            o << setw(8) << setiosflags(ios::left) << "void";
        if (normal == 1)
            o << setw(8) << setiosflags(ios::left) << "par";
        else
            o << setw(8) << setiosflags(ios::left) << " ";

        o << setw(8) << setiosflags(ios::left) << ref
          << setw(8) << setiosflags(ios::left) << seg
          << setw(8) << setiosflags(ios::left) << link << endl;
    }
};
struct atab_ele
{
    //数组下表类型为int，默认数组下标从0开始
    int eltyp;     //数组元素类型，0为int，1为float
    int elref;     //如果数组元素是数组的话，elref指向数组内情表下一项
    vector<int> v; //这一维数组对应的参数列表
    //相当于a[5,6,4,9],那么atab中的4个登记项v分别为（5，6，7，9）（6，7，9）（7，9）（9）
    int elsize; //数组元素的体积
    int size;   //数组本身的体积
};
struct btab_ele
{
    int lastpar; //指向最后一个形参在name表中的位置,-1表示没有上一个了
    int last;    //指向最后一个名字在name表中的位置,-1表示没有上一个了
    int func;    //指向这个btab表对应的函数的位置
    int psize;   //所有形参所占空间大小
    int vsize;   //所有局部数据所占空间大小
};
table::table()
{ //初始化函数
    display.push_back(0);
    btab_ele tmp;
    tmp.last = -1;
    tmp.lastpar = -1;
    tmp.psize = 0;
    tmp.vsize = 0;
    btab.push_back(tmp);
}
void table::regis(string n, int t, int nor)
{
    //登记变量，传入参数依次为名称，类型，是否为形参
    //登记时机为变量规约为E时
    //取出当前层次
    int cur_layer = display[display.size() - 1]; // display最后一个元素
    name_ele tmp;
    tmp.name = n;
    tmp.kind = 0;
    tmp.l_o.lev = lll; //嵌套层次
    if (display.size() >= 2)
        tmp.l_o.outer = display[display.size() - 2];
    else
        tmp.l_o.outer = -1;
    tmp.type = t;
    tmp.normal = nor;
    tmp.ref = -1;
    tmp.seg = btab[cur_layer].psize + btab[cur_layer].vsize; //变量在数据段分配的相对地址
    tmp.link = btab[cur_layer].last;                         //指向name表中同层次的上一个变量
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
    int cur_layer = display[display.size() - 1];
    int dim = par.size(); //数组维数
    name_ele tmp;
    tmp.name = n;
    tmp.kind = 1;
    tmp.l_o.lev = lll; //嵌套层次
    if (display.size() >= 2)
        tmp.l_o.outer = display[display.size() - 2];
    else
        tmp.l_o.outer = -1;
    tmp.type = t;
    tmp.normal = nor;
    tmp.ref = atab.size(); //指向数组内情表下一行
    //建立数组内情表
    for (int next = tmp.ref; (next - tmp.ref) < dim; next++)
    {
        atab_ele temp;
        temp.eltyp = t;
        temp.size = temp.elsize = 1;
        temp.v.assign(par.begin() + next - tmp.ref, par.end());
        //计算elsize
        for (int i = next - tmp.ref + 1; i < dim; i++)
            temp.elsize *= par[i];
        temp.elsize *= TYPE[t];
        temp.size = temp.elsize * par[next - tmp.ref];
        if (next - tmp.ref < dim - 1) //不是最后一维，需要指向下一个数组内情表
            temp.elref = next + 1;
        else
            temp.elref = -1;
        atab.push_back(temp);
    }
    tmp.seg = btab[cur_layer].psize + btab[cur_layer].vsize; //变量在数据段分配的相对地址
    tmp.link = btab[cur_layer].last;                         //指向name表中同层次的上一个变量
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
void table::regis(string n, int t)
{
    //登记函数，调用时机应该是开始规约函数内的形式参数时
    //先在这一层登记这个函数
    int cur_layer = display[display.size() - 1];
    name_ele tt;
    tt.name = n;
    tt.kind = 2;
    tt.l_o.lev = lll; //嵌套层次
    if (display.size() >= 2)
        tt.l_o.outer = display[display.size() - 2];
    else
        tt.l_o.outer = -1;
    tt.type = t;
    tt.normal = 0;
    tt.ref = count + 1;
    tt.seg = 0;                     //过程名填入这一个栈帧所需要的存储单元数目,随后函数翻译完进行更新
    tt.link = btab[cur_layer].last; //指向name表中同层次的上一个变量
    nametab.push_back(tt);
    //更改btab表中的指针的值
    btab[cur_layer].last = nametab.size() - 1;
    //登记下一层，进入新的嵌套层级
    count++;
    lll++;
    display.push_back(count);
    btab_ele tmp;
    tmp.last = -1;
    tmp.lastpar = -1;
    tmp.func = nametab.size() - 1;
    tmp.psize = tmp.vsize = 0;
    btab.push_back(tmp);
}
void table::end()
{
    //函数末尾调用
    int cur_layer = display[display.size() - 1];
    // seg.size项,首先要找到函数名在符号表中登记的位置
    int pt = btab[cur_layer].func; //函数的位置，指回nametab表
    nametab[pt].seg = btab[cur_layer].psize + btab[cur_layer].vsize;
    //更改display表，更改计数器，这个嵌套层次已经结束。
    lll--;
    display.pop_back();
}
int table::look(string n)
{
    //查找有没有定义，没有定义则返回-1,有定义则返回其在符号表中的位置
    int cc = display.size() - 1;
    int layer;
    int pt;
    stack<int> temp_stack;
    while (cc >= 0) //在所有的嵌套层次里面寻找
    {
        layer = display[cc];
        pt = btab[layer].last;
        while (pt != -1 && nametab[pt].name != n)
            pt = nametab[pt].link;
        if (pt != -1)
            break;
        else
            cc--;
    }
    if (pt != -1)
        return pt;
    else
        return -1;
}
void table::out(string n)
{
    int pos = 0;
    while ((nametab[pos].name != n || nametab[pos].kind != 2) && pos < nametab.size())
        pos++;
    if (pos == nametab.size())
        return;
    int pt = btab[nametab[pos].ref].last;
    ofstream dataFile;
    dataFile.open("symbol.list");
    dataFile << setw(18) << setiosflags(ios::left) << "func_name"
             << setw(18) << setiosflags(ios::left) << "func_return_type"
             << setw(18) << setiosflags(ios::left) << "func_level"
             << setw(18) << setiosflags(ios::left) << "func_outer"
             << setw(18) << setiosflags(ios::left) << "size_of_frame" << endl;
    dataFile << setw(18) << setiosflags(ios::left) << nametab[pos].name;
    if (nametab[pos].type == 0)
        dataFile << setw(18) << setiosflags(ios::left) << "int";
    else if (nametab[pos].type == 1)
        dataFile << setw(18) << setiosflags(ios::left) << "float";
    else
        dataFile << setw(18) << setiosflags(ios::left) << "void";
    dataFile << setw(18) << setiosflags(ios::left) << nametab[pos].l_o.lev
             << setw(18) << setiosflags(ios::left) << nametab[pos].l_o.outer
             << setw(18) << setiosflags(ios::left) << nametab[pos].seg << endl;
    dataFile << endl
             << "variables and parameters:" << endl;
    dataFile << setw(8) << setiosflags(ios::left) << "label"
             << setw(8) << setiosflags(ios::left) << "name"
             << setw(8) << setiosflags(ios::left) << "kind"
             << setw(8) << setiosflags(ios::left) << "level"
             << setw(8) << setiosflags(ios::left) << "outer"
             << setw(8) << setiosflags(ios::left) << "type"
             << setw(8) << setiosflags(ios::left) << "para?"
             << setw(8) << setiosflags(ios::left) << "ref"
             << setw(8) << setiosflags(ios::left) << "seg"
             << setw(8) << setiosflags(ios::left) << "link"
             << endl;
    while (pt != -1)
    {
        dataFile << setw(8) << setiosflags(ios::left) << pt;
        nametab[pt].print(dataFile);
        pt = nametab[pt].link;
    }
    dataFile.close();
    // 关闭文档
}
void table::out_all()
{
    ofstream dataFile;
    dataFile.open("out_all.txt");
    dataFile << endl
             << "variables and parameters:" << endl;
    dataFile << setw(8) << setiosflags(ios::left) << "label"
             << setw(8) << setiosflags(ios::left) << "name"
             << setw(8) << setiosflags(ios::left) << "kind"
             << setw(8) << setiosflags(ios::left) << "level"
             << setw(8) << setiosflags(ios::left) << "outer"
             << setw(8) << setiosflags(ios::left) << "type"
             << setw(8) << setiosflags(ios::left) << "para?"
             << setw(8) << setiosflags(ios::left) << "ref"
             << setw(8) << setiosflags(ios::left) << "seg"
             << setw(8) << setiosflags(ios::left) << "link"
             << endl;
    int pt = 0;
    while (pt < nametab.size())
    {
        dataFile << setw(8) << setiosflags(ios::left) << pt;
        nametab[pt].print(dataFile);
        pt++;
    }
    dataFile.close();
    // 关闭文档
}
int table::count_real(string n)
{
    int pos = 0;
    if (n != "global")
    {
        while ((nametab[pos].name != n || nametab[pos].kind != 2) && pos < nametab.size())
            pos++;
        if (pos == nametab.size())
            return -1;
    }
    int pt = nametab[pos].ref;
    int las = btab[pt].last;
    int cc = 0;
    while (las != -1)
    {
        if ((nametab[las].kind == 0 || nametab[las].kind == 1) && nametab[las].normal == 0)
            cc++;
        las = nametab[las].link;
    }
    return cc;
}
int table::count_par(string n)
{
    int pos = 0;
    if (n != "global")
    {
        while ((nametab[pos].name != n || nametab[pos].kind != 2) && pos < nametab.size())
            pos++;
        if (pos == nametab.size())
            return -1;
    }
    int pt = nametab[pos].ref;
    int las = btab[pt].lastpar;
    int cc = 0;
    while (las != -1)
    {
        cc++;
        las = nametab[las].link;
    }
    return cc;
}
// int main()
// {
//     /////////////
//     /*
//     int P()
//         int  a,b;
//         int P1 (int i1,int j1);
//             int c,d
//             …
//         end;
//         int P2 (int i2,int j2)
//             int a[4,5,6,7];
//             int P21();
//                 float b1,b2
//             return b1
//         return 0
//     return 0
//     */
//     table a;
//     a.regis("p", 0);
//     a.regis("a", 0, 0);
//     a.regis("b", 0, 0);
//     a.regis("p1", 0);
//     a.regis("i1", 0, 1);
//     a.regis("j1", 0, 1);
//     a.regis("c", 0, 0);
//     a.regis("d", 0, 0);
//     a.end();
//     a.regis("p2", 0);
//     a.regis("i2", 0, 1);
//     a.regis("j2", 0, 1);
//     vector<int> v = {4, 5, 6, 7};
//     a.regis("a", 0, 0, v);
//     a.regis("p21", 0);
//     a.regis("b1", 1, 0);
//     a.regis("b2", 1, 0);
//     a.end();
//     a.end();
//     a.end();
//     cout << a.count_real("p1");
//     cout << a.count_real("p");
// }
#endif
