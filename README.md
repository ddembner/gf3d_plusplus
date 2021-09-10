# gf3d++
a c++ engine using vulkan with the primary focus on simplicity for educational purposes. 

# Build Process
Currently the only required SDK for this project is Vulkan, which can be found [here](https://vulkan.lunarg.com/sdk/home).
```
NOTE: For Windows users the sdk by default doesn't include the debug libraries needed to compile debug builds. 
You will have to download them along with the sdk and extract them where the sdk libraries are.
```

This project will be using the Cmake build system to manage multiplatform support.

1. Obtain the code: `git clone <repo name>`
2. Get the submodules: `git submodule update --init --recursive`
3. Generate build files `cmake <path to put build>`
