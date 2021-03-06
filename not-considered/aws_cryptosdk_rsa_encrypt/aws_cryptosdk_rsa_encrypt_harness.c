/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include <aws/cryptosdk/cipher.h>
#include <aws/cryptosdk/private/cipher.h>
#include <ec_utils.h>
#include <make_common_data_structures.h>

#define KEY_LEN 256

int main() {
    /* Nondet Inputs */
    struct aws_byte_buf cipher;
    struct aws_allocator *alloc = can_fail_allocator();
    struct aws_byte_cursor plain;
    struct aws_string *key = ensure_string_is_allocated_nondet_length();
    enum aws_cryptosdk_rsa_padding_mode rsa_padding_mode = nondet_rsa_padding_mode();

    /* Assumptions */
    assume(aws_string_is_valid(key));
    assume(key->len <= KEY_LEN);

    ensure_byte_buf_has_allocated_buffer_member(&cipher);
    assume(aws_byte_buf_is_bounded(&cipher, MAX_BUFFER_SIZE));
    assume(aws_byte_buf_is_valid(&cipher));

    ensure_byte_cursor_has_allocated_buffer_member(&plain);
    assume(aws_byte_cursor_is_bounded(&plain, MAX_BUFFER_SIZE));
    assume(aws_byte_cursor_is_valid(&plain));

    /* Save current state of the data structure */
    struct aws_byte_cursor old_plain = plain;
    struct store_byte_from_buffer old_byte_from_plain;
    save_byte_from_array(plain.ptr, plain.len, &old_byte_from_plain);

    /* Initialize a nondeterministic but fixed max encryption size between 0 and INT_MAX */
    initialize_max_encryption_size();

    /* Operation under verification */
    if (aws_cryptosdk_rsa_encrypt(&cipher, alloc, plain, key, rsa_padding_mode) == AWS_OP_SUCCESS) {
        /* Post-conditions. */
        assert(aws_byte_buf_is_valid(&cipher));
    }
    /* Post-conditions. */
    if (plain.len != 0) {
        assert_byte_from_buffer_matches(plain.ptr, &old_byte_from_plain);
    }
    return 0;
}
