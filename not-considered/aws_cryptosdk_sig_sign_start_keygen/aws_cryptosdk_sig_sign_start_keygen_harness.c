/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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
#include <ec_utils.h>
#include <evp_utils.h>
#include <make_common_data_structures.h>
#include <proof_helpers/proof_allocators.h>

#include <cipher_openssl.h>

int main() {
    /* arguments */
    struct aws_cryptosdk_sig_ctx *pctx;
    struct aws_allocator *alloc = can_fail_allocator();
    struct aws_string *pub_key;
    enum aws_cryptosdk_alg_id alg_id = nondet_alg_id();
    struct aws_cryptosdk_alg_properties *props = aws_cryptosdk_alg_props(alg_id);

    /* assumptions */
    __CPROVER_assume(props);

    bool save_pub_key = nondet_bool();

    /* operation under verification */
    if (aws_cryptosdk_sig_sign_start_keygen(&pctx, alloc, save_pub_key ? &pub_key : NULL, props) == AWS_OP_SUCCESS) {
        /* assertion: on success, context is initialized unless no curve name was given */
        assert((!props->impl->curve_name && !pctx) || (aws_cryptosdk_sig_ctx_is_valid_cbmc(pctx) && pctx->is_sign));
    }

    /* assertions */
    if (save_pub_key) assert(!pub_key || aws_string_is_valid(pub_key));
    return 0;
}
