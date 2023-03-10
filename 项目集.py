import numpy as np
import json

def setins(set,ele):
    length = len(set)
    set.add(ele)
    if (len(set) > length):
        return 1
    else:
        return 0

def listfind(lis,ele):
    for i in range(len(lis)):
        if (lis[i] == ele):
            return i
    return -1

def listostr(lis):
    ans = str(lis[0] + ' ->')
    for i in range(len(lis) - 1):
        ans = ans + ' ' + str(lis[i+1])
    return ans

def tlistostr(lis):
    ans = ''
    for i in lis:
        ans = ans + listostr(i) + '/'
    ans = ans[:len(ans) - 1]
    return ans

nons = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

#Î->I Š->V Ď->C Ě->F

file = open('dic.json','r')
data = json.load(file)
file.close()

itemdic = {}
itemdicp = {}
count = 0
for itemkey in data:
    itemlist = []
    extl = data[itemkey]
    for expr in extl:
        rulelist = []
        for i in range(len(expr)+1):
            tmp = expr.copy()
            tmp.insert(i,'·')
            tmp.insert(0,count)
            rulelist.append(tmp.copy())
            tmp.pop(0)
            tmp.insert(0,itemkey)
            itemdicp[count]=tmp.copy()
            count = count + 1
        itemlist.append(rulelist)
    itemdic[itemkey] = itemlist

norm = []
normset = []
normdic = {}
count = 0
for var in itemdicp:
    closure = [itemdicp[var]]
    clonum = set({var})
    i = 0
    while (i < len(closure)):
        item = closure[i]
        k = listfind(item,'·')
        if (k >= len(item) - 1):
            i = i + 1
            continue
        k = k + 1
        next = item[k]
        if (nons.find(next) >= 0):
            for j in itemdic[next]:
                tmp = j[0].copy()
                num = tmp[0]
                tmp.pop(0)
                tmp.insert(0,next)
                if (setins(clonum,num) == 1): closure.append(tmp.copy())
        i = i + 1
    if (listfind(normset,clonum) == -1):
        normset.append(clonum.copy())
        norm.append(closure)
        normdic[count] = closure.copy()
        count = count + 1
file = open('norm.txt',"w")
for var in normdic.items():
    file.write(str(var[0])+':'+tlistostr(var[1]))
    #file.write(str(var[0])+':'+str(var[1]))
    file.write('\r\n')
file.close()
#规范项目集族
