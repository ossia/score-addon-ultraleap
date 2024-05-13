#!/bin/bash
rm -rf release
mkdir -p release

cp -rf UltraLeap *.{hpp,cpp,txt,json} LICENSE release/

mv release score-addon-ultraleap
7z a score-addon-ultraleap.zip score-addon-ultraleap
