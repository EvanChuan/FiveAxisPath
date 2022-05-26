# -*- coding: utf-8 -*-

from ctypes import sizeof

veiwpoint = []

file = open('skel-poly.polylines.txt', "r", encoding="utf-8")
line = file.readlines()
# each segmentlines are stored in templine list
templine = []
for i in range(len(line)):
    templine.append(line[i].split())

x,y,z = None,None,None
for i in range(len(templine)):
    for j in range(1,len(templine[i])):
        if j%3 ==1:
            x=templine[i][j]
        elif j%3 ==2:
            y=templine[i][j]
        elif j%3 ==0:
            z=templine[i][j]
            p1 = 'X'+x+' Y'+y+' Z'+z+'\n'
            veiwpoint.append(p1)

with open("veiwpoint.txt", 'w') as f:
    for i in range(len(veiwpoint)):
        f.write(veiwpoint[i])
    f.close()
