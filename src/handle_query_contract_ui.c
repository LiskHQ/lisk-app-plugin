#include <stdio.h>
#include "plugin.h"

static bool set_bytes_to_hex(char *msg, size_t msgLen, const uint8_t *value, uint16_t valueLen) {
    if (value == NULL || msg == NULL || (valueLen * 2U) + 1U > msgLen) {
        return false;
    }

    const char *const HEX_DIGITS = "0123456789abcdef";

    size_t len = valueLen;

    do {
        *msg++ = HEX_DIGITS[(*value >> 4U) & 0xF];
        *msg++ = HEX_DIGITS[*value & 0xF];
        ++value;
    } while (len-- > 1U);

    *msg = '\0';

    return true;
}

// Set UI with custom title for amount screen with uint8 data.
static bool set_amount_ui_uint8(ethQueryContractUI_t *msg,
                                const uint8_t *amount,
                                const char *title) {
    strlcpy(msg->title, title, msg->titleLength);

    uint8_t decimals = 18;
    const char *ticker = "LSK";

    return amountToString(amount, INT256_LENGTH, decimals, ticker, msg->msg, msg->msgLength);
}

// Set UI with custom title for amount screen with uint8 data.
static bool set_amount_ui_arr(ethQueryContractUI_t *msg,
                              const arr_uint8_t *amount,
                              const char *title) {
    return set_amount_ui_uint8(msg, amount->value, title);
}

// Set UI for "Sender Public Key" screen.
static bool set_sender_public_key_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Sender Lisk Public Key", msg->titleLength);
    set_bytes_to_hex(msg->msg,
                     msg->msgLength,
                     context->lisk.body.claim.public_key,
                     sizeof(context->lisk.body.claim.public_key));
    return true;
}

// Set UI for "Lisk Address" screen.
static bool set_lisk_address_ui(ethQueryContractUI_t *msg, context_t *context) {
    if (context->selectorIndex == CLAIM_AIRDROP) {
        strlcpy(msg->title, "Lisk Address", msg->titleLength);
    } else {
        strlcpy(msg->title, "Sender Lisk Address", msg->titleLength);
    }

    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    set_bytes_to_hex(msg->msg + 2,
                     msg->msgLength,
                     context->lisk.body.claim.lsk_address,
                     sizeof(context->lisk.body.claim.lsk_address));
    return true;
}

// Set UI for "Reason" screen.
static bool set_reason_ui(ethQueryContractUI_t *msg, string_uint8_t *reason) {
    strlcpy(msg->title, "Reason", msg->titleLength);
    snprintf(msg->msg, msg->msgLength, "%s", reason->value);
    return true;
}

// Set UI with custom title for decimal screan with uint8 data
static bool set_decimal_ui_uint8(ethQueryContractUI_t *msg,
                                 const uint8_t *value,
                                 const char *title) {
    strlcpy(msg->title, title, msg->titleLength);
    return uint256_to_decimal(value, INT256_LENGTH, msg->msg, msg->msgLength);
}

// Set UI with custom title for "decimal" screen with array of uint8 data.
static bool set_decimal_ui_arr(ethQueryContractUI_t *msg,
                               const arr_uint8_t *arr,
                               const char *title) {
    return set_decimal_ui_uint8(msg, arr->value, title);
}

// Set UI with custom title for address Screen.
static bool set_address_ui(ethQueryContractUI_t *msg, uint8_t *address, const char *title) {
    strlcpy(msg->title, title, msg->titleLength);

    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    return getEthAddressStringFromBinary(
        address,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        chainid);
}

void handle_query_contract_ui(ethQueryContractUI_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    bool ret = false;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    // EDIT THIS: Adapt the cases for the screens you'd like to display.
    switch (context->selectorIndex) {
        case CLAIM_REGULAR_ACCOUNT:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_amount_ui_uint8(msg,
                                              context->lisk.body.claim.claim_amount,
                                              "Claim LSK");
                    break;
                case 1:
                    ret = set_sender_public_key_ui(msg, context);
                    break;
                case 2:
                    ret = set_address_ui(msg,
                                         context->lisk.body.claim.recipient,
                                         "Recipient Address L2");
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case CLAIM_MULTI_SIGNATURE_ACCOUNT:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_amount_ui_uint8(msg,
                                              context->lisk.body.claim.claim_amount,
                                              "Claim LSK");
                    break;
                case 1:
                    ret = set_lisk_address_ui(msg, context);
                    break;
                case 2:
                    ret = set_address_ui(msg,
                                         context->lisk.body.claim.recipient,
                                         "Recipient Address L2");
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case REWARD_CREATE_POSITION:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_amount_ui_uint8(msg,
                                              context->lisk.body.rewardCreatePosition.first,
                                              "Lock Amount");
                    break;
                case 1:
                    ret =
                        set_decimal_ui_uint8(msg,
                                             context->lisk.body.rewardAddUnusedRewards.data.second,
                                             "Duration (in days)");
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case REWARD_ADD_UNUSED_REWARDS:
        case REWARD_FUND_STAKING_REWARDS:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_amount_ui_uint8(msg,
                                              context->lisk.body.rewardAddUnusedRewards.data.first,
                                              "Amount");
                    break;
                case 1:
                    ret =
                        set_decimal_ui_uint8(msg,
                                             context->lisk.body.rewardAddUnusedRewards.data.second,
                                             "Duration (in days)");
                    break;
                case 2:
                    ret = set_decimal_ui_uint8(msg,
                                               context->lisk.body.rewardAddUnusedRewards.delay,
                                               "Delay (in days)");
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case REWARD_INIT_FAST_UNLOCK:
        case REWARD_CLAIM_REWARDS:
        case REWARD_PAUSE_UNLOCKING:
        case REWARD_RESUME_UNLOCKING:
        case REWARD_DELETE_POSITIONS:
            if (msg->screenIndex < context->lisk.body.reward.lock_ids_len) {
                ret = set_decimal_ui_arr(msg,
                                         &context->lisk.body.reward.lock_id[msg->screenIndex],
                                         "Lock ID");
            } else {
                PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case REWARD_INC_LOCKING_AMOUNT:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_decimal_ui_arr(msg,
                                             &context->lisk.body.rewardIncLockingAmount.first[0],
                                             "Lock ID");
                    break;
                case 1:
                    ret = set_amount_ui_arr(msg,
                                            &context->lisk.body.rewardIncLockingAmount.second[0],
                                            "Increase Amount");
                    break;
                case 2:
                    ret = set_decimal_ui_arr(msg,
                                             &context->lisk.body.rewardIncLockingAmount.first[1],
                                             "Lock ID");
                    break;
                case 3:
                    ret = set_amount_ui_arr(msg,
                                            &context->lisk.body.rewardIncLockingAmount.second[1],
                                            "Increase Amount");
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case REWARD_EXTEND_DURATION:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_decimal_ui_arr(msg,
                                             &context->lisk.body.rewardExtendDuration.first[0],
                                             "Lock ID");
                    break;
                case 1:
                    ret = set_decimal_ui_arr(msg,
                                             &context->lisk.body.rewardExtendDuration.second[0],
                                             "Duration");
                    break;
                case 2:
                    ret = set_decimal_ui_arr(msg,
                                             &context->lisk.body.rewardExtendDuration.first[1],
                                             "Lock ID");
                    break;
                case 3:
                    ret = set_decimal_ui_arr(msg,
                                             &context->lisk.body.rewardExtendDuration.second[1],
                                             "Duration");
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case CLAIM_AIRDROP:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_lisk_address_ui(msg, context);
                    break;
                case 1:
                    ret = set_amount_ui_uint8(msg,
                                              context->lisk.body.claim.claim_amount,
                                              "Claim Airdrop (LSK)");
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case GOVERNOR_CAST_VOTE:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_decimal_ui_uint8(msg,
                                               context->lisk.body.governor.data.first,
                                               "Proposal ID");
                    break;
                case 1:
                    ret = set_decimal_ui_uint8(msg,
                                               context->lisk.body.governor.data.second,
                                               "Support");
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case GOVERNOR_CAST_VOTE_WITH_REASON:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_decimal_ui_uint8(msg,
                                               context->lisk.body.governor.data.first,
                                               "Proposal ID");
                    break;
                case 1:
                    ret = set_decimal_ui_uint8(msg,
                                               context->lisk.body.governor.data.second,
                                               "Support");
                    break;
                case 2:
                    ret = set_reason_ui(msg, &context->lisk.body.governor.reason);
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case GOVERNOR_PROPOSE:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_address_ui(msg,
                                         context->lisk.body.governorPropose.data.first[0].value,
                                         "Target Address");
                    break;
                case 1:
                    ret = set_decimal_ui_arr(msg,
                                             &context->lisk.body.governorPropose.data.second[0],
                                             "Value");
                    break;
                case 2:
                    ret = set_address_ui(msg,
                                         context->lisk.body.governorPropose.data.first[1].value,
                                         "Target Address");
                    break;
                case 3:
                    ret = set_decimal_ui_arr(msg,
                                             &context->lisk.body.governorPropose.data.second[1],
                                             "Value");
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
