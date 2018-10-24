# RB [![Build Status](https://travis-ci.com/Grigler/RB.svg?branch=master)](https://travis-ci.com/Grigler/RB)
RigidBody physics implemented with NGL, and TravisCI with gtest

# Test Cases 

  - World Class
    - Limits
      - Adding Large number of bodies
      - Adding zero bodies
    - Behaviour
      - Adding body returns valid pointers
      - Killing world results in invalidated pointers
     
  - Body
    - Limits
      - Attempting to spawn body at large 
    
  - AABB
    - Limits
      - Known pass-case at high floating point values
      - Known fail-case at high floating point values
      - Known pass-case at negative extremity
      - Known fail-case at negative extremity
      - Known pass-case at high relative distance (from cent.)
      - Known fail-case at high relative distance      
    - Behaviour
      - Known pass-case
      - Known fail-case
      - Testing two identical AABBs
      - Testing two closely passing AABBs (within 0.000....01f)
      - Series of 4 90degree rotations results in identical AABB
    
  - BVH
    - Limits
      - Adding Large number of bodies
      - Attempting traversal with no bodies
    - Behaviour
      - Clearing tree results in no invalidated AABB ptrs in body
      - Killing bodies results in invalidated ptrs in BVH
      - Attempting known pass-case AABB test with arbitrary sized trees
      - Attempting known fail-case AABB test with arbitrary sized trees
      - Testing Collision against Body's own AABB
   