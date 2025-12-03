# TO DO

## Research: Keywords

- [x] networks intruders
- [x] cop and rob problem
- [x] Fugitive search games
- [x] Pursuit evasion in graphs
- [x] moving maze
- [x] patrolling game
- [ ] Potentially remove "patrolling game" and replace it with "robbers and cops"

## Code Cleanup

- [x] Remove copy-pasted code
- [x] Rename variables
- [x] 80 columns max

## Updates

- [x] New guard structure
- [x] Optimize pathfinding
- [x] Resize heap
- [x] Clean up structs
- [x] Change `end`
- [x] Multicore support
- [x] Check if `cp` is needed
- [x] Valgrind (19 and 20)
- [x] Undetected issue (1 to 3)
- [x] Rename 19 and 20
- [x] Save all
- [x] Finish sublime 2

## Python Output

- [x] Python version
- [x] Average grid
- [x] Relative average grid
- [x] Slides
- [ ] Score graph
- [x] Multiply size

## Interface (argc/v python or c)

- [x] assert

## Optimizations

- [x] Filename filled with zeros
- [x] Modify `draw` to get rid of `clean_hole`
- [x] Multithread main
- [x] Remove insta-kill
- [x] Not too long for "nothing"

## Main exploitation function

- [x] Run x simulations and output x scores
- [x] Make a render among the x simulations
- [x] Display the number of simulations done
- [x] Run k simulations for all behaviors for n to m guards

## Guard refactoring

- [x] Finish `natural_move`
- [x] Initializer function
- [x] Initializer + behaviors in another file
- [x] Cyclic list defining the behavior

## Behaviors

- [ ] Simple movements
    - [x] Line from a to b (Astar or draw)
    - [x] Rotation of x ± e deg
    - [x] Rotation from a to b ± e deg
- [x] Combine them by modifying `guard->behave` with conditions
- [x] Move towards a fixed guard
- [x] Centered square
- [x] Crowd movement
    - To test:
        - leader with different behavior
        - slightly change values
- [x] Barrier / serpentine movement
    - [x] Choose the right column in `behave`
- [ ] Vary parameters (Rotation_speed, crowd)

## Find rotation speed for snake

- [x] Every 20°
- [x] Then every 2°

- [x] Change parameters for mousaïd
- [ ] Increase quality to find rotation speed for rotation

## Options

- [ ] Obstacles (problem with guard movement): generation with Perlin noise
- [x] Display chosen path
- [ ] Multiple robbers