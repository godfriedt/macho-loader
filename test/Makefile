test:
    gcc custom.c -o custom
    ./custom
    echo "return value of custom is"
    echo $?
    gcc ../src/main.c ../src/load.c -o main
    ../src/main custom
    echo "return value of loader is"
    echo $?