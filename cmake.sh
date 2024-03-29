#!/bin/sh

# 创建build目录
echo -e "\e[1;34mCreating the build directory if it doesn't exist\e[0m"
echo ""
mkdir -p build
echo -e "\e[1;34m----------------------------------------\e[0m"

# 删除build目录的所有内容
echo -e "\e[1;34mDeleting all contents in the build directory\e[0m"
echo ""
rm -rf build/*
echo -e "\e[1;34m----------------------------------------\e[0m"

# 进入build目录
echo ""
echo -e "\e[1;34mEntering the build directory\e[0m"
echo ""
cd build
echo -e "\e[1;34m----------------------------------------\e[0m"

# 运行CMake
echo ""
echo -e "\e[1;34mRunning CMake\e[0m"
echo ""
cmake ..
echo -e "\e[1;34m----------------------------------------\e[0m"

# 获取处理器核心数
CORES=$(nproc)

# 构建项目
echo ""
echo -e "\e[1;34mBuilding the project\e[0m"
echo ""
make -j$CORES
echo -e "\e[1;34m----------------------------------------\e[0m"

# 执行app
echo ""
echo -e "\e[1;34mExecuting the app\e[0m"
echo ""
./app
echo -e "\e[1;34m----------------------------------------\e[0m"
