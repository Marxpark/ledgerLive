#include "test_plugin.h"

// EDIT THIS: You need to adapt / remove the static functions (set_send_ui, set_receive_ui ...) to
// match what you wish to display.


static void set_deposit_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Deposit", msg->titleLength);

    uint8_t decimals = context->decimals;
   const char *ticker = context->ticker;

   // If the token look up failed, use the default network ticker along with the default decimals.
   if (!context->token_found) {
       decimals = WEI_TO_ETHER;
       ticker = msg->network_ticker;
   }

   amountToString(context->amount,
                  sizeof(context->amount),
                  decimals,
                  ticker,
                  msg->msg,
                  msg->msgLength);
}


static void set_withdraw_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Withdraw", msg->titleLength);

    uint8_t decimals = context->decimals;
    const char *ticker = context->ticker;

    // If the token look up failed, use the default network ticker along with the default decimals.
    if (!context->token_found) {
        decimals = WEI_TO_ETHER;
        ticker = msg->network_ticker;
    }

    amountToString(context->amount,
                   sizeof(context->amount),
                   decimals,
                   ticker,
                   msg->msg,
                   msg->msgLength);
}

// Set UI for "vault" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static void set_vault_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Vault", msg->titleLength);

    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    // Get the string representation of the address stored in `context->vault`. Put it in
    // `msg->msg`.
    getEthAddressStringFromBinary(
        context->vault,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        msg->pluginSharedRW->sha3,
        chainid);
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: Adapt the cases for the screens you'd like to display.
    switch (msg->screenIndex) {
        case 0:
            switch (context->selectorIndex) {
                case DEPOSIT:
                    set_deposit_ui(msg, context);
                    break;
                case WITHDRAW:
                    set_withdraw_ui(msg, context);
                    break;
                case APPROVE:
                    set_withdraw_ui(msg, context);
                    break;
                default:
                  PRINTF("Received an invalid screenIndex\n");
                  msg->result = ETH_PLUGIN_RESULT_ERROR;
                  return;
            }
        case 1:
            set_vault_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
