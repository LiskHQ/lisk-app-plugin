#include "plugin.h"

// Set UI for "Claim LSK" screen.
static bool set_claim_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Claim LSK", msg->titleLength);

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
    array_hexstr(msg->msg, context->lisk.body.claim.public_key, PUBLIC_KEY_LENGTH);
    return true;
}

// Set UI for "Sender Address" screen.
static bool set_sender_address_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Sender Lisk Address", msg->titleLength);
    array_hexstr(msg->msg, context->lisk.body.claim.lsk_address, LISK_ADDRESS_LENGTH);
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

void handle_query_contract_ui(ethQueryContractUI_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    bool ret = false;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

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
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            ret = false;
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
