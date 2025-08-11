import os
import sys
import shutil
import subprocess

def run_command(command):
    """运行一个系统命令，如果失败则退出脚本"""
    print(f"--- Running command: {' '.join(command)}")
    try:
        # 移除了 capture_output=True，让编译器的输出实时显示在终端上，体验更好
        process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, encoding='utf-8', errors='replace')
        while True:
            output = process.stdout.readline()
            if output == '' and process.poll() is not None:
                break
            if output:
                print(output.strip())
        
        rc = process.poll()
        if rc != 0:
            print(f"--- [ERROR] Command failed with exit code {rc}")
            sys.exit(rc)

    except FileNotFoundError:
        print(f"--- [ERROR] Command not found: {command[0]}. Is CMake installed and in your PATH?")
        sys.exit(1)

def main():
    """主构建函数"""
    # 1. 切换到脚本所在的目录，以确保路径正确
    script_dir = os.path.dirname(os.path.realpath(__file__))
    os.chdir(script_dir)
    print(f"--- Switched to project root directory: {script_dir}")

    build_dir = "build"

    # 2. 如果build目录存在，则先删除
    if os.path.exists(build_dir):
        print(f"--- Found existing '{build_dir}' directory. Removing it.")
        shutil.rmtree(build_dir)
    
    # 3. 确定构建类型 (默认Release, 可选Debug)
    build_type = "MinSizeRel" # 默认为Release模式 (对应-Os -s)
    if "--debug" in sys.argv:
        build_type = "Debug"
        print("--- Debug mode selected via --debug flag.")
    else:
        print("--- Release mode selected (MinSizeRel for smallest size, default).")

    # 4. 创建新的build目录
    print(f"--- Creating new '{build_dir}' directory.")
    os.makedirs(build_dir)

    # 5. 定义并执行CMake配置命令
    cmake_configure_command = [
        "cmake",
        "-S", ".",          # -S 指定源代码根目录 (当前目录)
        "-B", build_dir,    # -B 指定构建目录
        f"-DCMAKE_BUILD_TYPE={build_type}" # 指定构建类型
    ]
    run_command(cmake_configure_command)

    # 6. 定义并执行CMake编译命令
    cmake_build_command = [
        "cmake",
        "--build", build_dir
    ]
    run_command(cmake_build_command)

    print("\n--- [SUCCESS] Build completed successfully!")
    print(f"--- The executable is located in the '{build_dir}' directory.")

if __name__ == "__main__":
    main()