# for three axis print
from audioop import lin2adpcm
import sys

printing_point = []

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
    last_Z = 0
    for i in range(len(line)):
        if line[i][0] == "G":
            linesplit = line[i].split()
            for j in range(len(linesplit)):
                if linesplit[j][0] == "F":
                    f = float(linesplit[j][1:])
                    if f > 1000 :
                        f = f/2
                        linesplit[j] = "F"+str(f)
                elif linesplit[j][0] == "X":
                    new_x = CX + float(linesplit[j][1:])
                    linesplit[j] = "X"+str(new_x)
                elif linesplit[j][0] == "Y":
                    new_y = CY + float(linesplit[j][1:])
                    linesplit[j] = "Y"+str(new_y)
                elif linesplit[j][0] == "Z":
                    new_z = CZ - float(linesplit[j][1:])
                    last_Z =new_z
                    linesplit[j] = "Z"+str(new_z)
            point =""
            for k in range(len(linesplit)):
                point += linesplit[k]+" "    
            point += "\n"
            printline.append(point)
        else:
            printline.append(line[i])
    print("last_Z is ", last_Z)
    return printline
            
if __name__ == '__main__':
    # read input file
    with open(sys.argv[1], "r", encoding="utf-8") as f:
        line = f.readlines()
        print("文件名稱: ", f.name)
    
    afterdata = []
    while len(line) != 0:
        process_line = line.pop(0)     # 每行自行處理
        linesplit = process_line.split()
        if len(linesplit) == 3:
            P = "X"+str(linesplit[0])+" Y"+str(linesplit[1])+" Z"+str(linesplit[2])+"\n"
            afterdata.append(P)
        else:
            P1 = "X"+str(linesplit[1])+" Y"+str(linesplit[2])+" Z"+str(linesplit[3])+"\n"
            afterdata.append(P1)
            P2 = "X"+str(linesplit[4])+" Y"+str(linesplit[5])+" Z"+str(linesplit[6])+"\n"
            afterdata.append(P2)
    
    
    with open("skeleton_view.txt", 'w') as f:
        for item in afterdata:
            f.write(str(item))
