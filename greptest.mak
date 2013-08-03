

GREP = grep.exe
DIFF = d:\moztools\diff

all: options

options: .SYMBOLIC
  $(GREP) -E -f tests\khadafy.regexp tests\khadafy.lines > khadafy.out
  $(DIFF) khadafy.out tests\khadafy.lines
  @IF NOT ERRORLEVEL 0 @echo Failure options test 1
#  echo "abababccccccd" | $(GREP) -G -e 'c\\{3\\}'
#  @IF NOT ERRORLEVEL 0 @echo Failure options test 2
#  echo "abababccccccd" | $(GREP) -F -e 'c\{3\}'
#  @IF NOT ERRORLEVEL 1 @echo Failure options test 3
