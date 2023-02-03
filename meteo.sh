#!/bin/bash

# args handler, loop while arg not null
while [[ $# -gt 0 ]]; do 
    case $1 in
        -t1) #if arg == -t1
            T1=true # set t1 to true to enable the mode
            Type=true # set type to true to know we have one or more type
            shift #skip this arg
            ;; #end of condition
        -t2)
            T2=true
            Type=true
            shift
            ;;
        -t3)
            T3=true
            Type=true
            shift
            ;;
        -p1)
            P2=true
            Type=true
            shift
            ;;
        -p2)
            P2=true
            Type=true
            shift
            ;;
        -p3)
            P2=true
            Type=true
            shift
            ;;
        -w)
            Type=true
            W=true
            shift
            ;;
        -h)
            Type=true
            H=true
            shift
            ;;
        -m)
            Type=true
            M=true
            shift
            ;;
        -F|-G|-S|-A|-O|-Q) # Location checker, if arg is location
            if [[ ${loc} ]]; then #check if loc is already set
                echo "One location" #exit error
                exit 1
            fi
            loc=true # set loc to true
            if [[ ${1} == "-F" ]]; then #check if arg is France to enable the mode
                F=true
            fi
            if [[ ${1} == "-G" ]]; then #check if arg is Guyanne to enable the mode
                G=true
            fi
            shift
            ;;
        -d)
            dmin="$2"
            dmax="$3"
            shift
            shift
            shift
            ;;
        --tab|--abr|--avl) #check if sort mode is already set
            if [[ ${sort} ]]; then
                echo "One sort mode"
                exit 1
            fi
            sort="$1" #set sort variable to current arg
            shift
            ;;
        -f) # set input file
            input="$2"
            shift
            shift
            ;;
        *) # if arg is other, error and exit
            echo "Unknown option $1"
            exit 1
            ;;
    esac
done

if [[ -z ${Type} ]]; then #check if we have a mode
    echo "No type"
    exit 1
fi
if [[ -z ${input} ]]; then #check if input file is already
    echo "No input file"
    exit 1
fi

if ! [[ -f "meteo_sort" ]]; then #if binary doesn't exist, we compile
    make re
fi

if [[ -z ${sort} ]]; then #check if we have a sort mode else it set to avl by default
    sort="--avl"
fi


if [[ ${H} ]]; then #for H mode
    rm -f inn.txt #remove temporarie file
    #read $input file and split each line by ; delimitator
    #if loc F is set, check GPS coords and write data needed for the final graph in the temporarie file inn.txt
    awk -F "\"*;\"*" '
    {
        if (NR!=1) {
            if ($F) {
                if ($10 < 41 || $10 > 52 || $11 < -5 || $11 > 10) {
                    next;
                }
            }
            if ($14) {
                print $14";"$14";"$10 > "inn.txt";
            }
        }
    }' "$input"
    # start sort programm
    ./meteo_sort -f "inn.txt" -o "outpuut.txt" -r ${sort}
    # if no error in the return value, create the graph with gnuplot
    if [[ $(echo $?) == 0 ]]; then
        echo 'set terminal png
        set output "height.png"
        set datafile separator ";,"
        set border 4095 front lt black linewidth 1.000 dashtype solid
        set title "Height of stations"
        set style data lines
        set xlabel "Latitude"
        set ylabel "Longitude"
        set dgrid3d 20,20,10
        set pm3d map
        set palette rgb 21,22,23
        splot "outpuut.txt" using 1:2:3' >> gnutest
        gnuplot --persist gnutest
        rm gnutest
    fi
fi

if [[ ${M} ]]; then
    rm -f inn.txt
    awk -F "\"*;\"*" '
    {
        if (NR!=1) {
            if ($F) {
                if ($10 < 41 || $10 > 52 || $11 < -5 || $11 > 10) {
                    next;
                }
            }
            if ($5) {
                print $5";"$5";"$10 > "inn.txt";
            }
        }
    }' "$input"
    ./meteo_sort -f "inn.txt" -o "outpuut.txt" -r ${sort}
    if [[ $(echo $?) == 0 ]]; then
        echo 'set terminal png
        set output "moisture.png"
        set datafile separator ";,"
        set title "Moisture of stations"
        set style data lines
        set xlabel "Latitude"
        set ylabel "Longitude"
        set border 4095 front lt black linewidth 1.000 dashtype solid
        set dgrid3d 40,40,10
        set pm3d map
        set palette rgb 21,22,23
        splot "outpuut.txt" using 1:2:3' >> gnutest
        gnuplot --persist gnutest
        rm gnutest
    fi

fi

if [[ ${W} ]]; then
    rm -f inn.txt
    awk -F '[;]' '
    {
        if (NR!=1) {
            if ($F) {
                if ($10 < 41 || $10 > 52 || $11 < -5 || $11 > 10) {
                    next;
                }
            }
            if ($5) {
                print $1";"$4";"$5";"$10";"$11 > "inn.txt";
            }
        }
    }' "$input"
    ./meteo_sort -f "inn.txt" -o "outpuut.txt" ${sort}
    if [[ $(echo $?) == 0 ]]; then
        echo 'set terminal png
        set datafile separator ";,"
        set output "wind.png"
        set title "Wind"
        set style data points
        set angles degrees
        set xlabel "Longitude"
        set ylabel "Latitude"' >> gnutest
        echo "plot  '"outpuut.txt"' using 3:4:"'(($2*1.5)*cos($1)):(($2*1.5)*sin($1))'" with vector" >> gnutest
        gnuplot --persist gnutest
        rm gnutest
    fi

fi

