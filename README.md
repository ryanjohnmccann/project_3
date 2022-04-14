# Group Members: Ryan McCann

#### To run this code, please first extract the tar file:
```tar -xzvf ryan_mccann_project_3.tar.gz```

#### Then, cd into the project:
```cd project_3```

#### If you'd like to just run the program, cd into the proper directory:
```cd cmake-build-debug```

#### And run the program with desired command line arguments:
```./project_3 testin1.dat out1.txt 10```

#### If you'd like to test the cmake file, you must create your own build directory, since the cmake cache has saved my local paths. First, change your current directory to the second level in the project tree i.e., you should be able to see all the source files using the ls command e.g. main.c.

#### Then, create a build directory and cd into it:
```mkdir build```

```cd build```

#### Run cmake:
```cmake ../../project_3/ .``` OR ```cmake . ../../project_3/```

#### Build your binaries:
```cmake --build .```

#### And then run the proper binary with command line arguments:
```./project_3 testin1.dat out1.txt 10```