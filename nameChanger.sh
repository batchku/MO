#!/bin/bash
mode=$1
search=$2
replace=$3

if [ "$mode" = "test" ]; then
	find . -name '*'$search'*' | while read f; do echo mv "$f" "${f/$search/$replace}"; done
fi
if [ "$mode" = "change" ]; then
	find . -name '*'$search'*' | while read f; do mv "$f" "${f/$search/$replace}"; done
fi