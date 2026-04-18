#!/usr/bin/sh
cmake -B build -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
cp build/compile_commands.json .
echo "Building Android app"
cd android
gradle -Pandroid.aapt2FromMavenOverride=$ANDROID_HOME/build-tools/35.0.0/aapt2 -PAPP_ID=io.github.iudah.greygreg -PGAME_NAME=greg assembleDebug
cd -
