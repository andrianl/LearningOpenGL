#!/bin/bash

# Перевірте, чи передані аргументи для конфігурації
if [ $# -ne 1 ]; then
  echo "Usage: $0 {Debug|Release}"
  exit 1
fi

CONFIGURATION=$1

# Перевірте, чи передано допустиме значення конфігурації
if [[ "$CONFIGURATION" != "Debug" && "$CONFIGURATION" != "Release" ]]; then
  echo "Invalid configuration: $CONFIGURATION"
  echo "Usage: $0 {Debug|Release}"
  exit 1
fi

# Виконання Premake для генерації Makefile
echo "Generating Makefile for configuration: $CONFIGURATION"
premake gmake2

# Перевірте, чи генерація Makefile пройшла успішно
if [ $? -ne 0 ]; then
  echo "Failed to generate Makefile"
  exit 1
fi

# Виконання make для компіляції
echo "Building project with configuration: $CONFIGURATION"
make CONFIGURATION=$CONFIGURATION

# Перевірте, чи компіляція пройшла успішно
if [ $? -ne 0 ]; then
  echo "Build failed"
  exit 1
fi

echo "Build completed successfully for configuration: $CONFIGURATION"
