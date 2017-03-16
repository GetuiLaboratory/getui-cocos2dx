# getui-cocos2dx

个推官方提供的 Cocos2d-x插件 （目前仅支持Android，iOS版本近期将会发布）。

## Android Studio 自动集成
目前自动集成的方式仅支持Android Studio，Eclipse ADT 方式请参考手动集成

- 将`getui-cocos2dx`文件夹放置于 `YourProjectName/cocos2d/plugin/glugins/` 目录下

- 使用命令行工具，进入插件目录 `getui-cocos2dx/Android/` 执行自动安装脚本

			./install_android_studio.py -project PROJECT_NAME -package PACKAGE_NAME -appId APP_ID -appKey APP_KEY -appSecret APP_SECRET

 	显示 `GeTui SDK installed successfullfy!` 则安装成功！


- 由于cocos2dx的版本不同，可能导致工程目录不同，若因为工程目录不同导致自动安装失败时，可尝试手动集成（自动集成需要的目录如下）


			  ┬ YourProjectName/
			  ├── /Classes/
			  ├─┬ /proj.android/
			  │ ├── /jni/
			  │ ├── /libs/
			  │ └── /src/
			  ├─┬ /proj.android-studio/app/
			  | ├── /jni/
			  | ├── /libs/
			  | └── /src/
			  └─┬ /cocos2d
			  	├── plugin/plugins/getui-cocos2dx/Android/


## Android Studio 手动集成（待续）

## Android 手动集成（待续）
