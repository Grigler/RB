[![Build Status](https://travis-ci.com/Grigler/RB.svg?branch=master)](https://travis-ci.com/Grigler/RB) [![Coverage Status](https://coveralls.io/repos/github/Grigler/RB/badge.svg?branch=master)](https://coveralls.io/github/Grigler/RB?branch=master)

# RB
RigidBody physics implemented with NGL, and TravisCI with gtest

## Physics Features
- User-Extendable Factory implementations for Integration and LCP solvers - implemented include:
  - Integrators
    - Forward Euler (Explicit Euler)
    - Semi-Implicit Euler
  - LCP Solvers
    - Sequential Impulse (SI)
- 2 Collider types:
  - Half-extend defined oriented boxes
  - Radius defined spheres
- Static bodies - via infinite mass representation
- Fixed timestep - with variable sub-stepping
- AABBs broadphase collision step
  - Originally intended for use within a BVH - time restrictions resulted in a brute-force method being used in its place 

## OpenGL Demo Features
- Simple Scene structure - with templated object instantiation
- 2 Demo Scenes
  - Plinko Scene (default scene)
  - Box Scene (can be accessed via the command-line argument "box")
- Keyboard/Mouse controlled first-person camera
- Ability to launch spheres in direction of camera
- Ability to pause physics updates, while continuing to survey the scene with the camera
- Gamma correction as a post-process
  - Facility for HDR (relevant lines commented out within the shader itself - due to preference of visual result)

## Testing
Initially this project was set out with the intention of following through with TDD - however as deadlines loomed and time dwindled, it became clear that the slower (albeit more methodical and logical) process was too time consuming for this kind of project. While a physics library is admitedly much more suited to TDD than most real-time graphics applications, due to its grounding in replicating known responses to simulated actions, it is a lengthy process to cover all cases. To this end, there is the "build failing" mark of shame on the project due to its failing in tests - although it should be seen that the CI building and code coverage were implemented early into the project and looked into greatly as it began.

### Control Scheme
Control Key | Action
-----------|--------
 W,A,S,D | Camera movement - relative to its orientation
 Mouse Movement | Camera Rotation
 F | Fires a single sphere (with a mass of 10kg) in the direction of the camera
 I/O | Switches between ForwardEuler and SemiImplicitEuler integrators, respectively
 Backspace | Toggles AABB bounding volume rendering - can be intensive with high numbers of bodies as this is not an instanced render (as it truly should be)
 Spacebar | Pauses/Unpauses physical simulation - the clock still runs and allows the camera to move around the scene and view a snapshot of the simulation
 Escape | Closes the program in a controlled manner (closing the window will also do this)

### Demo Scenes
#### Plinko Scene
![Plinko Scene](https://i.imgur.com/n4WqJVe.png)
##### [Video can be seen here](https://youtu.be/IZ3tUonwZmM)

This scene consist of a collection of boxes in an arrangement similar to a plinko board. A series of boxes provides the visible green boundaries, as well as a non-visible infront of the 'pins' (mimicing glass). Spheres are dropped into the scene from a moving emitter above the board at a constant rate, allowing for a simple and steady stress test for how the system handles dense collisions between multiple static/infinite mass objects.


#### Box Scene
![Box Scene](https://i.imgur.com/1hM4A1j.png)
##### [Video can be seen here](https://youtu.be/AjiuwR5IGB4)

The box scene consists of an open-topped, thick-walled box and the ability for the camera to launch spheres into the box. This demo scene proved useful in testing the stability of the implemented integrators, due to the highly constrained nature of a box full of spheres. If Forward-euler is switched to ('I' key), the system soon becomes unstable and explosive - as is to be expected.
