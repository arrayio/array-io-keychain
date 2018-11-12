@echo %cd%
dir
candle.exe wix_app_entry.wxs
light.exe -ext WixUIExtension wix_app_entry.wixobj