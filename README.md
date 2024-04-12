# R_ATXcore

Robust_AdvancedThread core

A simple data core in C++ to simplify data handling and processing.

Basic overview
 - Multithreaded
 - _baseContainter is the base class
 - _task : public _baseContainer is a container to hold code to be multithreaded
 - _data : public _baseContainer is a storage class for data
 - As Fast As Possible
 - 2 Stacks; data (array of _data) and processing (queue of _task)
