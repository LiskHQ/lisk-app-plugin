#include "plugin.h"

bool getLiskAddressFromPublicKey(uint8_t *publicKey) {
    return true;
}

// Set UI for "Claim LSK" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static bool set_claim_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Claim LSK", msg->titleLength);

    uint8_t decimals = 18;
    const char *ticker = "LSK";

    return amountToString(context->claim_amount,
                          sizeof(context->claim_amount),
                          decimals,
                          ticker,
                          msg->msg,
                          msg->msgLength);
}

// Set UI for "Sender" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static bool set_sender_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Sender Lisk Address", msg->titleLength);

    // TODO: Implement utility method
    return getLiskAddressFromPublicKey(context->public_key);
}

// Set UI for "Recipient" screen.
// EDIT THIS: Adapt / remove this function to your needs.
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
        context->recipient,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        msg->pluginSharedRW->sha3,
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
    switch (msg->screenIndex) {
        case 0:
            ret = set_claim_ui(msg, context);
            break;
        case 1:
            ret = set_sender_ui(msg, context);
            break;
        case 2:
            ret = set_recipient_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
