#!/bin/bash
SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname $SCRIPT`

tests_path=$SCRIPTPATH/tests
echo $tests_path
docker run -v $tests_path:/vsf/tests -ti vsf bash