# Guard Simulation Project

This project simulates the behavior of guards and a player in a 2D environment. It allows for generating GIFs of individual simulations or running batches of simulations to analyze the performance of different guard behaviors.

## Compilation

The project is compiled using the provided `Makefile`. The main targets are explained in the following section.

## Makefile Usage

The `Makefile` simplifies the compilation and execution of simulations.

-   `make tests`: Compiles the single simulation program and generates the `tests.out` executable.
-   `make main`: Compiles the batch simulation program and generates the `main.out` executable.
-   `make all_t`: **All-in-one shortcut to generate a GIF.** Compiles and runs the test simulation, then generates the `out/test.gif` file.
-   `make all_m`: **All-in-one shortcut for analysis.** Launches a batch simulation with default parameters, then generates and displays analysis graphs.
-   `make clean`: Deletes object files, executables, and result files (`.txt`).

## How to Generate a Simulation GIF

The simplest method is to use the `all_t` target of the `Makefile`, which compiles, runs the test simulation, and generates the `out/test.gif` file.

To customize this test simulation, you need to modify the `tests/tests.c` file.

1.  **Open `tests/tests.c`.** The main parameters are found in the `test_simulate_path` function.

2.  **Adjust entity parameters:**
    *   **Number of guards:** Modify the `number` variable.
        ```c
        // in tests/tests.c
        int number = 180; // The simulation will run with 180 guards.
        ```
    *   **Guard behavior:** Modify the second argument of `new_guard_list`. The list of behaviors (1: rotation, 2: random_line, etc.) is further down in this README.
        ```c
        // in tests/tests.c
        guard_list* guards = new_guard_list(number, 5, ROTATION_SPEED, 1, 4); // Behavior 5 (mousaid).
        ```
    *   **Guard position:** To test a precise configuration, you can fix a guard's position by uncommenting and modifying the `moveto` line.
        ```c
        // in tests/tests.c
        // moveto(guards->tab[0]->pos, 50, 50); // Moves guard 0 to (50, 50)
        ```

3.  **Adjust simulation parameters:**
    The `simulate` function is called near the end of `test_simulate_path`. You can modify its arguments for more customization.
    `simulate(scene, guards, bot, 5, n, true, f)`
    *   **`speed` (4th argument):** Controls the maximum duration of the simulation (`max_duration = height * speed`). Increasing this value gives the player more time to reach the exit. The value here is `5`.
    *   **`accuracy` (5th argument, variable `n`):** This is the depth of the player's anticipation. This parameter is defined in the `main` function of `tests/tests.c`.
        ```c
        // in tests/tests.c, main() function
        test_simulate_path(100); // Accuracy is 100 here.
        ```
    *   **`export` (6th argument):** This is a boolean (`true` or `false`). It must be `true` for the program to write simulation data to a file, which is necessary for GIF creation.

4.  **Execute the `make all_t` command:**
    ```bash
    make all_t
    ```
    After saving your changes in `tests/tests.c`, this command will restart the entire process.

## How to Run Batch Simulations

The simplest method to run batch simulations is to use the `all_m` target of the `Makefile`, which handles everything (compilation, execution, analysis).

To configure your batch of simulations, you can directly modify the parameters in the `Makefile`.

1.  **Open the `Makefile` file.**

2.  **Modify the simulation variables** at the beginning of the file:
    ```makefile
    # Parameters for all_m
    K        = 20
    N        = 1
    M        = 200
    PAS      = 5
    ACCURACY = 20
    ```
    -   `K`: The number of simulations to run for each configuration.
    -   `N`, `M`: The range for the number of guards (from N to M).
    -   `PAS`: The increment step for the number of guards.
    -   `ACCURACY`: The precision of the player's algorithm (number of anticipated turns).

3.  **Execute the `make all_m` command:**
    ```bash
    make all_m
    ```
    This command will launch the simulations with your parameters and generate the corresponding analysis graphs.

4.  **(Optional) Change the type of batch simulation:**
    If you want to run another type of batch simulation (e.g., to test the impact of guard rotation speed), you can modify the `main.c` file.
    -   Open the `main.c` file.
    -   In the `main` function, you can comment out the `simulate_from_to(...)` line and uncomment the `find_angle_rotation(...)` line to change the simulation type.
        ```c
        // Example in main.c
        simulate_from_to(k,n,m,pas,MAX_BEHAVE,accuracy);
        // find_angle_rotation(0,20,2,k,n,m,pas,accuracy);
        ```
    -   You can also create your own simulation function based on the `simulate_from_to` or `find_angle_rotation` model and call it in `main`.
    -   After modifying `main.c`, rerun `make all_m` to compile and execute your new simulations.

5.  **(Optional) Change the range of simulated behaviors:**
    By default, the batch simulation (`simulate_from_to`) tests a range of guard behaviors that is fixed directly in the code. You can modify it to analyze only the behaviors you are interested in.
    -   Open the `main.c` file.
    -   Go to the `simulate_from_to` function.
    -   Modify the 3rd and 4th arguments of the `k_simulation` call to define the range of behaviors (e.g., from 1 to 4).
        ```c
        // in main.c, simulate_from_to function
        k_simulation(k, i, 1, 4, accuracy); // Simulates behaviors 1, 2, 3 and 4
        ```
    -   The program also uses a `MAX_BEHAVE` constant at the beginning of the file. Currently, it is not used by `simulate_from_to`, but you could modify the function to use `MAX_BEHAVE` as an upper bound if you wish (`k_simulation(k, i, 1, range, accuracy)`).

## Python Analysis and Visualization Scripts

The `out/` folder contains several Python scripts for processing and visualizing data.

-   `main.py`: The main script that takes a command-line argument:
    -   `gif`: Reads `test.txt` to create an animated GIF.
    -   `plot`: Reads data in `out/score/` to display performance graphs.
    The `Makefile` commands (`all_t`, `all_m`) use these arguments automatically.
-   `sublim.py`: Library for creating images from simulation data.
-   `analyses.py`: Library for statistical analysis of scores and graph generation with `matplotlib`.
-   `const.py`: Constants file defining folder paths.

To use these scripts, the `matplotlib` and `Pillow` libraries are required:
```bash
pip install matplotlib Pillow
```

## Configuration

### The `accuracy` parameter
This parameter, passed to the `main` executable or modified in `tests.c`, is crucial for the player's intelligence. It represents the **number of game turns the player can anticipate**.

Specifically, the program pre-calculates the `accuracy` next movements of the guards. The player's pathfinding algorithm (A*) then uses these future positions to find a path that is not only safe now but will also remain safe for the next `accuracy` turns.

-   **Low `accuracy`** will make the player "short-sighted": they might commit to a path that seems safe but will become a trap.
-   **High `accuracy`** will allow the player to find complex strategies to avoid guards but will significantly increase computation time.

**Performance warning:** The impact of `accuracy` on computation time can be very significant. In the worst case, the increase is exponential. If a situation is simple (few guards, obvious path), any level of accuracy will be fast. However, for the same very complex simulation, computation time can more than double when going from an `accuracy` of 100 to 150. It is therefore advisable to start with low-values and increase them gradually.

### Guard Behaviors

Behaviors (`behave`) are defined in the `initializer` function of the `entity/guard.c` file. Here is the list:

| Value | Behavior         |
| :----: | :--------------- |
|   1    | `rotation`       |
|   2    | `random_line`    |
|   3    | `corridor`       |
|   4    | `centred_square` |
|   5    | `mousaid`        |
|   6    | `snake`          |
| other  | `nothing`        |

### Global Parameters

You can modify global constants for the simulation in `values.h`, such as:

-   `HEIGHT`, `WIDTH`: Dimensions of the simulation area.
-   `VISION_DISTANCE`: Guards' vision distance.
-   `VISION_FIELD`: Guards' field of vision (in degrees).
-   `ROTATION_SPEED`: Guards' rotation speed.
