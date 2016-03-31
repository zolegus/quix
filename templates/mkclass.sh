#!/bin/sh

mkdir -p "../src/${1}/"
cat __TEMPLATE__.cc  | sed "s/__NAMESPACE__/${1}/g" | sed "s/__CLASS__/${2}/g" > ../src/${1}/${2}.cc
cat __TEMPLATE__.cpp | sed "s/__NAMESPACE__/${1}/g" | sed "s/__CLASS__/${2}/g" > ../src/${1}/${2}.cpp
cat __TEMPLATE__.h   | sed "s/__NAMESPACE__/${1}/g" | sed "s/__CLASS__/${2}/g" > ../src/${1}/${2}.h
cat __TEMPLATE__.hh  | sed "s/__NAMESPACE__/${1}/g" | sed "s/__CLASS__/${2}/g" > ../src/${1}/${2}.hh
cat __TEMPLATE__.hpp | sed "s/__NAMESPACE__/${1}/g" | sed "s/__CLASS__/${2}/g" > ../src/${1}/${2}.hpp
cat ../src/${1}/${2}.h
