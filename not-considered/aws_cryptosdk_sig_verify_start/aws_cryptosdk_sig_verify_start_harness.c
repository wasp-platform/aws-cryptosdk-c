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

#include <aws/cryptosdk/cipher.h>
#include <cbmc_invariants.h>
#include <cipher_openssl.h>
#include <ec_utils.h>
#include <evp_utils.h>
#include <make_common_data_structures.h>
#include <proof_allocators.h>
#include <proof_helpers/proof_allocators.h>

int main() {
    /* Nondet input */
    struct aws_cryptosdk_sig_ctx *ctx;
    struct aws_allocator *alloc = can_fail_allocator();
    struct aws_string *pub_key  = ensure_string_is_allocated_nondet_length();
    enum aws_cryptosdk_alg_id alg_id = nondet_alg_id();
    struct aws_cryptosdk_alg_properties *props = aws_cryptosdk_alg_props(alg_id);

    /* Assumptions */
    __CPROVER_assume(aws_cryptosdk_alg_properties_is_valid(props));
    __CPROVER_assume(aws_string_is_valid(pub_key));

    /* Operation under verification */
    if (aws_cryptosdk_sig_verify_start(&ctx, alloc, pub_key, props) == AWS_OP_SUCCESS) {
        /* Post-conditions */
        assert((!props->impl->curve_name && !ctx) || (aws_cryptosdk_sig_ctx_is_valid(ctx) && !ctx->is_sign));
    }

    /* Post-conditions */
    assert(aws_string_is_valid(pub_key));
    return 0;
}
