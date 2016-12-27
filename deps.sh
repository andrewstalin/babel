#!/bin/bash

declare -A DOWNLOADED

CURDIR=${PWD}

function getdeps {
    while IFS='' read -r line || [[ -n "$line" ]]; do
        IFS=', ' read -r -a parts <<< "$line"
        if ! [ ${DOWNLOADED[${parts[0]}]+_} ]; then
	    if ! [ -d ${PWD}/3rdparty/${parts[0]} ]; then
	        if [ ${#parts[@]} -gt 2 ]; then
		   git clone --progress -v -b ${parts[2]} ${parts[1]} ${PWD}/3rdparty/${parts[0]}
                else
                   git clone --progress -v ${parts[1]} ${PWD}/3rdparty/${parts[0]}
                fi
	     else
                cd ${PWD}/3rdparty/${parts[0]}
             
                if [ ${#parts[@]} -gt 2 ]; then
		   git pull -v --progress origin ${parts[2]}
                else
                   git pull -v --progress origin master
                fi

                cd ${CURDIR}
	     fi

	    DOWNLOADED[${parts[0]}]=1
            echo ""

	    if [ -f ${PWD}/3rdparty/${parts[0]}/deps ]; then
                echo "get deps: ${PWD}/3rdparty/${parts[0]}/deps"
	        getdeps "${PWD}/3rdparty/${parts[0]}/deps"
	    fi      
        fi
    done < "$1"
}

getdeps "deps"
