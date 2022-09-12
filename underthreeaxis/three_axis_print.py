# for three axis print
import sys
import math
# Z home at min
CX,CY,CZ  = 120,135,195.8  # printing center
prx,pry,prz = 0,0,0
printing_point = []
decadeFR = 0.5
second_G28 = 0
MAXZ = 0

def feedrate(org_feedrate):
        return int(org_feedrate*decadeFR)

def roughfliter(line):
    global MAXZ
    GM_LINE = []
    for i in range(len(line)):
        if line[i][0] != ";":
            GM_LINE.append(line[i])
        elif line[i][1:5] == "MAXZ":
            MAXZ = float(line[i][6:])
    return GM_LINE

def recenter(line):
    global second_G28
    printline = []
    for i in range(len(line)):
        if line[i][0] == "G":
            if line[i][0:3] == "G28":
                if second_G28 == 1:
                    FZ = MAXZ+20
                    Finish_point = "G0 X"+str(CX)+" Y"+str(CY)+" Z"+str(FZ)+"\n"
                    printline.append(Finish_point)
                else:
                    second_G28 = 1
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
    #check output
    with open("printing.txt", 'w') as f:
        for item in afterdata:
            f.write(str(item))
    '''''''''
    with open("printing.gcode", 'w') as f:
        for item in afterdata:
            f.write(str(item))