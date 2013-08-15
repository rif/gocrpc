gocrpc
======

Golang to c json rpc communication


Running
-------

In one terminal:

```shell
go run server.go
```

In other terminal:
```shell
gcc -O2 -Wall -o client client.c
./client
```
