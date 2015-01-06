cd ..

copy /Y "%cd%\BaseForm.vcxproj" "%cd%\Windows\BaseForm.vcxproj"
copy /Y "%cd%\BaseForm.vcxproj.filters" "%cd%\Windows\BaseForm.vcxproj.filters"
copy /Y "%cd%\BaseForm.vcxproj.user" "%cd%\Windows\BaseForm.vcxproj.user"

del "BaseForm.vcxproj"
del "BaseForm.vcxproj.filters"
del "BaseForm.vcxproj.user"
del "BaseCompile.bash"
del "BaseForm"

cd Windows
