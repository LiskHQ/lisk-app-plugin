#include "plugin.h"

// EDIT THIS: You need to adapt / remove the static functions (set_send_ui, set_receive_ui ...) to
// match what you wish to display.

// Set UI for the "Send" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static bool set_send_ui(ethQueryContractUI_t *msg) {
    strlcpy(msg->title, "Send", msg->titleLength);

    const uint8_t *eth_amount = msg->pluginSharedRO->txContent->value.value;
    uint8_t eth_amount_size = msg->pluginSharedRO->txContent->value.length;

    // Converts the uint256 number located in `eth_amount` to its string representation and
    // copies this to `msg->msg`.
    return amountToString(eth_amount,
                          eth_amount_size,
                          WEI_TO_ETHER,
                          "ETH",
                          msg->msg,
                          msg->msgLength);
}

// Set UI for "Receive" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static bool set_receive_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Receive Min.", msg->titleLength);

    uint8_t decimals = context->decimals;
    const char *ticker = context->ticker;

    // If the token look up failed, use the default network ticker along with the default decimals.
    if (!context->token_found) {
        decimals = WEI_TO_ETHER;
        ticker = msg->network_ticker;
    }

    return amountToString(context->lisk.body.swap.amount_received,
                          sizeof(context->lisk.body.swap.amount_received),
                          decimals,
                          ticker,
                          msg->msg,
                          msg->msgLength);
}

// Set UI for "Beneficiary" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static bool set_beneficiary_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Beneficiary", msg->titleLength);

    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    // Get the string representation of the address stored in `context->beneficiary`. Put it in
    // `msg->msg`.
    return getEthAddressStringFromBinary(
        context->lisk.body.swap.beneficiary,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        chainid);
}

// Set UI for "Claim LSK" screen.
// EDIT THIS: Adapt / remove this function to your needs.
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

// Set UI for "Sender" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static bool set_sender_ui(ethQueryContractUI_t *msg, context_t *context) {
    if (sizeof(context->lisk.body.claim.public_key) != 0) {
        strlcpy(msg->title, "Sender Lisk Public Key", msg->titleLength);
        array_hexstr(msg->msg, context->lisk.body.claim.public_key, msg->msgLength);
    } else {
        strlcpy(msg->title, "Sender Lisk Address", msg->titleLength);
        array_hexstr(msg->msg, context->lisk.body.claim.lsk_address, msg->msgLength);
    }
    return true;
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
        case SWAP_EXACT_ETH_FOR_TOKENS:
            switch (msg->screenIndex) {
                case 0:
                    ret = set_send_ui(msg);
                    break;
                case 1:
                    ret = set_receive_ui(msg, context);
                    break;
                case 2:
                    ret = set_beneficiary_ui(msg, context);
                    break;
                    // Keep this
                default:
                    PRINTF("Received an invalid screenIndex\n");
            }
            break;
        case CLAIM_REGULAR_ACCOUNT:
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
                    ret = set_sender_ui(msg, context);
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
