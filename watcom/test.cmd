@echo off


grep -E -f ..\tests\khadafy.regexp ..\tests\khadafy.lines > khadafy.out
diff khadafy.out ..\tests\khadafy.lines
IF NOT ERRORLEVEL 0 @echo "Khadafy test failed -- output left on khadafy.out"

@del khadafy.out

echo radar >patfile
echo MILES >>patfile
echo GNU   >>patfile

echo "miles" |  grep -i -E -f patfile
IF NOT ERRORLEVEL 0 @echo "File_pattern: Wrong status code, test #1 failed"

echo "GNU" | grep -G -f patfile
IF NOT ERRORLEVEL 0 @echo "File_pattern: Wrong status code, test #2 failed"

echo "ridar" | grep -F -f patfile
IF NOT ERRORLEVEL 1 @echo "File_pattern: Wrong status code, test #3 failed"

echo >patfile

echo "abbcd"h | grep -F -f patfile
IF NOT ERRORLEVEL 0 @echo "File_pattern: Wrong status code, test #4 failed"

@echo "Spencer bre tests - no failed output is passed"
@echo "                    example: Spencer bre test #1 failed"
echo 'abc' | grep -e 'a\(b\)c' >NULL
IF NOT ERRORLEVEL 0 @echo "Spencer bre test #1 failed"

echo 'a(' | grep -e 'a(' >NULL
IF NOT ERRORLEVEL 0 @echo "Spencer bre test #2 failed"

echo 'EPAREN' | grep -e 'a\(' >NULL
IF NOT ERRORLEVEL 2 @echo "Spencer bre test #3 failed"

echo 'EPAREN' | grep -e 'a\(b' >NULL
IF NOT ERRORLEVEL 2 @echo "Spencer bre test #4 failed"

echo 'a(b' | grep -e 'a(b' >NULL
IF NOT ERRORLEVEL 0 @echo "Spencer bre test #5 failed"

echo 'a)' | grep -e 'a)' >NULL
IF NOT ERRORLEVEL 0 @echo "Spencer bre test #6 failed"

echo 'EPAREN' | grep -e 'a\)' >NULL
IF NOT ERRORLEVEL 2 @echo "Spencer bre test #7 failed"

echo 'EPAREN' | grep -e '\)' >NULL
IF NOT ERRORLEVEL 2 @echo "Spencer bre test #8 failed"

echo 'ab' | grep -e 'a\(\)b' >NULL
IF NOT ERRORLEVEL 0 @echo "Spencer bre test #9 failed"

echo 'a$b' | grep -e 'a$b' >NULL
IF NOT ERRORLEVEL 0 @echo "Spencer bre test #10 failed"

