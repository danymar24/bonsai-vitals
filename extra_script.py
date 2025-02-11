Import("env")
import os
import platform

def before_build_spiffs(source, target, env):
    print("Building React App...")
    print("platform: %s", platform.platform())
    env.Execute("cd web && npm run build")
    print("React App built!")

    if "windows" not in platform.platform().lower():
        print("Removing old SPIFFS image...")
        env.Execute("rm -rf data")
        print("Copying React App to SPIFFS...")
        env.Execute("cp -r web/build data")    
    else:
        print("Removing old SPIFFS image...")
        env.Execute(r"rmdir /s /q data")
        env.Execute(r"del web\\build\\static\\css\\main.*.css.LICENSE.txt web\build\static\\css\\main.txt")
        env.Execute(r"del web\\build\\static\\js\\main.*.js.LICENSE.txt web\\build\\static\\js\\main.txt")
        env.Execute(r"del web\\build\\static\\css\\*.map")
        env.Execute(r"del web\\build\\static\\js\\*.map")
        print("Copying React App to SPIFFS...")
        env.Execute(r"xcopy /s web\\build\\* data\\") 


env.AddPreAction("$BUILD_DIR/spiffs.bin", before_build_spiffs)