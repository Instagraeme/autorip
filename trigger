#!/bin/bash

case $1 in
  bd)
    echo bluray $2 | nc 127.0.0.1 5000
    ;;
  dvd)
    #/usr/local/bin/dvd_trigger &
    echo dvd $2 | nc 127.0.0.1 5000
    ;;
  *)
    echo "Usage: $0 {bd|dvd}"
    ;;
esac

exit 0
