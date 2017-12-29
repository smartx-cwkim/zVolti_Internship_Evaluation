#!/bin/bash

gcc $1.c -lpthread -o $1

./$1 $2
