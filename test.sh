#!/bin/bash

function run_test() {
	entry=$1
	
	name=`basename $entry .qk`
	echo "$name"
	
	javac "./test/java/Test_$name.java" -d .
	build/jcp $entry
	
	diff <(java "Test1") <(java "Test_$name")

	if [[ $? == 0 ]] ;then
		echo "Pass"
	else
		echo "Fail"
		echo ""
		echo "Expected: "
		echo "`java Test_$name`"
		echo "Actual:"
		echo "`java Test1`"
		exit 1
	fi
	
	echo ""
}

echo "Testing..."
echo ""

for entry in "./test/quik"/*.qk
do
	run_test $entry
done

rm *.class

echo "Done"
