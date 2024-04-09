#include "plugin.h"

void handle_finalize(ethPluginFinalize_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_GENERIC;
    msg->numScreens = 2;
    // EDIT THIS: Handle this case like you wish to (i.e. maybe no additional screen needed?).
    // If the beneficiary is NOT the sender, we will need an additional screen to display it.
    if (memcmp(msg->address, context->beneficiary, ADDRESS_LENGTH) != 0) {
        msg->numScreens += 1;
    }

    switch (context->selectorIndex) {
        case SWAP_EXACT_ETH_FOR_TOKENS:
            if (memcmp(msg->address, context->beneficiary, ADDRESS_LENGTH) != 0) {
                msg->numScreens += 1;
            }
            msg->tokenLookup1 = context->token_received;
            break;
        case CLAIM_REGULAR_ACCOUNT:
            break;
        case CLAIM_MULTI_SIGNATURE_ACCOUNT:
            break;
        default:
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
