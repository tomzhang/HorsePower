#!/usr/bin/env bash

for x in {1..22}
do
    echo Processing query $x
    python convert_raw_to_opt.py "raw/q${x}.json" > "opt/q${x}.json"
done

