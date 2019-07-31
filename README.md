# panopticon-nfc-helper
> NFC device setup helper for the Panopticon project.

## Compile

Require [libnfc](https://github.com/nfc-tools/libnfc)

```
gcc -c nfc-helper.c
gcc -o nfc-helper nfc-helper.o -lnfc
```

## Run

```
./nfc-helper
```

and follow the on-screen instructions.
