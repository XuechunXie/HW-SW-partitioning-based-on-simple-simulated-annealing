

def main():


    #put the name of the input file
    a = "pptExample.txt"

    getasmAndoutputrequiredFormateForCL(a)

  




def getasmAndoutputrequiredFormateForCL(asmfileName):



    a = "pptExample.txt"

    b = listWithoutDivideDeleteSpaceAndTab(asmfileToLinesListWithoutDivide(a))
    #b = asmfileToLinesListWithoutDivide(a)

    #c = partToLineList(b[0])

    e = optimizelist(b)
    c = listWithoutDivideToPartList(e)

    print(c)

    #c[number] choice number of node  improtant!
    #d = partToLineList(c[9])
    #print(d)

    outputList = []
    for idx,partToLineListEachNode in enumerate(c):
        outputList.append(giveOutputforEachNode(idx,partToLineListEachNode))


    firstLineList = giveFirstLineList(outputList)




    outputListOfList = []

    #print(outputList)
    outputListOfList.append(firstLineList)
    outputListOfList.append(outputList)

    #print(finaloutputList)


    finaloutputList = []
    tempList = []
    for item in outputListOfList:
        
        for  ele in item:
            tempList.append(str(ele) + " ")
            
        finaloutputList.append(tempList)
        tempList = []
            
    

    writeFileforCL('pptExampleCL.txt',firstLineList,outputList)




### Has some problems ";" is not completely deleted, and some" xxx :" is deleted



#write file for the computational load
def writeFileforCL(myFileName,firstLineList,otherList):
    with open(myFileName, 'w') as f:
        f.write("%s\n" % convertListToStr(firstLineList))

        
        for item in otherList:
            f.write("%s\n" % convertListToStr(item))
    return 0

#Delete comments for the assembly code
def writeFile(myFileName,my_list):
    with open(myFileName, 'w') as f:
        for item in my_list:
            if item[-1] == ":":
                f.write("%s\n" % item)
            else:
                Tab = "\t"
                item = Tab + item
                f.write("%s\n" % item)
    return 0

#Convert a list to a string
def convertListToStr(lst):

    string = ''

    for s in lst:
        string = string+str(s)+"\t"


    return string

#Read the assebly file and converted to a List line by line
def asmfileToLinesListWithoutDivide(name):
    with open(name, 'r') as f:
        linesListWithoutDivide = [line.strip() for line in f] 

    return linesListWithoutDivide


def listWithoutDivideDeleteSpaceAndTab(linesListWithoutDivide):
    afterdelet = []

    countSpace = 0
    rightLine = []

    combine = []

    for line in linesListWithoutDivide:
        linesp = line.split()
        if len(linesp) <3:
            rightLine.append(linesp)
        else:
            rightLine.append([linesp[0]+' '+linesp[1]])
            
    afterdelet.append(rightLine)
    rightLine = []
    afterdelet = afterdelet[0]

    return afterdelet


def joincharToList(beforeList):
    afterList = []
    afterList.append(''.join(beforeList))
    return afterList



def optimizelist(lst):
    afterlst = []
    new = ""
    for item in lst:
        if len(item) == 2:
            new = str(item[0]+" "+item[1])
            afterlst.append(new)
        if len(item) == 1:
            new = str(item[0])
            afterlst.append(new)

    return afterlst







def listWithoutDivideToPartList(linesListWithoutDivide):
    
    
    partList = []
    part = []
    #part2 = []
    countcolon = 0
    linecount = 0
    for line in linesListWithoutDivide:
        linecount = linecount + 1

        #check the ":" situation
        if line[-1] == ":":
            countcolon = countcolon + 1
            if (countcolon == 1):
                part.append(line)

            #if next colon
            elif (countcolon == 2):
                partList.append(part)
                part = []
                part.append(line)
                countcolon = 1
            else:
                partList.append(part)
                part = []
                

        #noncolon situation
        else:
            part.append(line)

        #put the last part into part list
        if len(linesListWithoutDivide) == linecount:
            partList.append(part)
            linecount = 0
    

    return partList



# part => linelist
def partToLineList(eachPart):

    lineList = []
    #put each line to line
    for line in eachPart:

        #ignore the label when sweep lines
        findEnd = line[-1]
        if findEnd != ":":
            lineList.append(line)

    return lineList





# mov A,3 => "mov" One to one relation
def lineToInstruction(eachline):

    breakdown = eachline.split(" ")
    instruction = breakdown[0]
   
    return instruction


#add new instruction or increase instruction into the present instruction list
def addNewInstructionToList(preInstructionList,Instruction):

    #check whether the instruction list is empty
    if bool(preInstructionList) == 0:
        preInstructionList[Instruction] = 1

    #Not empty
    else:
        #sweep each line 
        for key,val in preInstructionList.items():

            # OpList has this instruction type
            if Instruction == key:
                preInstructionList[key] = preInstructionList[key] + 1
                
                return preInstructionList
            
        #if there is not this instruction type               
        preInstructionList[Instruction] = 1


    postInstructionList = preInstructionList

    return postInstructionList


########################################################

#get the psoc table list
def psocTableToList():
    
    file = open("psocTable.txt", "r")
    rewfile = (file.read())

    tableList = rewfile.split("\n")

    return tableList




def instPatternMatchWithTable(tableList,eachInst):

    idxOfTable = 0

    tableItemList = []
    
    oneTableInstList = []
    
    eachInstList = eachInst.split()
    
    for idx,item in enumerate(tableList):
        tableItemList = item.split()
        oneTableInstList = tableItemList[0:1]
        #jmp do not have other thing
        if instPatternDetection(str(oneTableInstList),str(eachInstList)):
            idxOfTable = idx
    

    return idxOfTable



#strtable = 
def instPatternDetection(strcode,strtable):
    detect = False
    
    if (((strtable == "jmp")|(strtable == "jc")|(strtable == "jacc")
    |(strtable == "nop")|(strtable == "index")
    |(strtable == "jnc")|(strtable == "jz")) == 0):

        patterntable = []
        str3 = strtable.split(" ")
        #print(str3)
        for char in str3[1]:
            if ((char == "[")|(char == "]")|(char == ",")|(char == "+")
                |(char == "-")|(char == "reg")|(char == "A")|(char == "X")|(char == "F")):
                patterntable.append(char)
            
        patterncode = []
        str4 = strcode.split(" ")
        for char in str4[1]:
            if ((char == "[")|(char == "]")|(char == ",")|(char == "+")
                |(char == "-")|(char == "reg")|(char == "A")|(char == "X")|(char == "F")):
                patterncode.append(char)

        i = 0
        for idx,char in enumerate(patterncode):
            if ((char == "A")|(char == "X")|(char == "F")):
                i = idx

        if ((i == len(patterncode))|(i == 0)) != 1:
            patterncode.remove(patterncode[i])

        detect = (patterncode == patterntable)

        return detect

    #has some problem when detect jmp. jz,jc
    else:
        str3 = strtable.split(" ")
        str4 = strcode.split(" ")

        detect = (str3[0] == str4[0])
        
        return detect
        
      




def calClForEachInst(eachInst):


    #might have problem
    #CL = []
    CL = 0
    tableList = psocTableToList()
    mnemonic = eachInst.split()[0]
    #print(mnemonic)

    

    strcode = ''
    stringtable = []
    strtable = ''
    strtableList = []
    mnemTable = ''

    index = []
    #findIdxFromTable, get the mnem which is the same
    for idx,item in enumerate(tableList):

        #print(item)
        
        OneInstList = item.split()
        #print(OneInstList[0])
        mnemtemp = OneInstList[0]
        if mnemonic == mnemtemp:
            
            stringtable.append(item)
            index.append(idx)

    #print(index)
    #print(stringtable)


    #pattern becomes narrow
    for i,strtab in enumerate(stringtable):

        #idex
        #index[i]

        OneList = []

        #print(eachInst)
        #print(stringtable)

        str1 = strtab.split()
        #print(str1)

        #check whether they are special instruction jmp jz
        if (((str1[0] == "jmp")|(str1[0] == "jc")|(str1[0] == "jacc")
        |(str1[0] == "nop")|(str1[0] == "index")
        |(str1[0] == "jnc")|(str1[0] == "jz")) == 0):
            strtableList = str1[0:2]
            #print(strtableList[1])
            strtable = strtableList[0]+" "+strtableList[1]
            #print(strtable)

        else:
            strtableList = str1[0]
            strtable = strtableList
            #print(strtableList)
            

        #strtable = strtableList

        #print(strtable)
        
        
        if instPatternDetection(eachInst,strtable):
            
            OneList = tableList[index[i]].split()

            #might have problem
            CL = (OneList[len(OneList)-3])
            #print(CL)
            #print(len(OneList)-3)
        



    return CL


#jmp or call mov okay     and F,F9 is not okay because if has F,F cannot detect properly

#mov [WWW],02 has ['8', '8']    not okay because there is  + L+1 cmp [m],L+1

####################################








'''
def calTotalNthNode(listWithoutDivideToPartList):

    return len(listWithoutDivideToPartList)
'''


def calNr_op(InstList):

    Nr_op = 0
    for i,key in enumerate(InstList):
        Nr_op = Nr_op + InstList[key]

    return Nr_op



def calNr_kind_op(InstList):


    Nr_kind_op = len(InstList)
    
    return Nr_kind_op




def giveOutputforEachNode(nodeNumber,partToLineListEachNode):

    
    resultListEachNode = []
    resultListEachNode.append(nodeNumber)

    clEachnode = 0
    
    for item in partToLineListEachNode:
        clEachInst = calClForEachInst(item)
        clEachnode = clEachnode + int(clEachInst)

    
    resultListEachNode.append(clEachnode)



    instList = {}
    for eachline in partToLineListEachNode:
        
        inst = lineToInstruction(eachline)
        
        insList = addNewInstructionToList(instList,inst)



    nr_op = calNr_op(instList)
    
    resultListEachNode.append(nr_op)

    nr_kind_op = calNr_kind_op(instList)
    
    resultListEachNode.append(nr_kind_op)

    

    return resultListEachNode

def getNumOfTotalNodes(outputList):

    return len(outputList)

def getTotalCL(outputList):

    totalCL = 0

    for line in outputList:

        totalCL = totalCL + line[1]

    return totalCL

def giveFirstLineList(outputList):

    firstList = []

    firstList.append(getNumOfTotalNodes(outputList))
    firstList.append(getTotalCL(outputList))

    return firstList



if __name__ == "__main__":
    main()  

        



