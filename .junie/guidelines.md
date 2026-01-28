### Project Overview
- **Project Name:** theademo
- **Language:** C++ (Standard: 20)
- **Build System:** CMake (Minimum Version: 4.1)
- **Toolchain:** MinGW (local)

### Project Structure
- `main.cpp`: Entry point of the application.
- `CMakeLists.txt`: CMake configuration file.
- `product_definition.md`: Detailed requirements and features of the application.
- `.junie/`: Contains project-specific guidelines and configuration for Junie.

### Development Guidelines
- Follow C++20 standards.
- Maintain existing naming conventions and code style.
- Use CMake for building and managing dependencies.
- Ensure all changes are compatible with the MinGW toolchain used in the "Debug" profile.

### Build and Run Instructions
To build the project using the active CMake profile:
```powershell
cmake --build cmake-build-debug --target theademo
```

To run the application:
```powershell
.\cmake-build-debug\theademo.exe
```
