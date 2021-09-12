#include <Arduino.h>
#include <uECC.h>
#include <keccak256.h>

#define PRIVATE_KEY_LENGTH 32
#define PUBLIC_KEY_LENGTH 64
#define ADDRESS_LENGTH 20

const struct uECC_Curve_t* curve = uECC_secp256k1();

static void toHex(char* dest, size_t dest_len, const uint8_t* values, size_t val_len) {
  *dest = '\0';
  while (val_len--) {
    sprintf(dest, "%02X", *values);
    dest += 2;
    ++values;
  }
}

static int RNG(uint8_t * dest, unsigned size) {
  int i = 0;
  for(i = 0; i < size; i++) {
    // TODO: check if ok not to skip ticks
    // vTaskDelay(1);
    uint32_t randomNumber = esp_random();
    *(dest + i) = (uint8_t)(randomNumber & 0xff);
  }
  return 1;
}

static void keccak256(const uint8_t *data, uint16_t length, uint8_t *result) {
    SHA3_CTX context;
    keccak_init(&context);
    keccak_update(&context, (const unsigned char*)data, (size_t)length);
    keccak_final(&context, (unsigned char*)result);
    // clear context
    memset((char*)&context, 0, sizeof(SHA3_CTX));
}

void initKeys() {
  uECC_set_rng(&RNG);
}

void pick(char* privateKey, char* publicKey, char* address) {
  // get a key pair
  uint8_t privKey[PRIVATE_KEY_LENGTH];
  uint8_t pubKey[PUBLIC_KEY_LENGTH];
  uECC_make_key(pubKey, privKey, curve);
  // hash the public key
  uint8_t hashed[32];
  keccak256(pubKey, 64, hashed);
  // extract relevant address part from the hash
  uint8_t addressPart[ADDRESS_LENGTH];
  memcpy(addressPart, &hashed[12], ADDRESS_LENGTH);
  // store human readable values
  toHex(privateKey, sizeof(privateKey), privKey, sizeof(privKey));
  toHex(publicKey, sizeof(publicKey), pubKey, sizeof(pubKey));
  char tmpAddress[ADDRESS_LENGTH * 2 + 1];
  toHex(tmpAddress, sizeof(tmpAddress), addressPart, sizeof(addressPart));
  strcpy(address, "0x");
  strcat(address, tmpAddress);
}
