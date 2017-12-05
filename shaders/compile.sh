#!/bin/bash
for f in shader.*
do
    glslangValidator -V ${f}
done