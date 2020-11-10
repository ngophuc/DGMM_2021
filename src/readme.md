# Example of generating images with/without a rigid motion

## Disks
- disk of center (0,0) and radius 5 with rotation (3,4,5) and translation (1/5,0) (**disk5.pgm** and **disk5_moved.pgm**):
`
./digitizeRationalDisk 0 1 0 1 5 1 3 4 5 1 5 0 1 disk5.pgm
`
- disk of center (0,0) and radius 23/2 with rotation (3,4,5) and translation (1/5,0) (**disk11_5.pgm** and **disk11_5_moved.pgm**) :
`
./digitizeRationalDisk 0 1 0 1 23 2 3 4 5 1 5 0 1 disk11_5.pgm
`
## Ellipses
- ellipse of center (0,0), width 10 and height 6 with rotation (3,4,5) and translation (1/5,0) (**ellipse10_6.pgm** and **ellipse10_6_rot3-4-5.pgm**) :
`
 ./digitizeRationalEllipse 0 1 0 1 10 1 6 1 3 4 5 1 5 0 1 ellipse10-6.pgm
 `
- ellipse of center (0,0), width 10 and height 6 with rotation (24,7,25) and translation (1/5,0) (**ellipse10_6.pgm** and **ellipse10_6_rot24-7-25.pgm**) :
`
./digitizeRationalEllipse 0 1 0 1 10 1 6 1 24 7 25 1 5 0 1 ellipse10-6.pgm
`
