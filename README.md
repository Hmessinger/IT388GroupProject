# IT388GroupProject
# By: Suhail, Hogan, Angel, and Derek
# Parallelizing the Trapping Rainwater Problem
We will be implementing a parallelized version of the trapping rainwater problem. The problem consists of finding the maximum amount of rainwater that can be trapped between two solid surfaces without overflowing. The solid walls are represented by arrays, where the values in the array are equal to the wall heights. The rainwater contained is the open space between two solid surfaces, and the total rainwater trapped is the sum of all of the regions of trapped rainwater.

We decided to work on three different approaches to  the trapping rainwater problem. We worked on a naive approach, prefix and suffix approach, and a two pointer approach. We decided to paralelize these aproaches using omp and mpi. For all of these approaches, we have a sequential file, an omp file, and an mpi file.  
