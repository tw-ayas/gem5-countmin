#!/bin/sh
DIR="$HOME/stats/perlbench_r"
if [ ! -d "$DIR" ]; then
    echo 'mkdir $HOME/stats/perlbench_r'
    mkdir $HOME/stats/perlbench_r
fi
DIR="$HOME/stats/perlbench_r/cron_out"
if [ ! -d "$DIR" ]; then
    echo 'mkdir $HOME/stats/perlbench_r/cron_out'
    mkdir $HOME/stats/perlbench_r/cron_out
fi

