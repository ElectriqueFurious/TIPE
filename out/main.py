import glob
import shutil
import os
import sys

from sublim import *
from analyses import *

def generate_gif():
    """Generates a GIF from the test.txt simulation output."""
    if not os.path.exists(rac+"test.txt"):
        print("Error: test.txt not found. Run 'make all_t' first.")
        return
        
    if os.path.exists(rac+"jpg"):
        shutil.rmtree(rac+"jpg")
    os.mkdir(rac+"jpg")
    
    print("Generating BMP images from test.txt...")
    cpt = grids("test.txt", 3)
    mean_grid("test.txt", cpt, 3)
    
    print("Assembling the GIF...")
    frames = [Image.open(image) for image in sorted(glob.glob(rac+"jpg/*.bmp"), key=lambda x: int(os.path.basename(os.path.splitext(x)[0])))]
    if not frames:
        print("Error: No BMP images were generated.")
        return
        
    frame_one = frames[0]
    frame_one.save(rac+"test.gif", format="GIF", append_images=frames, save_all=True, duration=80, loop=1)
    print(f"GIF saved as {rac}test.gif")
    shutil.rmtree(rac+"jpg")

def plot_results():
    """Analyzes batch simulation results and plots graphs."""
    if not os.path.exists(rac+folder) or not listdir(rac+folder):
        print(f"Error: The {rac+folder} directory is empty or does not exist. Run 'make all_m' first.")
        return
        
    print("Analyzing results and generating plots...")
    dico = {}
    fill_dico(dico)
    legend = "Behavior: " # "speed (degree/turn): "
    plot_achive_rate(dico, legend)
    plot_mean_dico(dico, legend)
    # scatter_all_dico(dico, legend)
    print("Displaying plots.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 main.py [gif|plot]")
        sys.exit(1)

    mode = sys.argv[1]

    if mode == "gif":
        generate_gif()
    elif mode == "plot":
        plot_results()
    else:
        print(f"Error: Mode '{mode}' not recognized.")
        print("Usage: python3 main.py [gif|plot]")
        sys.exit(1)