/*******************************************************************************
 *   Ledger App Lisk
 *   (c) 2023 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#pragma once

#include <string.h>
#include "eth_plugin_interface.h"

#define PUBLIC_KEY_LENGTH   32
#define LISK_ADDRESS_LENGTH 20

// All possible selectors of your plugin.
// EDIT THIS: Enter your selectors here, in the format X(NAME, value)
// A Xmacro below will create for you:
//     - an enum named selector_t with every NAME
//     - a map named SELECTORS associating each NAME with it's value
#define SELECTORS_LIST(X)                        \
    X(CLAIM_REGULAR_ACCOUNT, 0xf6de242d)         \
    X(CLAIM_MULTI_SIGNATURE_ACCOUNT, 0x2f559f68) \
    X(REWARD_CREATE_POSITION, 0xd1aaef05)        \
    X(REWARD_INIT_FAST_UNLOCK, 0x864c8725)       \
    X(REWARD_CLAIM_REWARDS, 0x5eac6239)          \
    X(REWARD_RESUME_UNLOCKING, 0x82d4ae58)       \
    X(REWARD_PAUSE_UNLOCKING, 0xfe042b5b)

// Xmacro helpers to define the enum and map
// Do not modify !
#define TO_ENUM(selector_name, selector_id)  selector_name,
#define TO_VALUE(selector_name, selector_id) selector_id,

// This enum will be automatically expanded to hold all selector names.
// The value SELECTOR_COUNT can be used to get the number of defined selectors
// Do not modify !
typedef enum selector_e {
    SELECTORS_LIST(TO_ENUM) SELECTOR_COUNT,
} selector_t;

typedef struct {
    uint8_t value[INT256_LENGTH];
} lock_t;

// This array will be automatically expanded to map all selector_t names with the correct value.
// Do not modify !
extern const uint32_t SELECTORS[SELECTOR_COUNT];

// Enumeration used to parse the smart contract data.
// EDIT THIS: Adapt the parameter names here.
typedef enum {
    // Common parameters
    PROOF,
    CLAIM_AMOUNT,
    RECIPIENT,
    UNEXPECTED_PARAMETER,
    NONE,

    // Claim regular account parameters
    PUBLIC_KEY,
    ED25519_SIGNATURE,

    // Claim multi-sig account parameters
    MULTISIG_KEYS,
    LSK_ADDRESS,
    ED25519_SIGNATURES,

    // Reward contract parameters
    LOCK_AMOUNT,
    LOCK_DURATION,
    LOCK_ID,
    LOCK_IDS_LEN,
    OFFSET,
} parameter;

typedef struct {
    union {
        struct {
            uint8_t claim_amount[INT256_LENGTH];
            uint8_t recipient[ADDRESS_LENGTH];
            uint8_t public_key[PUBLIC_KEY_LENGTH];
            uint8_t lsk_address[LISK_ADDRESS_LENGTH];
        } claim;

        struct {
            uint8_t lock_amount[INT256_LENGTH];
            uint8_t lock_duration[INT256_LENGTH];
        } rewardCreatePosition;

        struct {
            uint16_t lock_ids_len;
            lock_t lock_id[4];
        } reward;
    } body;
} lisk_t;

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
// EDIT THIS: This struct is used by your plugin to save the parameters you parse. You
// will need to adapt this struct to your plugin.
typedef struct context_s {
    char ticker[MAX_TICKER_LEN];
    uint8_t decimals;
    uint8_t token_found;

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.
    uint16_t offset;     // Offset at which the array or struct starts.
    bool go_to_offset;   // If set, will force the parsing to iterate through parameters until
                         // `offset` is reached.

    // For both parsing and display.
    selector_t selectorIndex;

    // lisk related context
    lisk_t lisk;
} context_t;

// Check if the context structure will fit in the RAM section ETH will prepare for us
// Do not remove!
ASSERT_SIZEOF_PLUGIN_CONTEXT(context_t);
