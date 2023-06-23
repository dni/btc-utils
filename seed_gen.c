#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wally_core.h>
#include <wally_address.h>
#include <wally_bip32.h>
#include <wally_bip39.h>
#include <sodium.h>

int main(void) {
    int lw_response;
    lw_response = wally_init(0);
    if (lw_response) {
        printf("Error: Wally_init failed: %d\n",lw_response);
        exit(-1);
    }

    unsigned char entropy[16];
    randombytes_buf(entropy, 16);

    char *mnem = NULL;
    lw_response = bip39_mnemonic_from_bytes(NULL, entropy, 16, &mnem);
    if (lw_response) {
        printf("Error: getting the mnemonic\n");
        exit(1);
    }

    printf("Mnemonic: %s\n", mnem);

    unsigned char seed[BIP39_SEED_LEN_512];
    size_t seed_len;

    lw_response = bip39_mnemonic_to_seed(mnem, NULL, seed, BIP39_SEED_LEN_512, &seed_len);
    if (lw_response) {
        printf("Error: getting the seed\n");
        exit(1);
    }

    char *seed_hex;
    wally_hex_from_bytes(seed, sizeof(seed), &seed_hex);
    printf("Seed: %s\n",seed_hex);

    struct ext_key *key_root;
    lw_response = bip32_key_from_seed_alloc(seed,sizeof(seed),BIP32_VER_MAIN_PRIVATE,0,&key_root);
    if (lw_response) {
        printf("Error: getting key_root\n");
        exit(1);
    }

    char *xprv;
    lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PRIVATE, &xprv);
    if (lw_response) {
        printf("Error: getting xprv key\n");
        exit(1);
    }
    printf("Root xprv: %s\n", xprv);

    char *xpub;
    lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PUBLIC, &xpub);
    if (lw_response) {
        printf("Error: getting xpub key\n");
        exit(1);
    }
    printf("Root xpub: %s\n", xpub);

    uint32_t path_account[] = {BIP32_INITIAL_HARDENED_CHILD+84, BIP32_INITIAL_HARDENED_CHILD+1, BIP32_INITIAL_HARDENED_CHILD};

    struct ext_key *key_account;
    lw_response = bip32_key_from_parent_path_alloc(key_root, path_account, sizeof(path_account), BIP32_FLAG_KEY_PRIVATE, &key_account);
    if (lw_response) {
        printf("Error: getting ext_key\n");
        exit(1);
    }

    char *axprv;
    lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PRIVATE, &axprv);
    if (lw_response) {
        printf("Error: getting account xprv key\n");
        exit(1);
    }
    printf("Account xprv: %s\n", axprv);

    char *axpub;
    lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PUBLIC, &axpub);
    if (lw_response) {
        printf("Error: getting account xpub key\n");
        exit(1);
    }
    printf("Account xpub: %s\n", axpub);

    char *segwit;
    lw_response = wally_bip32_key_to_addr_segwit(key_account, "bc", 0, &segwit);

    printf("[m/84'/1'/0'/0/0]: %s\n", segwit);

    wally_cleanup(0);
    return 0;
}
