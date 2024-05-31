#include "plugin.h"

uint16_t max_counter = 0;
uint16_t counter = 0;

static void copy_text(uint8_t *dst, uint16_t dst_len, uint16_t max_len, uint8_t *src) {
    size_t len = MIN(dst_len, max_len);
    memcpy(dst, src, len);
}

static void handle_claim_regular_account(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case PROOF:  // _proof
            context->next_param = PUBLIC_KEY;
            break;
        case PUBLIC_KEY:  // _pubKey
            copy_parameter(context->lisk.body.claim.public_key,
                           msg->parameter,
                           sizeof(context->lisk.body.claim.public_key));
            context->next_param = AMOUNT;
            break;
        case AMOUNT:  // _amount
            copy_parameter(context->lisk.body.claim.claim_amount,
                           msg->parameter,
                           sizeof(context->lisk.body.claim.claim_amount));
            context->next_param = RECIPIENT;
            break;
        case RECIPIENT:  // _recipient
            copy_address(context->lisk.body.claim.recipient,
                         msg->parameter,
                         sizeof(context->lisk.body.claim.recipient));
            context->next_param = SKIPED_PARAMETER;
            break;
        case SKIPED_PARAMETER:  // ED25519_SIGNATURE
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
            context->next_param = AMOUNT;
            break;
        case AMOUNT:  // _amount
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
            context->next_param = SKIPED_PARAMETER;
            break;
        case SKIPED_PARAMETER:  // ED25519_SIGNATURES
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
        case AMOUNT:  // amount
            copy_parameter(context->lisk.body.rewardCreatePosition.lock_amount,
                           msg->parameter,
                           sizeof(context->lisk.body.rewardCreatePosition.lock_amount));
            context->next_param = DURATION;
            break;
        case DURATION:  // lockingDuration
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

static void handle_lock_ids_array(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case OFFSET:
            context->next_param = LENGTH;
            break;
        case LENGTH:
            if (!U2BE_from_parameter(msg->parameter, &context->lisk.body.reward.lock_ids_len) ||
                context->lisk.body.reward.lock_ids_len > 4 ||
                context->lisk.body.reward.lock_ids_len == 0) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }

            context->next_param = ID;
            break;
        case ID:
            copy_parameter(context->lisk.body.reward.lock_id[counter].value,
                           msg->parameter,
                           INT256_LENGTH);
            if (counter == context->lisk.body.reward.lock_ids_len - 1) {
                counter = 0;
                context->next_param = NONE;
            } else {
                counter++;
            }
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_increase_locking_amount(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case OFFSET:
            context->next_param = LENGTH;
            break;
        case LENGTH:
            if (!U2BE_from_parameter(msg->parameter,
                                     &context->lisk.body.rewardIncLockingAmount.len) ||
                context->lisk.body.rewardIncLockingAmount.len > 2 ||
                context->lisk.body.rewardIncLockingAmount.len == 0) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }

            context->next_param = ID;
            break;
        case ID:
            copy_parameter(context->lisk.body.rewardIncLockingAmount.lock_id[counter].value,
                           msg->parameter,
                           INT256_LENGTH);
            context->next_param = AMOUNT;
            break;
        case AMOUNT:
            copy_parameter(context->lisk.body.rewardIncLockingAmount.amount[counter].value,
                           msg->parameter,
                           INT256_LENGTH);
            if (context->lisk.body.rewardIncLockingAmount.len > 1 &&
                counter < context->lisk.body.rewardIncLockingAmount.len - 1) {
                counter++;
                context->next_param = ID;
            } else {
                context->next_param = NONE;
            }
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_extend_duration(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case OFFSET:
            context->next_param = LENGTH;
            break;
        case LENGTH:
            if (!U2BE_from_parameter(msg->parameter,
                                     &context->lisk.body.rewardExtendDuration.len) ||
                context->lisk.body.rewardExtendDuration.len > 2 ||
                context->lisk.body.rewardExtendDuration.len == 0) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }

            context->next_param = ID;
            break;
        case ID:
            copy_parameter(context->lisk.body.rewardExtendDuration.lock_id[counter].value,
                           msg->parameter,
                           INT256_LENGTH);
            context->next_param = DURATION;
            break;
        case DURATION:
            copy_parameter(context->lisk.body.rewardExtendDuration.duration[counter].value,
                           msg->parameter,
                           INT256_LENGTH);
            if (context->lisk.body.rewardExtendDuration.len > 1 &&
                counter < context->lisk.body.rewardExtendDuration.len - 1) {
                counter++;
                context->next_param = ID;
            } else {
                context->next_param = NONE;
            }
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_add_unused_rewards(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case AMOUNT:
            copy_parameter(context->lisk.body.rewardAddUnusedRewards.amount,
                           msg->parameter,
                           sizeof(context->lisk.body.rewardAddUnusedRewards.amount));
            context->next_param = DURATION;
            break;
        case DURATION:
            copy_parameter(context->lisk.body.rewardAddUnusedRewards.duration,
                           msg->parameter,
                           sizeof(context->lisk.body.rewardAddUnusedRewards.duration));
            context->next_param = DELAY;
            break;
        case DELAY:
            copy_parameter(context->lisk.body.rewardAddUnusedRewards.delay,
                           msg->parameter,
                           sizeof(context->lisk.body.rewardAddUnusedRewards.delay));
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

static void handle_claim_airdrop(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case LSK_ADDRESS:  // liskAddress
            copy_parameter(context->lisk.body.claim.lsk_address,
                           msg->parameter,
                           sizeof(context->lisk.body.claim.lsk_address));
            context->next_param = AMOUNT;
            break;
        case AMOUNT:  // amount
            copy_parameter(context->lisk.body.claim.claim_amount,
                           msg->parameter,
                           sizeof(context->lisk.body.claim.claim_amount));
            context->next_param = SKIPED_PARAMETER;
            break;
        case SKIPED_PARAMETER:  // merkleProof
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_governor_cast_vote(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case ID:  // proposalId
            copy_parameter(context->lisk.body.governor.proposal_id, msg->parameter, INT256_LENGTH);
            context->next_param = SUPPORT;
            break;
        case SUPPORT:  // support
            copy_parameter(context->lisk.body.governor.support,
                           msg->parameter,
                           sizeof(context->lisk.body.governor.support));
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_governor_cast_vote_with_reason(ethPluginProvideParameter_t *msg,
                                                  context_t *context) {
    uint16_t tmp;
    switch (context->next_param) {
        case ID:  // proposalId
            copy_parameter(context->lisk.body.governor.proposal_id, msg->parameter, INT256_LENGTH);
            context->next_param = SUPPORT;
            break;
        case SUPPORT:  // support
            copy_parameter(context->lisk.body.governor.support,
                           msg->parameter,
                           sizeof(context->lisk.body.governor.support));
            context->next_param = OFFSET;
            break;
        case OFFSET:
            context->next_param = LENGTH;
            break;
        case LENGTH:  // reason
            if (!U2BE_from_parameter(msg->parameter, &tmp)) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            context->lisk.body.governor.reason.len = tmp;
            if (context->lisk.body.governor.reason.len % PARAMETER_LENGTH != 0) {
                counter = context->lisk.body.governor.reason.len / PARAMETER_LENGTH + 1;
            } else {
                counter = context->lisk.body.governor.reason.len / PARAMETER_LENGTH;
            }

            max_counter = counter;
            context->next_param = VALUE;
            break;
        case VALUE:
            if (counter == max_counter) {
                copy_text(context->lisk.body.governor.reason.value,
                          context->lisk.body.governor.reason.len,
                          PARAMETER_LENGTH,
                          (uint8_t *) msg->parameter);
            }

            counter--;
            if (counter == 0) {
                context->next_param = NONE;
            }
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_governor_propose(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }

    switch (context->next_param) {
        case OFFSET:
            context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - sizeof(context->offset));
            context->go_to_offset = true;
            context->next_param = LENGTH;
            break;
        case LENGTH:
            if (!U2BE_from_parameter(msg->parameter,
                                     &context->lisk.body.governorPropose.target_len) ||
                context->lisk.body.governorPropose.target_len > 2 ||
                context->lisk.body.governorPropose.target_len == 0) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }

            context->offset = msg->parameterOffset - SELECTOR_SIZE + PARAMETER_LENGTH;
            context->go_to_offset = true;
            context->next_param = TARGET_ADDRESS;
            break;
        case TARGET_ADDRESS:
            copy_address(context->lisk.body.governorPropose.targets[counter].value,
                         msg->parameter,
                         sizeof(context->lisk.body.governorPropose.targets[counter].value));
            if (counter + 1 < context->lisk.body.governorPropose.target_len) {
                counter++;
                context->next_param = SECOND_TARGET_ADDRESS;
            } else {
                context->next_param = PROPOSE_VALUE_LEN;
            }
            break;
        case SECOND_TARGET_ADDRESS:
            copy_address(context->lisk.body.governorPropose.targets[counter].value,
                         msg->parameter,
                         sizeof(context->lisk.body.governorPropose.targets[counter].value));
            counter = 0;
            context->next_param = PROPOSE_VALUE_LEN;
            break;
        case PROPOSE_VALUE_LEN:
            if (!U2BE_from_parameter(msg->parameter,
                                     &context->lisk.body.governorPropose.value_len) ||
                context->lisk.body.governorPropose.value_len > 2 ||
                context->lisk.body.governorPropose.value_len == 0 ||
                context->lisk.body.governorPropose.target_len !=
                    context->lisk.body.governorPropose.value_len) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }
            context->next_param = VALUE;
            break;
        case VALUE:
            copy_parameter(context->lisk.body.governorPropose.values[counter].value,
                           msg->parameter,
                           INT256_LENGTH);
            if (counter + 1 < context->lisk.body.governorPropose.value_len) {
                counter++;
                context->next_param = SECOND_VALUE;
            } else {
                context->next_param = NONE;
            }
            break;
        case SECOND_VALUE:
            copy_parameter(context->lisk.body.governorPropose.values[counter].value,
                           msg->parameter,
                           INT256_LENGTH);
            context->next_param = NONE;
            break;
        case NONE:
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
        case REWARD_CLAIM_REWARDS:
        case REWARD_PAUSE_UNLOCKING:
        case REWARD_RESUME_UNLOCKING:
        case REWARD_DELETE_POSITIONS:
            handle_lock_ids_array(msg, context);
            break;
        case REWARD_ADD_UNUSED_REWARDS:
        case REWARD_FUND_STAKING_REWARDS:
            handle_add_unused_rewards(msg, context);
            break;
        case REWARD_INC_LOCKING_AMOUNT:
            handle_increase_locking_amount(msg, context);
            break;
        case REWARD_EXTEND_DURATION:
            handle_extend_duration(msg, context);
            break;
        case CLAIM_AIRDROP:
            handle_claim_airdrop(msg, context);
            break;
        case GOVERNOR_CAST_VOTE:
            handle_governor_cast_vote(msg, context);
            break;
        case GOVERNOR_CAST_VOTE_WITH_REASON:
            handle_governor_cast_vote_with_reason(msg, context);
            break;
        case GOVERNOR_PROPOSE:
            handle_governor_propose(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
