
# Grime

![Imgur](https://i.imgur.com/MHlEHl0.png)
Grime is a graph generator and renderer inspired by Desmos graphing tool.

## Build

To get started, clone the repository. Ensure that the following packages are installed on your system: **Clang**, **Make**, and **CMake**. Then, navigate to the repository directory and run the script "**./runner.sh**". You'll find the executable located at **bin/Grime**.

**Note:** When executing, ensure to run **./Grime** directly from the **bin/** directory, rather than using the command **bin/Grime**.

Additionally, you can pass any argument to **runner.sh** to build and execute the application. For instance, **./runner.sh 1** 

## Usage

After successfully running the application, navigate to the **Hierarchy** window and click **Add New Graph**. Select the newly created graph in the hierarchy window, then set your equation in the **Properties** window and click **Submit**. Any unknown variables will be added as sliders in the Properties window.

**Note:** If the application becomes **laggy**, consider increasing the increment in the **Settings** tab, under **Graph Settings**.


## Dependencies

- **OpenGL**
  - glfw
  - glad
- **Math**
  - glm
- **Equation Evaluation**
  - tinyexpr_plusplus
- **GUI**
  - ImGui



