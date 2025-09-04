import os
import sys
import shutil
import subprocess
import time

def run_command(command):
    """运行一个系统命令，如果失败则退出脚本"""
    print(f"--- Running command: {' '.join(command)}")
    try:
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
        print(f"--- [ERROR] Command not found: {command[0]}. Is CMake or Ninja installed and in your PATH?")
        sys.exit(1)

def main():
    """主构建函数"""
    start_time = time.monotonic()

    script_dir = os.path.dirname(os.path.realpath(__file__))
    os.chdir(script_dir)
    print(f"--- Switched to project root directory: {script_dir}")

    build_dir = "build"

    # --- [修改点] ---
    # 不再删除 build 文件夹，而是确保它存在。
    # 如果文件夹已存在，os.makedirs(..., exist_ok=True) 不会做任何事。
    # if os.path.exists(build_dir):
    #     print(f"--- Found existing '{build_dir}' directory. Removing it.")
    #     shutil.rmtree(build_dir)
    
    print(f"--- Ensuring '{build_dir}' directory exists for incremental build.")
    os.makedirs(build_dir, exist_ok=True)
    # --- [修改结束] ---

    build_type_arg = []
    if "--debug" in sys.argv:
        print("--- Debug mode selected. Will pass '-DCMAKE_BUILD_TYPE=Debug' to CMake.")
        build_type_arg = ["-DCMAKE_BUILD_TYPE=Debug"]
    else:
        print("--- Release mode selected (default). Optimization flags will be set by CMakeLists.txt.")

    cmake_configure_command = [
        "cmake",
        "-S", ".",
        "-B", build_dir,
        "-G", "Ninja",
        "-DCMAKE_COLOR_DIAGNOSTICS=ON"
    ] + build_type_arg
    
    run_command(cmake_configure_command)

    cmake_build_command = [
        "cmake",
        "--build", build_dir
    ]
    run_command(cmake_build_command)

    end_time = time.monotonic()
    duration = end_time - start_time

    print("\n--- [SUCCESS] Build completed successfully with Ninja!")
    print(f"--- Total build time: {duration:.2f} seconds.")
    print(f"--- The executable is located in the '{build_dir}' directory.")

if __name__ == "__main__":
    main()