# Quick Start

This is a simple quick start guide how to clone my wonderful project and get it running.

1. You can clone the code with this command:

```bash
git clone --recursive --depth 1 https://gitlab2.informatik.uni-wuerzburg.de/GE/Teaching/gl3/projects/2023/27-gl3-odonnell.git
```

2. Once cloned, you can open the root folder in CLion. And load the `CMakeLists.txt` file

3. In CLion go to Settings->Build,Execution,Deployment->Toolchains add the Microsoft Visual Studio Compiler (2019 or 2022) and hit Apply

4. Make sure all the dependencies are in the code/extern/ folder (Box2D, GLAD, GLFW, GLM, SoLoud, Stb_image, Tinygltf, FastNoise).

5. You should now have a running configuration -> compile the code and have fun!