import numpy as np
import json

file = open('dic.json','r')
data = json.load(file)
file.close()

nons = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
firstdic = {}
rulefirstdic = []

def getfirst (nch):
    #print(nch)
    firstlis = set()
    itemlis = data[nch]
    for i in itemlis:
        #if (nch == 'D'): print('hello',i)
        if (len(i) == 0):
            firstlis.add('e')
            continue
        if (not(nons.find(i[0]) >= 0)):
            firstlis.add(i[0])
        else:
            k = 0
            while(1):
                if (i[k] != nch):
                    getfirst(i[k])
                    sonlis = firstdic[i[k]].copy()
                    firstlis = set.union(firstlis,sonlis)
                    if (not ('e' in firstdic[i[k]]) or (k == len(i) - 1)): break
                else:
                    if (not ('e' in firstlis) or (k == len(i) - 1)): break
                k = k + 1
    firstdic[nch] = firstlis
    return

def lisgetfirst (lis1):
    #print(lis)
    lis = lis1.copy()
    ans = set()
    for i in range(len(lis)):
        if (nons.find(lis[i]) >= 0):
            ans = set.union(ans,firstdic[lis[i]])
            if (not('e' in firstdic[lis[i]])): break
        else:
            tmp = set()
            tmp.add(lis[i])
            ans = set.union(ans,tmp)
            break
    if (i == len(lis)):
        ans.add('e')
    return ans;

def getstrrule(lis1):
    lis = lis1.copy()
    ans = str(lis[0] + '->')
    if (len(lis) == 1):
        ans = ans + 'e'
    else:
        for i in range(len(lis) - 1):
            ans = ans + str(lis[i+1])
    return ans

for nitem in data:
    getfirst(nitem)

file = open('first.txt',"w")
for i in firstdic.items():
    #print(i)
    file.write(str(i[0]) + ': ' + str(i[1]))
    file.write('\r\n')
file.write('\r\n')
#非终结符的first集

for item in data:
    rulelis = data[item]
    for rule1 in rulelis:
        rule = rule1.copy()
        firstlis = set()
        if (len(rule) == 0):
            firstlis = {'e'}
        else:
            k = 0
            while (nons.find(rule[k]) >= 0 and ('e' in firstdic[rule[k]]) and k < len(rule) - 1):
                firstlis = set.union(firstlis,firstdic[rule[k]])
                k = k + 1
            firstlis.discard('e')
            if (nons.find(rule[k]) >= 0):
                firstlis = set.union(firstlis,firstdic[rule[k]])
            else:
                tmp = set()
                tmp.add(rule[k])
                firstlis = set.union(firstlis,tmp.copy())
        rule.insert(0,item)
        rulefirstdic.append((rule,firstlis))

for i in rulefirstdic:
    file.write(getstrrule((i[0]))+': '+str(i[1]))
    file.write('\r\n')
file.close()

followdic = {}
for item in data:
    followdic[item] = set()
followdic['A'] = {'#'}
count = 0

while(1):
    count = count + 1
    for item in data:
        for rule in data[item]:
            if (len(rule) == 0): continue
            for i in range(len(rule) - 1):
                tmp = set()
                ch = rule[i]
                if (nons.find(ch) == -1): continue
                tfirst = lisgetfirst(rule[i+1:])
                tmp = set.union(tmp,tfirst)
                if ('e' in tfirst):
                    tmp = tmp.union(tmp,followdic[item])
                tmp.discard('e')
                followdic[ch] = set.union(followdic[ch],tmp)
            ch = rule[-1]
            if (nons.find(ch) >= 0):
                if (ch == 'I'): print(item,followdic[item])
                followdic[ch] = set.union(followdic[ch],followdic[item])
    if (count == 50): break;

file = open('follow.txt','w')
for var in followdic.items():
    #print(var)
    file.write(str(var[0]) + ': ' + str(var[1]))
    file.write('\r\n')
file.close()