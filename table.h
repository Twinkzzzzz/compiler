#ifndef TABLE_H
#define TABLE_H
#include <bits/stdc++.h>
using namespace std;
//����ͨ�ù涨��intռ�����ֽڣ�floatռ4���ֽ�
int TYPE[2] = {2, 4};
// xx_eleΪ���ű�һ�е�Ԫ��
union mem;
struct name_ele;
struct btab_ele;
struct atab_ele;
////////////////////˵������///////////////////////////////////////////
/*
table�ṹ���Ƿ��ű����ܰ�������������������飬����������ĳһ�����Ƿ�Ǽǡ�
�Զ�ά����
Ƕ�ײ㼶�������������ջ֡���βκ;ֲ���������ռ�ռ��С���������ջ֡��ƫ������
*/
/////////////////////////////////////////////////////////////////////
struct table
{
    vector<name_ele> nametab; //����������һ�����ֱ�
    vector<btab_ele> btab;    //����������һ��������
    vector<int> display;      //����������һ�����Ƽ�¼Ƕ�ײ�εı�
    vector<atab_ele> atab;    //���������
    int count = 0;            //��display�����ʹ�ã�׷�ٶ���
    int lll = 0;              //׷��Ƕ�ײ��

    table();                              //��ʼ������
    void regis(string n, int t, int nor); //�ǼǱ����������������Ϊ���ƣ����ͣ�0Ϊint,1Ϊfloat�����Ƿ�Ϊ�βΣ�Ϊ�β���Ϊ1������Ϊ0��

    void regis(string n, int t, int nor, vector<int> par); //�Ǽ����飬����ʱ��Ϊ�������鶼������ʱ
                                                           //��������������������������Ԫ�����ͣ��涨ͬ�ϣ����Ƿ�Ϊ�βΣ��涨ͬ�ϣ����Լ�����ά���б�

    void regis(string n, int t); //�ǼǺ���������ʱ��Ӧ���ǿ�ʼ��Լ�����ڵ���ʽ����ʱ����T d (����ʱ����

    void end();         //����ĩβ���ã��Ժ������Ǽǲ��Ҹ���Ƕ�ײ�Ρ���return} ����ʱ����
                        //����display�����ļ����������Ƕ�ײ���Ѿ�������
    int look(string n); //������û�ж��壬û�ж����򷵻�-1,�ж����򷵻����ڷ��ű�nametab�е�λ��
    void out(string n); //���뺯�������֣���ӡ�ú�����Ӧ�ķ��ű��������Ϣ
    void out_all();     //��ӡnametab��ȫ��
    int count_real(string n);
    int count_par(string n);
};
struct lev_outer
{
    int lev;   //��¼�Ǽ����Ƕ�ײ��
    int outer; //��¼�Ǽ����Ƕ�׶����ֱ������Ƕ�ײ��
};
struct name_ele
{
    string name;            //�Ǽ��������
    int kind;               //��¼�Ǽ�������ͣ�����0������1���飬2������
    lev_outer l_o;          // l_o.lev��¼�Ǽ����Ƕ�ײ�Σ�������Ϊ��0�㣻l_o.outer��¼�Ǽ����Ƕ�׶����ֱ�����Ĳ��
    int type;               //��¼���������������,0Ϊint,1Ϊfloat��Ϊ����0��int��1��float,2��void
    int normal;             //����Ƿ�Ϊ�βΣ�Ϊ�β���Ϊ1������Ϊ0
    int ref;                //ָ��Ǽ�����ӱ��λ�ã�����Ǳ�����Ϊ-1
    int seg;                //����������洢ƫ�Ƶ�ַ�������洢ջ֡��С
    int link;               //ָ��ͬһ�������ж������һ�����ֵ�λ��(�±�)
    void print(ofstream &o) //��ӡ��һ��Ԫ��
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
    //�����±�����Ϊint��Ĭ�������±��0��ʼ
    int eltyp;     //����Ԫ�����ͣ�0Ϊint��1Ϊfloat
    int elref;     //�������Ԫ��������Ļ���elrefָ�������������һ��
    vector<int> v; //��һά�����Ӧ�Ĳ����б�
    //�൱��a[5,6,4,9],��ôatab�е�4���Ǽ���v�ֱ�Ϊ��5��6��7��9����6��7��9����7��9����9��
    int elsize; //����Ԫ�ص����
    int size;   //���鱾������
};
struct btab_ele
{
    int lastpar; //ָ�����һ���β���name���е�λ��,-1��ʾû����һ����
    int last;    //ָ�����һ��������name���е�λ��,-1��ʾû����һ����
    int func;    //ָ�����btab���Ӧ�ĺ�����λ��
    int psize;   //�����β���ռ�ռ��С
    int vsize;   //���оֲ�������ռ�ռ��С
};
table::table()
{ //��ʼ������
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
    //�ǼǱ����������������Ϊ���ƣ����ͣ��Ƿ�Ϊ�β�
    //�Ǽ�ʱ��Ϊ������ԼΪEʱ
    //ȡ����ǰ���
    int cur_layer = display[display.size() - 1]; // display���һ��Ԫ��
    name_ele tmp;
    tmp.name = n;
    tmp.kind = 0;
    tmp.l_o.lev = lll; //Ƕ�ײ��
    if (display.size() >= 2)
        tmp.l_o.outer = display[display.size() - 2];
    else
        tmp.l_o.outer = -1;
    tmp.type = t;
    tmp.normal = nor;
    tmp.ref = -1;
    tmp.seg = btab[cur_layer].psize + btab[cur_layer].vsize; //���������ݶη������Ե�ַ
    tmp.link = btab[cur_layer].last;                         //ָ��name����ͬ��ε���һ������
    nametab.push_back(tmp);
    //����btab���е�ָ���ֵ
    if (nor == 1) //�ñ���Ϊ�β�
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
    //�Ǽ����飬�Ǽ�ʱ��Ϊ�������鶼������ʱ
    //��������������������������Ԫ�����ͣ��Ƿ�Ϊ�βΣ��Լ�����ά���б�
    int cur_layer = display[display.size() - 1];
    int dim = par.size(); //����ά��
    name_ele tmp;
    tmp.name = n;
    tmp.kind = 1;
    tmp.l_o.lev = lll; //Ƕ�ײ��
    if (display.size() >= 2)
        tmp.l_o.outer = display[display.size() - 2];
    else
        tmp.l_o.outer = -1;
    tmp.type = t;
    tmp.normal = nor;
    tmp.ref = atab.size(); //ָ�������������һ��
    //�������������
    for (int next = tmp.ref; (next - tmp.ref) < dim; next++)
    {
        atab_ele temp;
        temp.eltyp = t;
        temp.size = temp.elsize = 1;
        temp.v.assign(par.begin() + next - tmp.ref, par.end());
        //����elsize
        for (int i = next - tmp.ref + 1; i < dim; i++)
            temp.elsize *= par[i];
        temp.elsize *= TYPE[t];
        temp.size = temp.elsize * par[next - tmp.ref];
        if (next - tmp.ref < dim - 1) //�������һά����Ҫָ����һ�����������
            temp.elref = next + 1;
        else
            temp.elref = -1;
        atab.push_back(temp);
    }
    tmp.seg = btab[cur_layer].psize + btab[cur_layer].vsize; //���������ݶη������Ե�ַ
    tmp.link = btab[cur_layer].last;                         //ָ��name����ͬ��ε���һ������
    nametab.push_back(tmp);
    if (nor == 1) //�ñ���Ϊ�β�
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
    //�ǼǺ���������ʱ��Ӧ���ǿ�ʼ��Լ�����ڵ���ʽ����ʱ
    //������һ��Ǽ��������
    int cur_layer = display[display.size() - 1];
    name_ele tt;
    tt.name = n;
    tt.kind = 2;
    tt.l_o.lev = lll; //Ƕ�ײ��
    if (display.size() >= 2)
        tt.l_o.outer = display[display.size() - 2];
    else
        tt.l_o.outer = -1;
    tt.type = t;
    tt.normal = 0;
    tt.ref = count + 1;
    tt.seg = 0;                     //������������һ��ջ֡����Ҫ�Ĵ洢��Ԫ��Ŀ,�������������и���
    tt.link = btab[cur_layer].last; //ָ��name����ͬ��ε���һ������
    nametab.push_back(tt);
    //����btab���е�ָ���ֵ
    btab[cur_layer].last = nametab.size() - 1;
    //�Ǽ���һ�㣬�����µ�Ƕ�ײ㼶
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
    //����ĩβ����
    int cur_layer = display[display.size() - 1];
    // seg.size��,����Ҫ�ҵ��������ڷ��ű��еǼǵ�λ��
    int pt = btab[cur_layer].func; //������λ�ã�ָ��nametab��
    nametab[pt].seg = btab[cur_layer].psize + btab[cur_layer].vsize;
    //����display�����ļ����������Ƕ�ײ���Ѿ�������
    lll--;
    display.pop_back();
}
int table::look(string n)
{
    //������û�ж��壬û�ж����򷵻�-1,�ж����򷵻����ڷ��ű��е�λ��
    int cc = display.size() - 1;
    int layer;
    int pt;
    stack<int> temp_stack;
    while (cc >= 0) //�����е�Ƕ�ײ������Ѱ��
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
    // �ر��ĵ�
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
    // �ر��ĵ�
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
//             ��
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
