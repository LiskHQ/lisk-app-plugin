#include "plugin.h"

static void handle_claim_regular_account(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case PROOF:  // _proof
            context->next_param = PUBLIC_KEY;
            break;
        case PUBLIC_KEY:  // _pubKey
            copy_parameter(context->lisk.body.claim.public_key,
                           msg->parameter,
                           sizeof(context->lisk.body.claim.public_key));
            context->next_param = CLAIM_AMOUNT;
            break;
        case CLAIM_AMOUNT:  // _amount
            copy_parameter(context->lisk.body.claim.claim_amount,
                           msg->parameter,
                           sizeof(context->lisk.body.claim.claim_amount));
            context->next_param = RECIPIENT;
            break;
        case RECIPIENT:  // _recipient
            copy_address(context->lisk.body.claim.recipient,
                         msg->parameter,
                         sizeof(context->lisk.body.claim.recipient));
            context->next_param = ED25519_SIGNATURE;
            break;
        case ED25519_SIGNATURE:  // _sig
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case UNEXPECTED_PARAMETER:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_claim_multisig_account(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case PROOF:  // _proof
            context->next_param = LSK_ADDRESS;
            break;
        case LSK_ADDRESS:  // _lskAddress
            copy_parameter(context->lisk.body.claim.lsk_address,
                           msg->parameter,
                           sizeof(context->lisk.body.claim.lsk_address));
            context->next_param = CLAIM_AMOUNT;
            break;
        case CLAIM_AMOUNT:  // _amount
            copy_parameter(context->lisk.body.claim.claim_amount,
                           msg->parameter,
                           sizeof(context->lisk.body.claim.claim_amount));
            context->next_param = MULTISIG_KEYS;
            break;
        case MULTISIG_KEYS:  // _keys
            context->next_param = RECIPIENT;
            break;
        case RECIPIENT:  // _recipient
            copy_address(context->lisk.body.claim.recipient,
                         msg->parameter,
                         sizeof(context->lisk.body.claim.recipient));
            context->next_param = ED25519_SIGNATURES;
            break;
        case ED25519_SIGNATURES:  // _sigs
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case UNEXPECTED_PARAMETER:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_reward_create_position(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case LOCK_AMOUNT:  // amount
            copy_parameter(context->lisk.body.rewardCreatePosition.lock_amount,
                           msg->parameter,
                           sizeof(context->lisk.body.rewardCreatePosition.lock_amount));
            context->next_param = LOCK_DURATION;
            break;
        case LOCK_DURATION:  // lockingDuration
            copy_parameter(context->lisk.body.rewardCreatePosition.lock_duration,
                           msg->parameter,
                           sizeof(context->lisk.body.rewardCreatePosition.lock_duration));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case UNEXPECTED_PARAMETER:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

uint16_t counter = 0;
static void handle_reward_init_fast_unlock(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case OFFSET:
            context->next_param = LOCK_IDS_LEN;
            break;
        case LOCK_IDS_LEN:
            if (!U2BE_from_parameter(msg->parameter,
                                     &context->lisk.body.rewardFastUnlock.lock_ids_len) ||
                context->lisk.body.rewardFastUnlock.lock_ids_len > 4 ||
                context->lisk.body.rewardFastUnlock.lock_ids_len == 0) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }

            context->next_param = LOCK_ID;
            break;
        case LOCK_ID:
            if (counter == context->lisk.body.rewardFastUnlock.lock_ids_len) {
                context->next_param = NONE;
            }
            copy_parameter(context->lisk.body.rewardFastUnlock.lock_id[counter].value,
                           msg->parameter,
                           INT256_LENGTH);
            if (context->lisk.body.rewardFastUnlock.lock_ids_len > 1) {
                context->next_param = LOCK_ID_NEXT;
                counter++;
            } else {
                context->next_param = NONE;
            }
            break;
        case LOCK_ID_NEXT:
            copy_parameter(context->lisk.body.rewardFastUnlock.lock_id[counter].value,
                           msg->parameter,
                           INT256_LENGTH);
            counter++;
            context->next_param = LOCK_ID;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
void handle_provide_parameter(ethPluginProvideParameter_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: adapt the cases and the names of the functions.
    switch (context->selectorIndex) {
        case CLAIM_REGULAR_ACCOUNT:
            handle_claim_regular_account(msg, context);
            break;
        case CLAIM_MULTI_SIGNATURE_ACCOUNT:
            handle_claim_multisig_account(msg, context);
            break;
        case REWARD_CREATE_POSITION:
            handle_reward_create_position(msg, context);
            break;
        case REWARD_INIT_FAST_UNLOCK:
            handle_reward_init_fast_unlock(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
