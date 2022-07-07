# for three axis print
import sys
import math
CX,CY,CZ  = 120,140,0.4  # printing center
prx,pry,prz = 0,0,0
printing_point = []
decadeFR = 0.5
def feedrate(org_feedrate):
        return int(org_feedrate*decadeFR)

def transfertoQ(line, flag):
    processingline = line.split()
    if flag == True:
        x, y, z = processingline[0][1:],processingline[1][1:],processingline[2][1:]
        A, C = processingline[3][1:],processingline[4][1:]
    else:
        x, y, z = processingline[1][1:],processingline[2][1:],processingline[3][1:]
    #m81 = feedrate(prx,pry,prz,x,y,z)
    # deal X
    orig_X = x  # X value
    new_X = round((347.750 + float(orig_X)),3)
    if processingline[0] == 'G0':
        printing_point.append('2000' + str(new_X)+'\n')   # m81=-100
    elif processingline[0] == 'G1':
        fedrate = 2025 #2000+m81
        printing_point.append(str(fedrate)+str(new_X)+'\n')
    #elif flag == True:
    #    fedrate = 2000+m81
    #    printing_point.append(str(fedrate)+str(new_X)+'\n')
    # deal Y
    orig_Y = y  # Y value
    new_Y = round((470.109 + float(orig_Y)),3)
    printing_point.append(str(new_Y)+'\n')
    # deal Z
    orig_Z = z  # Z value
    #new_Z = round((-148.300 + float(orig_Z)),3)
    new_Z = round((-148.500 + float(orig_Z)),3)   #原本為-148.300
    printing_point.append(str(new_Z)+'\n')
    # for A and C are 0
    if flag == True:
        printing_point.append(str(A)+'\n')
        printing_point.append(str(C)+'\n')
    else:
        printing_point.append('0'+'\n')
        printing_point.append('0'+'\n')
    return x, y, z

def gcodetoDNC(line):
    # catch out the needed gcode
    DNCpoint = []
    Z_height = 0
    line_store = []     # 儲存列印資訊
    
    for i in range(len(line)):
        if line[i][0:2] == "G0":
            z_index = line[i].find('Z')
            if z_index == -1:
                line_store.append(line[i][:-1]+" "+str(Z_height)+"\n")
            else: #儲存Z高度
                Z_height = line[i][int(z_index):-1]
                line_store.append(line[i])
        elif line[i][0:2] == "G1":
            if Z_height != 0:
                try:
                    E_index = line[i].find('E')
                    line_store.append(line[i][0:int(E_index)]+" "+str(Z_height)+" "+line[i][int(E_index):])
                except:
                    line_store.append(line[i]+" "+str(Z_height)+"\n")
    file.close()
    
    # transfer to DNC
    need_point = []
    while len(line_store) != 0:
        process_line = line_store.pop(0)     # 每行自行處理，從新整理數據
        if process_line[1:6] == "LAYER" or process_line[1:6] == "TYPE:":
            need_point.append(process_line)
        elif process_line[0] == "G":
            linesplit = process_line.split()
            for i in range(len(linesplit)):
                if linesplit[i][0] == 'G':
                    need_point.append(linesplit[i])
                elif linesplit[i][0] == 'X' or linesplit[i][0] == 'Y' or linesplit[i][0] == 'Z':
                    need_point.append(linesplit[i])
    string = ''
    for item in need_point:
        if item[0] == 'G' or item[0] == 'X' or item[0] == 'Y':
            string += (item+' ')
        elif item[0] == 'Z' :
            string += (item+'\n')
            DNCpoint.append(string)
            string = ''
        else:
            DNCpoint.append(item)
    need_point.clear()
    return DNCpoint

def roughfliter(line):
    GM_LINE = []
    for i in range(len(line)):
        if line[i][0] != ";":
            GM_LINE.append(line[i])
    
    return GM_LINE

def recenter(line):
    printline = []
    for i in range(len(line)):
        if line[i][0] == "G":
            linesplit = line[i].split()
            for j in range(len(linesplit)):
                if linesplit[j][0] == "X":
                    new_x = CX + float(linesplit[j][1:])
                    linesplit[j] = "X"+str(new_x)
                elif linesplit[j][0] == "Y":
                    new_y = CY + float(linesplit[j][1:])
                    linesplit[j] = "Y"+str(new_y)
                elif linesplit[j][0] == "Z":
                    new_z = CZ - float(linesplit[j][1:])
                    linesplit[j] = "Z"+str(new_z)
                elif linesplit[j][0] == "F" and decadeFR !=0:
                    newFR = feedrate(float(linesplit[j][1:]))
                    linesplit[j] = "F"+str(newFR)
            point =""
            for k in range(len(linesplit)):
                point += linesplit[k]+" "    
            point += "\n"
            printline.append(point)
        else:
            printline.append(line[i])
    
    return printline
            
if __name__ == '__main__':
    # read input file
    with open(sys.argv[1], "r", encoding="utf-8") as f:
        line = f.readlines()
        print("文件名稱: ", f.name)
    
    afterdata = []
    afterdata = recenter(roughfliter(line))


    '''''''''
    while len(afterdata) != 0:
        line = afterdata.pop(0)
        if line[0] == 'G' and line[3] == 'X':
            prx,pry,prz = transfertoQ(line, flag = False)
    '''''''''
    
    with open("printing.txt", 'w') as f:
        for item in afterdata:
            f.write(str(item))