#include "test_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_GENERIC;

    // EDIT THIS: Set the total number of screen you will need.

    switch (context->selectorIndex) {
        case DEPOSIT:
        case WITHDRAW:
            msg->numScreens = 2;
            break;
        case APPROVE:
            msg->numScreens = 1;
            break;
    }



    // EDIT THIS: Handle this case like you wish to (i.e. maybe no additional screen needed?).
    // If the vault is NOT the sender, we will need an additional screen to display it.
//     if (memcmp(msg->address, context->vault, ADDRESS_LENGTH) != 0) {
//         msg->numScreens += 1;
//     }

    // EDIT THIS: set `tokenLookup1` (and maybe `tokenLookup2`) to point to
    // token addresses you will info for (such as decimals, ticker...).
    msg->tokenLookup1 = context->token_received;

    msg->result = ETH_PLUGIN_RESULT_OK;
}
