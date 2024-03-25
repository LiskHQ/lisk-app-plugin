#include "plugin.h"

static void handle_claim_regular_account(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        // TODO: Verify if we want to show proof on device
        case PROOF:  // _proof
            context->next_param = PUBLIC_KEY;
            break;
        case PUBLIC_KEY:  // _pubKey
            copy_parameter(context->public_key,
                       sizeof(context->public_key),
                       msg->parameter);
            context->next_param = CLAIM_AMOUNT;
            break;
        case CLAIM_AMOUNT:  // _amount
            copy_parameter(context->claim_amount,
                       sizeof(context->claim_amount),
                       msg->parameter);
            context->next_param = RECIPIENT;
            break;
        case RECIPIENT: // _recipient
            copy_address(context->recipient, msg->parameter, sizeof(context->recipient));
            context->next_param = ED25519_SIGNATURE;
            break;
        // TODO: Verify if we want to show signature on device
        case ED25519_SIGNATURE:  // _sig
            context->next_param = UNEXPECTED_PARAMETER;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_claim_multisig_account(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        // TODO: Verify if we want to show proof on device
        case PROOF:  // _proof
            context->next_param = LSK_ADDRESS;
            break;
        case LSK_ADDRESS:  // _lskAddress
            copy_address(context->lsk_address, msg->parameter, sizeof(context->lsk_address));
            context->next_param = CLAIM_AMOUNT;
            break;
        case CLAIM_AMOUNT:  // _amount
            copy_parameter(context->claim_amount,
                       sizeof(context->claim_amount),
                       msg->parameter);
            context->next_param = MULTISIG_KEYS;
            break;
        // TODO: Verify if we want to show multi-sig account keys on device
        case MULTISIG_KEYS:  // _keys
            context->next_param = RECIPIENT;
            break;
        case RECIPIENT: // _recipient
            copy_address(context->recipient, msg->parameter, sizeof(context->recipient));
            context->next_param = ED25519_SIGNATURE;
            break;
        // TODO: Verify if we want to show signatures on device
        case ED25519_SIGNATURES:  // _sigs
            context->next_param = UNEXPECTED_PARAMETER;
        // Keep this
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

    // EDIT THIS: adapt the cases and the names of the functions.
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
