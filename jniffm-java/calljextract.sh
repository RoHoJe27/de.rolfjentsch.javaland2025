#!/bin/bash

/opt/jextract-22/bin/jextract \
        --include-dir include \
        --library api \
        --target-package de.rolfjentsch.javaland.ffm.implementation \
        --output src/main/java \
        include/api.h


