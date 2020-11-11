# DGMM_2021
Transform parameters : 

RigidTransform (int a, int b, int c, int t1x, int t1y, int t2x, int t2y);

with sin=a/c, cos=b/c, tx=t1x/t1y and ty=t2x/t2y;

=> RigidTransform t(3,4,5,1,5,0,1);

=> RigidTransform t(3,4,5,1,5,1,4);
