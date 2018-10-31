#!/bin/zsh
for file in $(ls ../test-programs/*.dcf); do
	echo "./parser < $file"
	./parser < $file
done
