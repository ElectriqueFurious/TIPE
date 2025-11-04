from PIL import Image
import numpy as np

from const import *

def color(n):
    match n:
        case 1:
            return [0,0,255] 
        case 2:
            return [255,0,0]
        case 3:
            return [0,255,0]
        case 4:
            return [0,0,100]
        case 5:
            return [255,0,255]
        case _:
            return [255,255,255]

def minmax(t,cpt):
    min,max = cpt,0
    for l in t:
        for v in l:
            if v[2]<min:
                min = v[2]
            elif v[2]>max:
                max = v[2]
    return min,max

def grids(filename,k):
    file = open(rac+filename,"r")
    width = int(file.readline())
    height = int (file.readline())
    cpt = 0
    img = None
    array = None
    line = file.readline()
    while line: 
        if line[0] == "O":
            array = np.zeros([width*k,height*k,3],dtype=np.uint8)
            array[:,:] = [255,255,255]
        elif line[0] == "C":
            img = Image.fromarray(array)
            img.save(rac+"jpg/"+str(cpt)+".bmp","bmp")
            cpt += 1
        else:
            values = line.split(";")
            v1,v2,v3=int(values[0])*k,int(values[1])*k,int(values[2])
            array[v1:v1+k,v2:v2+k] = color(v3)
        line = file.readline()
    file.close()
    return cpt
        
def mean_grid(filename,cpt,k):
    file = open(rac+filename,"r")
    width = int(file.readline())
    height = int (file.readline())
    mid = np.zeros([width,height,3],dtype=np.uint64)
    b = np.zeros([width,height])
    line = file.readline()
    while line: 
        if line[0] == "O":
            b[:,:] = True
        elif line[0] != "C":
            values = line.split(";")
            v1,v2=int(values[0]),int(values[1])
            if b[v1,v2]:
                mid[v1,v2,2] += 1
                b[v1,v2] = False
        line = file.readline()
    min,max = minmax(mid,cpt) 
    new_mid = np.zeros([width*k,height*k,3],dtype=np.uint8)
    for i in range(width):
        for j in range(height):
            new_mid[i*k:(i+1)*k,j*k:(j+1)*k,0:2] = int((mid[i,j,2]-min)*(255/(max-min+1)))
    img = Image.fromarray(new_mid)
    img.save(rac+"mid.bmp")
    file.close()