/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <aws/cryptosdk/private/cipher.h>
#include <cbmc_invariants.h>
#include <cipher_openssl.h>
#include <evp_utils.h>
#include <make_common_data_structures.h>
#include <proof_helpers/proof_allocators.h>
#include <proof_helpers/utils.h>

/* Expected runtime 1m30s */
int main() {
    /* arguments */
    struct aws_cryptosdk_md_context *md_context = can_fail_malloc(sizeof(*md_context));
    size_t length = __VERIFIER_nondet_uchar("length");
    void *buf = can_fail_malloc(length);

    /* assumptions */
    assume(md_context);
    ensure_md_context_has_allocated_members(md_context);
    assume(evp_md_ctx_get0_evp_pkey(md_context->evp_md_ctx) == NULL);
    assume(aws_cryptosdk_md_context_is_valid_cbmc(md_context));
    assume(buf);

    struct store_byte_from_buffer old_byte;
    save_byte_from_array(buf, length, &old_byte);

    /* operation under verification */
    if (aws_cryptosdk_md_update(md_context, buf, length) == AWS_OP_SUCCESS) {
        /* assertions */
        assert(aws_cryptosdk_md_context_is_valid_cbmc(md_context));
    }

    if (length > 0) {
        assert_byte_from_buffer_matches(buf, &old_byte);
    }

    /* clean up (necessary because we are checking for memory leaks) */
    EVP_MD_CTX_free(md_context->evp_md_ctx);
    free(md_context);
    free(buf);
    return 0;
}
