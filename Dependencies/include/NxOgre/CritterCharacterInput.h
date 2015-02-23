/** 
    
    This file is part of Critter.
    
    Copyright (c) 2009 Robin Southern, http://www.nxogre.org
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
    
*/

                                                                                       

#ifndef CRITTER_CHARACTERINPUT_H
#define CRITTER_CHARACTERINPUT_H

#include "CritterStable.h"

#if NxOgreHasCharacterController == 1

#include "CritterCommon.h"

                                                                                       

namespace Critter
{

/*struct CharacterInput
{
 // Amount forward (+127 max), or amount backwards (-127 max), or 0 for no forward/backward motion.
 // -127/127 is the maximum walk speed per timestep.
 char forward_backward       : 8;  // 8
 // Amount left (-127 max), or amount right (+127 max), or 0 for no left/right motion.
 // If is turning then -127/127 is the maximum turning angle.
 // If is sidesteping then -127/127 is the maximum walk speed per timestep.
 char left_right             : 8;  // 16
 // If set to 1 then left_right turns the character, than 0 which moves the character sideways.
 unsigned char is_turning    : 1;  // 17
 // Go "up".
 unsigned char up            : 1;  // 18
 // Go "down".
 unsigned char down          : 1;  // 19
 // User properties (to use with a mover).
 unsigned short user         : 13; // 32
};*/

struct CharacterInput
{
 
 // Maximum walk speed.
 short forward_backward     : 16;
 
 // Maximum sidestep speed, or turning value per timestep.
 //                            Where 1 = turning_value
 short left_right           : 16;
 
 // See Enums::TurningMode
 unsigned char turning_mode : 2;
 
 // Go up.
 unsigned char up           : 4;
 
 // Go down
 unsigned char down         : 4;
 
 // User properties.
 unsigned short user        : 16;
 
 // UNKNOWN
 unsigned char  RESERVED     : 6;
 
};

struct CharacterInputHelper
{
 
 CharacterInputHelper();
 
 void reset();
 
 void forward(unsigned short = 2^15);
 
 void forwardFractional(Ogre::Real decimal);
 
 void backward(unsigned short = 2^15);
 
 void backwardFractional(Ogre::Real decimal);
 
 void left(unsigned short = 2^15);
 
 void leftFractional(Ogre::Real decimal);
 
 void right(unsigned short = 2^15);
 
 void rightFractional(Ogre::Real decimal);
 
 void up(bool = true);
  
 void down(bool = true);

 void turn(Ogre::Degree difference, Ogre::Degree maxTurningAngle);
 
 void turn(short = 2^15);
 
 CharacterInput input;
 
};

} // namespace

                                                                                       

#endif
#endif
