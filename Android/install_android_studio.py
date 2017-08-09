#!/usr/bin/python
# install_android_studio.py

import sys
import os, os.path
import shutil

context = {
    "project_name": "undefined",
    "project_path": "undefined",
    "package_name": "undefined",
    "app_id": "undefined",
    "app_key": "undefined",
    "app_secret": "undefined",
    "script_dir": "undefined"
}


'''
print the usage of cocos2dx-getui's install_android_studio.py script
'''


def dump_usage():
    print "Usage: ./install_android_studio.py -project PROJECT_NAME -package PACKAGE_NAME -appId APP_ID -appKey " \
          "APP_KEY -appSecret APP_SECRET "
    print "Options:"
    print "  -project    Project name, for example: MyGame"
    print "  -package    Package name, for example: com.getui.mygame"
    print "  -appId      APP ID for GeTui portal "
    print "  -appKey     App Key for GeTui portal "
    print "  -appSecret  APP SECRET for GeTui portal "
    print "Sample : ./install_android_studio.py -project MyGame -package com.getui.mygame  -appId xxxxxxx -appKey " \
          "xxxxxxx -appSecret xxxxxxx "
# end of dumpUsage


def init_param():
    context["script_dir"] = os.getcwd() + "/"

    # invalid invoke params
    if len(sys.argv) < 11:
        dump_usage()
        sys.exit()

    # find params
    for i in range(1, len(sys.argv)):
        param = sys.argv[i]
        if "-project" == param:
            context["project_name"] = sys.argv[i + 1]
            context["project_path"] = os.getcwd() + "/../../../../../";
        elif "-package" == param:
            context["package_name"] = sys.argv[i + 1]
        elif "-appId" == param:
            context["app_id"] = sys.argv[i + 1]
        elif "-appKey" == param:
            context["app_key"] = sys.argv[i + 1]
        elif "-appSecret" == param:
            context["app_secret"] = sys.argv[i + 1]

    # print error log required paramters are not ready
    raise_error = False
    if context["project_name"] == "undefined":
        print "Invalid -project parameter"
        raise_error = True
    elif context["package_name"] == "undefined":
        print "Invalid -package parameter"
        raise_error = True
    elif context["app_id"] == "undefined":
        print "Invalid -appId parameter"
        raise_error = True
    elif context["app_key"] == "undefined":
        print "Invalid -appKey parameter"
        raise_error = True
    elif context["app_secret"] == "undefined":
        print "Invalid -appSecret parameter"
        raise_error = True

    if raise_error:
        sys.exit()
# end of initParam

'''
copy getui-cocos2dx plugin needs code files to project
'''


def copy_code_to_project():
    print "copy code files to project"
    # copy .h file
    h_file_source = context["script_dir"] + "/GTPushBridge.h"
    h_file_target = context["project_path"] + "/Classes/GTPushBridge.h"
    h_file_target_path = context["project_path"] + "/Classes/"
    if not os.path.exists(h_file_target_path):
        os.makedirs(h_file_target_path)
    elif os.path.exists(h_file_target):
        os.remove(h_file_target)
    shutil.copy(h_file_source, h_file_target)

    # copy .cpp file
    cpp_file_source = context["script_dir"] + "/GTPushBridge.cpp"
    cpp_file_target = context["project_path"] + "/proj.android-studio/app/jni/getui/GTPushBridge.cpp"
    cpp_file_target_path = context["project_path"] + "/proj.android-studio/app/jni/getui/"
    if not os.path.exists(cpp_file_target_path):
        os.makedirs(cpp_file_target_path)
    elif os.path.exists(cpp_file_target):
        os.remove(cpp_file_target)
    shutil.copy(cpp_file_source, cpp_file_target)

    # copy java file
    java_push_service_source = context["script_dir"] + "/GTPushService.java"
    java_push_intent_service_source = context["script_dir"] + "/GTPushIntentService.java"
    java_push_helper_source = context["script_dir"] + "/GTPushHelper.java"
    java_push_path = context["project_path"] + "/proj.android-studio/app/src/"
    package_paths = context["package_name"].split(".")

    for path in package_paths:
        java_push_path = java_push_path + path + "/"
    java_push_path += "getui" + "/"
    if not os.path.exists(java_push_path):
        os.makedirs(java_push_path)

    java_push_service_target = java_push_path + "GTPushService.java"
    java_push_intent_service_target = java_push_path + "GTPushIntentService.java"
    java_push_helper_target = java_push_path + "GTPushHelper.java"

    if os.path.exists(java_push_service_target):
        os.remove(java_push_service_target)
    shutil.copy(java_push_service_source, java_push_service_target)

    if os.path.exists(java_push_intent_service_target):
        os.remove(java_push_intent_service_target)
    shutil.copy(java_push_intent_service_source, java_push_intent_service_target)

    if os.path.exists(java_push_helper_target):
        os.remove(java_push_helper_target)
    shutil.copy(java_push_helper_source, java_push_helper_target)
# end of copy_code_to_project

'''
write code to android.mk
'''


def write_code_to_androidmk():
    print "write code to android.mk"
    mk_target = context["project_path"] + "/proj.android-studio/app/jni/Android.mk"
    mk_file = file(mk_target)

    # read Android.mk
    lines = []
    for line in mk_file:
        lines.append(line)
    mk_file.close()

    # include cocos jni
    is_include_cocos_jni = False
    for line in lines:
        if line.find('$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/platform/android/jni)') >= 0:
            is_include_cocos_jni = True
            break

    if not is_include_cocos_jni:
        i = 0;
        for line in lines:
            i += 1
            if line.find('$(call import-add-path') >= 0:
                lines.insert(i, "$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/platform/android/jni) \n")
                break

    # compile cpp file
    is_cpp_exist = False
    for line in lines:
        if line.find("GTPushBridge.cpp") >= 0:
            is_cpp_exist = True
            break
    if not is_cpp_exist:
        i = 0
        for line in lines:
            i += 1
            if line.find("LOCAL_SRC_FILES") >= 0:
                lines.insert(i, "                   getui/GTPushBridge.cpp \\\n")
                break
    mk_file = file(mk_target, 'w')
    for line in lines:
        mk_file.write(line)
    mk_file.close()
# end of copy_code_to_androidmk

'''
write code to build.gradle
'''


def write_code_to_gradle():
    print "write code to build.gradle"
    application_build_gradle_target = context["project_path"] + "/proj.android-studio/build.gradle"
    application_build_gradle_file = file(application_build_gradle_target)

    # read proj.android-studio/build.gradle
    lines = []
    for line in application_build_gradle_file:
        lines.append(line)
    application_build_gradle_file.close()

    # add repository
    is_repository_exist = False
    for line in lines:
        if line.find('url "http://mvn.gt.igexin.com/nexus/content/repositories/releases/"') >= 0:
            is_repository_exist = True
            break
    if not is_repository_exist:
        is_all_projects_searched = False
        i = 0
        for line in lines:
            i += 1
            if line.find("allprojects") >= 0:
                is_all_projects_searched = True
            if line.find("jcenter()") >= 0 and is_all_projects_searched:
                lines.insert(i, '        maven {\n')
                i += 1
                lines.insert(i, '            url "http://mvn.gt.igexin.com/nexus/content/repositories/releases/"\n')
                i += 1
                lines.insert(i, '        }\n')
                break
    application_build_gradle_file = file(application_build_gradle_target, 'w')
    for line in lines:
        application_build_gradle_file.write(line)
    application_build_gradle_file.close()

    # add meta-data and dependency
    app_build_gradle_target = context["project_path"] + "/proj.android-studio/app/build.gradle"
    app_build_gradle_file = file(app_build_gradle_target)

    # read proj.android-studio/app/build.gradle
    lines = []
    for line in app_build_gradle_file:
        lines.append(line)
    app_build_gradle_file.close()

    is_app_id_exist = False
    is_dependency_exist = False
    for line in lines:
        if line.find("GETUI_APP_ID") >= 0:
            is_app_id_exist = True
        if line.find("com.getui:sdk") >= 0:
            is_dependency_exist = True
        if is_app_id_exist and is_dependency_exist:
            break

    if not is_app_id_exist:
        i = 0
        for line in lines:
            i += 1
            if line.find("manifestPlaceholders") >= 0:
                lines.insert(i, '                GETUI_APP_ID : "' + context["app_id"] + '",\n')
                i += 1
                lines.insert(i, '                GETUI_APP_KEY : "' + context["app_key"] + '",\n')
                i += 1
                lines.insert(i, '                GETUI_APP_SECRET : "' + context["app_secret"] + '"\n')
                is_app_id_exist = True
                break

    if not is_app_id_exist:
        i = 0
        for line in lines:
            i += 1
            if line.find("versionName") >= 0:
                lines.insert(i, '        manifestPlaceholders = [\n')
                i += 1;
                lines.insert(i, '                GETUI_APP_ID : "' + context["app_id"] + '",\n')
                i += 1
                lines.insert(i, '                GETUI_APP_KEY : "' + context["app_key"] + '",\n')
                i += 1
                lines.insert(i, '                GETUI_APP_SECRET : "' + context["app_secret"] + '"\n')
                i += 1
                lines.insert(i, '        ]\n')
                break

    if not is_dependency_exist:
        i = 0
        for line in lines:
            i += 1
            if line.find("dependencies {") >= 0:
                lines.insert(i, "    compile 'com.getui:sdk-for-google-play:2.10.3.5'\n")
                break

    app_build_gradle_file = file(app_build_gradle_target, "w")
    for line in lines:
        app_build_gradle_file.write(line)
    app_build_gradle_file.close()
# end of write_code_to_gradle

'''
write code to AndroidManifest.xml
'''


def write_code_to_manifest():
    print "write code to AndroidManifest.xml"
    data = """        <!-- Getui Config-->
        <service android:name="org.cocos2dx.cpp.getui.GTPushService"
            android:exported="true"
            android:label="PushService"
            android:process=":pushservice"/>

        <service android:name="org.cocos2dx.cpp.getui.GTPushIntentService"/>
    """
    app_manifest_target = context["project_path"] + "/proj.android-studio/app/AndroidManifest.xml"
    app_manifest_file = file(app_manifest_target)

    lines = []
    for line in app_manifest_file:
        if line.find("Getui Config") >= 0:
            return
        lines.append(line)
    app_manifest_file.close()

    i = 0
    for line in lines:
        if line.find("<activity") >= 0:
            lines.insert(i, data + "\n")
            break
        i += 1

    app_manifest_file = file(app_manifest_target, "w")
    for line in lines:
        app_manifest_file.write(line)
    app_manifest_file.close()
# end of write_code_to_manifest

'''
write init push code to AppActivity
'''


def write_code_to_main_activity():
    print "write init push code to AppActivity"
    app_activity_target = context["project_path"] + "/proj.android-studio/app/src/"
    package_paths = context["package_name"].split(".")
    for path in package_paths:
        app_activity_target = app_activity_target + path + "/"
    app_activity_target += "AppActivity.java"
    app_activity_file = file(app_activity_target)

    lines = []
    for line in app_activity_file:
        if line.find("GTPushHelper.init") >= 0:
            app_activity_file.close()
            return
        lines.append(line)
    app_activity_file.close()

    i = 0
    for line in lines:
        i += 1
        if line.find("package " + context["package_name"]) >= 0:
            lines.insert(i, "\nimport " + context["package_name"] + ".getui.GTPushHelper;\n")
            break

    is_import_bundle = False
    for line in lines:
        if line.find("import android.os.Bundle;") >= 0:
            i += 1
            is_import_bundle = True
            break
    if not is_import_bundle:
        i = 0
        for line in lines:
            if line.find("package " + context["package_name"]) >= 0:
                lines.insert(i, "import android.os.Bundle;\n")
                break

    i = 0
    is_init_push_inserted = False
    for line in lines:
        i += 1
        if line.find("super.onCreate") >= 0:
            lines.insert(i, "GTPushHelper.init(getApplicationContext());\n")
            i += 1
            lines.insert(i, "GTPushHelper.initPush();\n")
            is_init_push_inserted = True
            break

    if not is_init_push_inserted:
        i = 0
        for line in lines:
            i += 1
            if line.find("extends Cocos2dxActivity") >= 0:
                data = """\n    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        GTPushHelper.init(getApplicationContext());
        GTPushHelper.initPush();
    }\n"""
                lines.insert(i, data)
                break

    app_activity_file = file(app_activity_target, "w")
    for line in lines:
        app_activity_file.write(line)
    app_activity_file.close()
# end of write init push code to AppActivity


# --------------------main-----------------------
init_param()
copy_code_to_project()
write_code_to_androidmk()
write_code_to_gradle()
write_code_to_manifest()
write_code_to_main_activity()
print "GeTui SDK installed successfullfy!"
