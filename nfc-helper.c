#include <err.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <nfc/nfc.h>
#include <nfc/nfc-types.h>

#define MAX_DEVICE_COUNT 4

int main(int argc, const char** argv) {
  nfc_context *context;
  nfc_init(&context);
  if (context == NULL) {
    printf("Unable to initialise LibNFC.\n");
    exit(-1);
  }

  nfc_connstring connstrings[MAX_DEVICE_COUNT];
  size_t found = nfc_list_devices(context, connstrings, MAX_DEVICE_COUNT);

  if (found == 0) {
    printf("No NFC devices found.\n");
    nfc_exit(context);
    exit(-1);
  }

  if (found == 1) {
    printf("Found 1 NFC device at %s.\n", connstrings[0]);
    nfc_exit(context);
    exit(-1);
  }

  if (found != 2) {
    printf("Found %d NFC devices, only a maximum of 2 devices are supported. "
        "Please remove %d devices to continue.\n",
        (int)found, (int)(found - 2));
    nfc_exit(context);
    exit(-1);
  }

  printf("Found 2 NFC devices, attempting to determine placement.\n");
  printf("Place a tag on the LEFT NFC device and remove any tags from the "
      "right NFC device\n");
  printf("Press enter to continue...\n");
  getchar();

  const nfc_modulation nmModulation = {.nmt = NMT_ISO14443A, .nbr = NBR_106};
  nfc_target nt;

  char* left = NULL;
  char* right = NULL;

  for (int i = 0; i < found; i++) {
    nfc_device* pnd = nfc_open(context, connstrings[i]);
    if (pnd != NULL) {
      if (nfc_initiator_init(pnd) < 0) {
        printf("Unable to initialise NFC device - %s.\n", nfc_strerror(pnd));
        nfc_close(pnd);
        nfc_exit(context);
        exit(-1);
      }

      if (nfc_initiator_list_passive_targets(pnd, nmModulation, &nt, 1) > 0) {
        left = connstrings[i];
      } else {
        right = connstrings[i];
      }

      nfc_close(pnd);
    } else {
      printf("nfc_open failed for %s.\n", connstrings[i]);
    }
  }

  printf("Configuration complete.\n");
  printf("Left NFC device address: %s\n", left);
  printf("Right NFC device address: %s\n", right);

  nfc_exit(context);
  exit(0);
}
