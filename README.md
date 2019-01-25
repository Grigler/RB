[![Build Status](https://travis-ci.com/Grigler/RB.svg?branch=master)](https://travis-ci.com/Grigler/RB) [![Coverage Status](https://coveralls.io/repos/github/Grigler/RB/badge.svg?branch=master)](https://coveralls.io/github/Grigler/RB?branch=master)

# RB
RigidBody physics implemented with NGL, and TravisCI with gtest 
(CGIT relevant sections can be seen at the end)

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

## CGIT Reference Section
### Use and implementation of appropriate CGI Techniques
- 3D Transformations
  - A collection of 3D transformations and supporting can be found in the Transformation class, as well as the Camera class. This includes the some code to allow for the easy extension of a transformation heirarchy, whereby the parents' model matrices would be retrieved and used as the initial basis for the relevent object's transformations to be applied (a sidenote that caching these variables would invariably optimise this process by removing uneeded calculations, especially in large or complex heirarchies). Due to the lack
- Gamma Correction
  - Despite the lack of lighting in the demonstration application, the use of gamma correction allowed for the harshness of flat shading to be evened out across the image into a more aesthetically pleasing image. This was implemented trivially through a post-process shader that rose the final image's colour to `1/gamma`, where `gamma = 2.2`.

### Project Specific
- Iterative LCP Solver
  - Sequential impulse (a derivative of the Projected Gauss-Seidel, or PGS, algorithm) was implemented to resolve the constraints formed from rigidbody collisions. This can be found in the SISolver class. I would argue that this implementation was successful in being both well optimised and stable, helped in part by the use of Baumgarte Stabilisation to allow for linear position projection to influence the lambda variable, whereby it can be described as 80% of interpenetration is attempted to be removed through linear projection per physics update.
- SAT Testing
  - SAT testing was attempted for finding collisions between both Sphere-OBB and OBB-OBB collision pairs, while this worked well for Sphere-OBB pairs (shown throughout the demo scenes) there was a shortcoming in the OBB-OBB case. In this, there were issues found in implementing this test beyond the point of simply finding whether a collision has occured, but in forming this known collision case into a series of constraints or some constraint manifold. Due to this OBBs have been used throughout the demo as static geometry for which the sphere bodies interact with as scene collision geometry, a purpose for which they are well suited in their level of implementation.
- AABB BVH
  - While an initial attempt was made in implementing an AABB BVH was made, the complexity in maintaining the graph resulted in a brute-force solution to be used as what was initially intended a temporary fallback. As can be seen in the BVH classes, there are a number of implemented recursive functions for testing collisions against the tree. As more time than originally intended was spent on other aspects of this project, namely the SI solver and attempted SAT implementation, this was put to the side as a "good-enough" solution for now. Although there is an obvious improvement to be made, even if only through simpler methods of spatial partitioning such as through Octrees.
