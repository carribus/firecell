@echo off
tools\genstringh -wide client\clientdata\strings_en.xml ..\..\Clients\FCClient\clientstrings.h
tools\genstringh client\clientdata\strings_en.xml ..\..\Clients\FCClient_Qt\clientstrings.h
