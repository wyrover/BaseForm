#!/bin/bash

VAR_ARG="g++ -o BaseForm -I ../CryptoGateway -I ../glGraphics -I ../WifiRC BaseMain.cpp baseForm.cpp basePopUps.cpp inetForm.cpp propertiesForm.cpp USBGenerationForm.cpp"

if [[ "$OSTYPE" == "darwin"* ]]; then
    VAR_ARG="$VAR_ARG -pthread -framework OpenGL -framework GLUT"
else
    VAR_ARG = "$VAR_ARG -pthread -lGL -lglut -lGLU"
fi
eval $VAR_ARG