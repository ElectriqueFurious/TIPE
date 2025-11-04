from PIL import Image
import glob
import numpy as np
import shutil
import os
from more_itertools import divide

rac = "out/"

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

if os.path.exists(rac+"jpg"):
    shutil.rmtree(rac+"jpg")
os.mkdir(rac+"jpg")
file = open(rac+"test.txt","r")

width = int(file.readline())
height = int (file.readline())

img = None
array = None

mid = np.zeros([width,height,3],dtype=np.uint64)
b = np.zeros([width,height])

NB_THREADS = 100

line = file.read()
line = line.replace("C\nO\n", "#").replace("O\n", "#").replace("C\n", "#").replace("\n", ",")
line = line.split("#")[:-2]
cpt_img = len(line)
line = [list(c) for c in divide(NB_THREADS, line)]

import threading 

def task(img_arr, debut):
    cpt = debut
    for img in img_arr:
        array = np.zeros([width,height,3],dtype=np.uint8)
        array[:,:] = [255,255,255]
        b[:,:] = True
        
        img = img.split(",")[:-2]
        for values in img :
            values = values.split(";")
            # print(values)
            v1,v2,v3=int(values[0]),int(values[1]),int(values[2])
            array[v1,v2] = color(v3)
            if b[v1,v2]:
                mid[v1,v2,2] += 1
                b[v1,v2] = False

        img = Image.fromarray(array)
        img.save(rac+"jpg/"+str(cpt)+".bmp","bmp")
        cpt += 1
        # print(str(cpt)+"\n")

d = 0
t = []
for i in range(NB_THREADS):
    thread = threading.Thread(target=task, args=(line[i],d, ))
    d += len(line[i])
    thread.start()
    t.append(thread)
    
for i in range(NB_THREADS):
    t[i].join()

file.close()

print("Fin threads")

new_mid = np.zeros([width,height,3],dtype=np.uint8)

for i in range(width):
    for j in range(height):
        new_mid[i,j,2] = int(mid[i,j,2]*(255/cpt_img))

img = Image.fromarray(new_mid)
img.save(rac+"mid.bmp")

frames = [Image.open(image) for image in sorted(glob.glob(rac+"jpg/*.bmp"), key=lambda x: int(os.path.basename(os.path.splitext(x)[0])))]
frame_one = frames[0]
frame_one.save(rac+"test.gif", format="GIF", append_images=frames,save_all=True, duration=80, loop=1)

shutil.rmtree(rac+"jpg")