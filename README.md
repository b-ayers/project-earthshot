This is a trimmed down and modified version of our "Project Moonshot" app, 
which was a 2D solar system sim calculating multibody gravity.

This version takes everything into 3 dimensions and focuses on Earth,
exploring the effect of perturbations away from a spherically symmetric potential.
Adding the J2 term due to the oblateness of the Earth results in the precession 
of the orbital plane and apsidal axis. 

3 dimensions is harder to visualize and control the spacecraft in, but necessary
for visualizing things like high inclination sun synchronous orbits.

----Controls----
Tab switches between calculating gravity with the J2 term vs without it.

The camera is controlled via mouse:
--right click drag to zoom
--left click drag to rotate around focal point
--middle click drag to pan

Currently, the spacecraft heading is controlled by:
--left arrow/right arrow increment/decrement the heading along the global azimuthal angle
--up arrow/down arrow increment/decrement the heading along the global polar angle
--space bar applies thrust along the heading vector
Not ideal; may try rotating relative to the spacecraft's local frame of reference.

The time scale the simulation is being calculated on can be sped up or slowed down
using 'a' and 'd'. 