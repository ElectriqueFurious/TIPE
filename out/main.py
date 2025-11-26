import glob
import shutil
import os

from sublim import *
from analyses import *

if os.path.exists(rac+"jpg"):
    shutil.rmtree(rac+"jpg")
os.mkdir(rac+"jpg")

## GIF generation

if os.path.exists(rac+"test.txt"):
    cpt = grids("test.txt",3)
    mean_grid("test.txt",cpt,3)
    
    frames = [Image.open(image) for image in sorted(glob.glob(rac+"jpg/*.bmp"), key=lambda x: int(os.path.basename(os.path.splitext(x)[0])))]
    frame_one = frames[0]
    frame_one.save(rac+"test.gif", format="GIF", append_images=frames,save_all=True, duration=80, loop=1)


## Analyse of main return

# if os.path.exists(rac+folder) and listdir(rac+folder) != []:
#     dico = {}
#     fill_dico(dico)
#     legend = "vitesse (degré/tour) : "
#     plot_achive_rate(dico,legend)
#     plot_mean_dico(dico,legend)
#     # scatter_all_dico(dico,legend)

# shutil.rmtree(rac+"jpg")