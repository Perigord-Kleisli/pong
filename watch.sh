#!/usr/bin/env bash

while true; do 
  watch -d -t -g ls -lR "./src" && meson compile -C builddir && ./builddir/pong
  echo -e "\x1b[32mRunning Program\x1b[0m"
  sleep 1
done
