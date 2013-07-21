#!/bin/bash

TEST_FOLDER="test_package"
FILES="analyse_recall_precision_mex.cpp evaluate_salient_object.m immaxpos.m install_dependencies.m calculate_classification_scores_mex.cpp README.md"

# copy the files and try to compile, if it failes, then the package is broken!
rm -rf $TEST_FOLDER
mkdir $TEST_FOLDER
for file in $FILES; do
	cp $file in $TEST_FOLDER
done
cd $TEST_FOLDER
zip -r sal_obj_eval.zip *.*
