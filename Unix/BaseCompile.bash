#!/bin/bash

g++ -o BaseForm -I ../CryptoGateway -I ../glGraphics -I ../WifiRC -pthread BaseMain.cpp baseForm.cpp basePopUps.cpp inetForm.cpp propertiesForm.cpp USBGenerationForm.cpp -lGL -lglut -lGLU