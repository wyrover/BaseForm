call "BaseWindowsDeconstruct.bat"

cd ..

copy "%cd%\Windows\BaseForm.vcxproj" "%cd%\BaseForm.vcxproj"
copy "%cd%\Windows\BaseForm.vcxproj.filters" "%cd%\BaseForm.vcxproj.filters"
copy "%cd%\Windows\BaseForm.vcxproj.user" "%cd%\BaseForm.vcxproj.user"

cd Windows