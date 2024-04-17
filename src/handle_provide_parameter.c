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
            copy_address(context->lisk.body.claim.lsk_address,
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
        // TODO: Verify if we want to show multi-sig account keys on device
        case MULTISIG_KEYS:  // _keys
            context->next_param = RECIPIENT;
            break;
        case RECIPIENT:  // _recipient
            copy_address(context->lisk.body.claim.recipient,
                         msg->parameter,
                         sizeof(context->lisk.body.claim.recipient));
            context->next_param = ED25519_SIGNATURE;
            break;
        case ED25519_SIGNATURES:  // _sigs
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
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

    switch (context->selectorIndex) {
        case CLAIM_REGULAR_ACCOUNT:
            handle_claim_regular_account(msg, context);
            break;
        case CLAIM_MULTI_SIGNATURE_ACCOUNT:
            handle_claim_multisig_account(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
