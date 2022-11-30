#include "test_plugin.h"

static void handle_deposit_and_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
        if (context->go_to_offset) {
                if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
                    return;
                }
                context->go_to_offset = false;
            }
        switch (context->next_param) {
            case PATH_OFFSET:  // vault strategies
                 context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - sizeof(context->offset));
                 context->next_param = AMOUNT;
                 break;
            case PATH_LENGTH:
                 context->offset = msg->parameterOffset - SELECTOR_SIZE + PARAMETER_LENGTH *1 ;
                 context->go_to_offset = true;
                 context->next_param = UNEXPECTED_PARAMETER;
                 break;
            case TOKEN_RECEIVED:
                copy_address(context->token_received,
                            sizeof(context->token_received),
                            msg->parameter);
                context->next_param = UNEXPECTED_PARAMETER;
                break;

            case AMOUNT:  // amount to deposit
                 copy_parameter(context->amount,
                                msg->parameter,
                                sizeof(context->amount));
                 context->next_param = PATH_LENGTH;
                 context->go_to_offset = true;
//                  context->next_param = BOOL_OPTION;
//                  context->next_param = UNEXPECTED_PARAMETER;
                 break;
//             case BOOL_OPTION:
//                  copy_parameter(context->bool_option,
//                                 msg->parameter,
//                                 sizeof(context->bool_option));
//                  context->next_param = PATH_LENGTH;
//                  context->go_to_offset = true;
//                  context->next_param = UNEXPECTED_PARAMETER;
//                  break;
            // Keep this
            default:
                PRINTF("Param not supported: %d\n", context->next_param);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
        }
}


static void handle_approve(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
            if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
                return;
            }
            context->go_to_offset = false;
        }
        switch (context->next_param) {
            case ADDRESS:
                 copy_address(context->vault,
                             sizeof(context->vault),
                             msg->parameter);
                 context->next_param = AMOUNT;
                 break;
            case AMOUNT:  // amount to deposit
                 copy_parameter(context->amount,
                                msg->parameter,
                                sizeof(context->amount));
                 context->next_param = UNEXPECTED_PARAMETER;
                 break;
            // Keep this
            default:
                PRINTF("Param not supported: %d\n", context->next_param);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
        }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
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
        case DEPOSIT:
        case WITHDRAW:
            handle_deposit_and_withdraw(msg, context);
            break;
//         case APPROVE:
//             handle_approve(msg, context);
//             break;
//         case WITHDRAW_FAST:
//             handle_deposit_and_withdraw(msg, context);
//             break;
//         case GET_REWARDS:
//             handle_deposit_and_withdraw(msg, context);
//             break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}