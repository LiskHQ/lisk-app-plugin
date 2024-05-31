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

// Set UI for "Claim LSK" screen.
static bool set_claim_ui(ethQueryContractUI_t *msg, const context_t *context) {
    if (context->selectorIndex == CLAIM_AIRDROP) {
        strlcpy(msg->title, "Claim Airdrop (LSK)", msg->titleLength);
    } else {
        strlcpy(msg->title, "Claim LSK", msg->titleLength);
    }

    uint8_t decimals = 18;
    const char *ticker = "LSK";

    return amountToString(context->lisk.body.claim.claim_amount,
                          sizeof(context->lisk.body.claim.claim_amount),
                          decimals,
                          ticker,
                          msg->msg,
                          msg->msgLength);
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

// Set UI for "Sender Address" screen.
static bool set_sender_address_ui(ethQueryContractUI_t *msg, context_t *context) {
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

// Set UI for "Recipient" screen.
static bool set_recipient_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Recipient Address L2", msg->titleLength);

    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    // Get the string representation of the address stored in `context->beneficiary`. Put it in
    // `msg->msg`.
    return getEthAddressStringFromBinary(
        context->lisk.body.claim.recipient,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        chainid);
}

// Set UI for "Lock Amount" screen.
static bool set_lock_amount_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Lock Amount", msg->titleLength);

    uint8_t decimals = 18;
    const char *ticker = "LSK";

    return amountToString(context->lisk.body.rewardCreatePosition.first,
                          sizeof(context->lisk.body.rewardCreatePosition.first),
                          decimals,
                          ticker,
                          msg->msg,
                          msg->msgLength);
}

// Set UI for "Lock Duration" screen.
static bool set_lock_duration_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Duration (in days)", msg->titleLength);
    return uint256_to_decimal(context->lisk.body.rewardCreatePosition.secound,
                              sizeof(context->lisk.body.rewardCreatePosition.secound),
                              msg->msg,
                              msg->msgLength);
}

// Set UI for "Lock IDs" screen.
static bool set_lock_ids_ui(ethQueryContractUI_t *msg, arr_uint8_t *lock) {
    strlcpy(msg->title, "Lock ID", msg->titleLength);
    return uint256_to_decimal(lock->value, INT256_LENGTH, msg->msg, msg->msgLength);
}

// Set UI for "Increase Amount" screen.
static bool set_amount_ui(ethQueryContractUI_t *msg, arr_uint8_t *amount) {
    strlcpy(msg->title, "Increase Amount", msg->titleLength);

    uint8_t decimals = 18;
    const char *ticker = "LSK";

    return amountToString(amount->value, INT256_LENGTH, decimals, ticker, msg->msg, msg->msgLength);
}

// Set UI for "Extend Duration" screen.
static bool set_duration_ui(ethQueryContractUI_t *msg, arr_uint8_t *duration) {
    strlcpy(msg->title, "Duration", msg->titleLength);
    return uint256_to_decimal(duration->value, INT256_LENGTH, msg->msg, msg->msgLength);
}

// Set UI for "Unused Rewards Amount" screen.
static bool set_unused_amount_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Amount", msg->titleLength);

    uint8_t decimals = 18;
    const char *ticker = "LSK";

    return amountToString(context->lisk.body.rewardAddUnusedRewards.data.first,
                          sizeof(context->lisk.body.rewardAddUnusedRewards.data.first),
                          decimals,
                          ticker,
                          msg->msg,
                          msg->msgLength);
}

// Set UI for "Add Unused Duration" screen.
static bool set_unused_duration_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Duration (in days)", msg->titleLength);
    return uint256_to_decimal(context->lisk.body.rewardAddUnusedRewards.data.secound,
                              sizeof(context->lisk.body.rewardAddUnusedRewards.data.secound),
                              msg->msg,
                              msg->msgLength);
}

// Set UI for "Add Unused Duration" screen.
static bool set_unused_delay_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Delay (in days)", msg->titleLength);
    return uint256_to_decimal(context->lisk.body.rewardAddUnusedRewards.delay,
                              sizeof(context->lisk.body.rewardAddUnusedRewards.delay),
                              msg->msg,
                              msg->msgLength);
}

// Set UI for "Proposal ID" screen.
static bool set_proposal_id_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Proposal ID", msg->titleLength);
    return uint256_to_decimal(context->lisk.body.governor.data.first,
                              sizeof(context->lisk.body.governor.data.first),
                              msg->msg,
                              msg->msgLength);
}

// Set UI for "Support" screen.
static bool set_support_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Support", msg->titleLength);
    return uint256_to_decimal(context->lisk.body.governor.data.secound,
                              sizeof(context->lisk.body.governor.data.secound),
                              msg->msg,
                              msg->msgLength);
}

static bool set_reason_ui(ethQueryContractUI_t *msg, string_uint8_t *reason) {
    strlcpy(msg->title, "Reason", msg->titleLength);
    snprintf(msg->msg, msg->msgLength, "%s", reason->value);
    return true;
}

// Set UI for "Target Address" screen.
static bool set_target_ui(ethQueryContractUI_t *msg, arr_address_t *target) {
    strlcpy(msg->title, "Target Address", msg->titleLength);

    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    return getEthAddressStringFromBinary(
        target->value,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        chainid);
}

// Set UI for "Value" screen.
static bool set_value_ui(ethQueryContractUI_t *msg, arr_uint8_t *value) {
    strlcpy(msg->title, "Value", msg->titleLength);
    return uint256_to_decimal(value->value, INT256_LENGTH, msg->msg, msg->msgLength);
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
                    ret = set_claim_ui(msg, context);
                    break;
                case 1:
                    ret = set_sender_public_key_ui(msg, context);
                    break;
                case 2:
                    ret = set_recipient_ui(msg, context);
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case CLAIM_MULTI_SIGNATURE_ACCOUNT:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_claim_ui(msg, context);
                    break;
                case 1:
                    ret = set_sender_address_ui(msg, context);
                    break;
                case 2:
                    ret = set_recipient_ui(msg, context);
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case REWARD_CREATE_POSITION:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_lock_amount_ui(msg, context);
                    break;
                case 1:
                    ret = set_lock_duration_ui(msg, context);
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case REWARD_ADD_UNUSED_REWARDS:
        case REWARD_FUND_STAKING_REWARDS:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_unused_amount_ui(msg, context);
                    break;
                case 1:
                    ret = set_unused_duration_ui(msg, context);
                    break;
                case 2:
                    ret = set_unused_delay_ui(msg, context);
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
                ret = set_lock_ids_ui(msg, &context->lisk.body.reward.lock_id[msg->screenIndex]);
            } else {
                PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case REWARD_INC_LOCKING_AMOUNT:
            switch (msg->screenIndex) {
                case 0:
                    ret =
                        set_lock_ids_ui(msg, &context->lisk.body.rewardIncLockingAmount.first[0]);
                    break;
                case 1:
                    ret = set_amount_ui(msg, &context->lisk.body.rewardIncLockingAmount.secound[0]);
                    break;
                case 2:
                    ret =
                        set_lock_ids_ui(msg, &context->lisk.body.rewardIncLockingAmount.first[1]);
                    break;
                case 3:
                    ret = set_amount_ui(msg, &context->lisk.body.rewardIncLockingAmount.secound[1]);
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case REWARD_EXTEND_DURATION:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_lock_ids_ui(msg, &context->lisk.body.rewardExtendDuration.first[0]);
                    break;
                case 1:
                    ret =
                        set_duration_ui(msg, &context->lisk.body.rewardExtendDuration.secound[0]);
                    break;
                case 2:
                    ret = set_lock_ids_ui(msg, &context->lisk.body.rewardExtendDuration.first[1]);
                    break;
                case 3:
                    ret =
                        set_duration_ui(msg, &context->lisk.body.rewardExtendDuration.secound[1]);
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case CLAIM_AIRDROP:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_sender_address_ui(msg, context);
                    break;
                case 1:
                    ret = set_claim_ui(msg, context);
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case GOVERNOR_CAST_VOTE:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_proposal_id_ui(msg, context);
                    break;
                case 1:
                    ret = set_support_ui(msg, context);
                    break;
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case GOVERNOR_CAST_VOTE_WITH_REASON:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_proposal_id_ui(msg, context);
                    break;
                case 1:
                    ret = set_support_ui(msg, context);
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
                    ret = set_target_ui(msg, &context->lisk.body.governorPropose.first[0]);
                    break;
                case 1:
                    ret = set_value_ui(msg, &context->lisk.body.governorPropose.secound[0]);
                    break;
                case 2:
                    ret = set_target_ui(msg, &context->lisk.body.governorPropose.first[1]);
                    break;
                case 3:
                    ret = set_value_ui(msg, &context->lisk.body.governorPropose.secound[1]);
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
