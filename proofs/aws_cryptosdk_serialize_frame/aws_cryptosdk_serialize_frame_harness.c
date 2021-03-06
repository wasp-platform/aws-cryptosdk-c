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

#include <aws/common/byte_buf.h>
#include <aws/cryptosdk/private/framefmt.h>
#include <make_common_data_structures.h>
#include <proof_helpers/make_common_data_structures.h>

extern int __VERIFIER_nondet_int(char *);

int main() {
    /* data structure */
    struct aws_cryptosdk_frame frame;
    size_t ciphertext_size = __VERIFIER_nondet_int("ciphertext_size");
    size_t plaintext_size = __VERIFIER_nondet_int("plaintext_size");
    struct aws_byte_buf ciphertext_buf;
    struct aws_cryptosdk_alg_properties *props = ensure_alg_properties_attempt_allocation(MAX_STRING_LEN);

    /* Assumptions about the function input */
    ensure_byte_buf_has_allocated_buffer_member(&ciphertext_buf);
    assume(aws_byte_buf_is_valid(&ciphertext_buf));
    assume(aws_cryptosdk_frame_has_valid_type(&frame));
    assume(aws_cryptosdk_alg_properties_is_valid(props));

    /* Save the old state of the ciphertext buffer */
    uint8_t *old_ciphertext_buffer   = ciphertext_buf.buffer;
    size_t old_ciphertext_buffer_len = ciphertext_buf.len;

    if (aws_cryptosdk_serialize_frame(&frame, &ciphertext_size, plaintext_size, &ciphertext_buf, props) ==
        AWS_OP_SUCCESS) {
        assert(aws_cryptosdk_frame_is_valid(&frame));
        assert(aws_cryptosdk_alg_properties_is_valid(props));
        assert(aws_cryptosdk_frame_serialized(&frame, props, plaintext_size));
        assert(ciphertext_buf.buffer == old_ciphertext_buffer);
        assert(ciphertext_buf.len == old_ciphertext_buffer_len + ciphertext_size);
    } else {
        // Assert that the ciphertext buffer is zeroed in case of failure
        assert_all_zeroes(ciphertext_buf.buffer, ciphertext_buf.capacity);
        assert(ciphertext_buf.len == 0);
    }
    return 0;
}
