/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may not use
 * this file except in compliance with the License. A copy of the License is
 * located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <aws/cryptosdk/default_cmm.h>

#include <proof_helpers/make_common_data_structures.h>

/**
 * Stub for the virtual function on_encrypt of a aws_cryptosdk_keyring_vt structure.
 * Must implement if used for encryption.
 * It should be as nondeterministic as possible to increase coverage on the operation under
 * verfication; thus, it also triggers states with errors.
 *
 * When the buffer for the unencrypted data key is not NULL at the time of the call, it
 * must not be changed by callee. All buffers for EDKs pushed onto the list must be in a
 * valid state, which means either that they are set to all zeroes or that they have been
 * initialized using one of the byte buffer initialization functions. This assures proper
 * clean up and serialization.
 */
int stub_on_encrypt(
    struct aws_cryptosdk_keyring *keyring,
    struct aws_allocator *request_alloc,
    struct aws_byte_buf *unencrypted_data_key,
    struct aws_array_list *keyring_trace,
    struct aws_array_list *edks,
    const struct aws_hash_table *enc_ctx,
    enum aws_cryptosdk_alg_id alg) {
    /* Check validity for all inputs to avoid memory-safety violations. */
    assert(aws_cryptosdk_keyring_is_valid(keyring));
    assert(aws_allocator_is_valid(request_alloc));
    assert(aws_byte_buf_is_valid(unencrypted_data_key));
    assert(aws_cryptosdk_keyring_trace_is_valid(keyring_trace));
    assert(aws_cryptosdk_edk_list_is_valid(edks));
    assert(aws_cryptosdk_edk_list_elements_are_valid(edks));
    assert((enc_ctx == NULL) || aws_hash_table_is_valid(enc_ctx));

    if (unencrypted_data_key->buffer == NULL) {
        const struct aws_cryptosdk_alg_properties *props = aws_cryptosdk_alg_props(alg);
        if (props != NULL) {
            __CPROVER_assume(aws_byte_buf_is_bounded(unencrypted_data_key, props->data_key_len));
            ensure_byte_buf_has_allocated_buffer_member(unencrypted_data_key);
            /*
             * This satisfies the post-condition that if this keyring
             * generated data key, it must be the right length.
             * The nondeterminism increases coverage.
             */
            if (nondet_bool()) unencrypted_data_key->len = props->data_key_len;
            unencrypted_data_key->allocator = request_alloc;
            __CPROVER_assume(aws_byte_buf_is_valid(unencrypted_data_key));
        }
    } else {
        /*
         * If the buffer is not NULL, the byte buffer must not have been modified;
         * however, this allocation modifies the buffer and increases coverage.
         */
        if (nondet_bool()) ensure_byte_buf_has_allocated_buffer_member(unencrypted_data_key);
    }
    return nondet_int();
}
