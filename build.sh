#!usr/bin/sh
cmake -B build -GNinja 
cmake --build build
cd android
gradle -Pandroid.aapt2FromMavenOverride=$ANDROID_HOME/build-tools/35.0.0/aapt2 clean assembleDebug
cd -
