@echo off
cd server
start fcserver /a
start fcauthserver /a
start fcworldserver /a
@rem start fcchatserver /a
cd ..
