import os
import sys
import shutil
import urllib.request
from zipfile import ZipFile

def debug_files_exist(filePath, filesRequired : list) -> bool :
    filesToRemove = []
    for file in filesRequired : 
        if os.path.isfile(filePath + file) : 
            filesToRemove.append(file)
    for file in filesToRemove : 
        filesRequired.remove(file)
    return len(filesRequired) == 0

def download_url(url, save_path):
    print("Downloading from url...", flush=True)
    
    with urllib.request.urlopen(url) as dl_file:
        with open(save_path, 'wb') as out_file:
            out_file.write(dl_file.read())
            
    print("Finished downloading")

def unzip_debug_file(file_name, libs) :
    libDir = list(libs)
    for i in range(len(libDir)) : 
        libDir[i] = 'lib/' + libDir[i]    
    with ZipFile(file_name, 'r') as zip:
        for name in zip.namelist():
            if name in libDir:
                zip.extract(name)

def move_libs_to_sdk(filePath, libs):
    libsInDir = os.listdir('lib')
    if len(libsInDir) == 0 :
        print("Can't find lib directory")
        return 

    for file in libsInDir :
        if file in libs:
            shutil.move('lib/' + file, filePath + file)
            print('moved file: ' + file + ' to location: ' + filePath)

def get_vulkan_sdk_lib_path(platform):
    vulkanSDKPath = os.environ.get("VULKAN_SDK")

    if vulkanSDKPath is None and platform == 'darwin' :
        vulkanSDKPath = '/usr/local/lib/'
    elif vulkanSDKPath is None :
        print('Could not get path to vulkan libraries')
        return None
    
    if platform == 'win32' : 
        vulkanSDKPath += '/Lib/'
    elif platform == 'linux' :
        vulkanSDKPath += '/x86_64/lib/'

    return vulkanSDKPath

def cleanup_zip(save_path):
    os.remove(save_path)
    print('Removed file ' + save_path)
    if len(os.listdir('lib')) == 0 :
        os.rmdir('lib')
        print('Removed directory lib')

if sys.platform == 'win32': #Windows
    print("Using platform windows")
    url = "https://github.com/KhronosGroup/glslang/releases/download/master-tot/glslang-master-windows-x64-Debug.zip"
    libExtension = '.lib'
elif sys.platform == 'darwin': #macOS
    print("Using platform macOS")
    url = "https://github.com/KhronosGroup/glslang/releases/download/master-tot/glslang-master-osx-Debug.zip"
    libExtension = '.a'
elif sys.platform == 'linux': #Linux
    print("Using platform Linux")
    url = "https://github.com/KhronosGroup/glslang/releases/download/master-tot/glslang-master-linux-Debug.zip"
    libExtension = '.a'
else:
    print("Platform not supported. Exiting program")
    exit()

libNames = [
'GenericCodeGend' + libExtension, 
'glslangd' + libExtension, 
'HLSLd' + libExtension, 
'MachineIndependentd' + libExtension, 
'OGLCompilerd' + libExtension, 
'OSDependentd' + libExtension
]   

vulkanPath = get_vulkan_sdk_lib_path(sys.platform)

filePath = "glslang_debug.zip"

if debug_files_exist(vulkanPath, libNames) :
    print("Files already exist, exiting...")
    exit()

download_url(url, filePath)
unzip_debug_file(filePath, libNames)
move_libs_to_sdk(vulkanPath, libNames)
cleanup_zip(filePath)