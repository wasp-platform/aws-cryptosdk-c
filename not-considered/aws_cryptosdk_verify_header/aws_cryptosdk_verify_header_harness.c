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

#include <aws/cryptosdk/cipher.h>
#include <aws/cryptosdk/private/cipher.h>
#include <make_common_data_structures.h>

int main() {
    /* arguments */
    enum aws_cryptosdk_alg_id alg_id = nondet_alg_id();

    struct aws_cryptosdk_alg_properties *props = aws_cryptosdk_alg_props(alg_id);

    struct content_key *c_key;
    struct aws_byte_buf *authtag = can_fail_malloc(sizeof(struct aws_byte_buf));
    struct aws_byte_buf *header = can_fail_malloc(sizeof(struct aws_byte_buf));

    /* assumptions*/
    __CPROVER_assume(aws_cryptosdk_alg_properties_is_valid(props));

    ensure_byte_buf_has_allocated_buffer_member(authtag);
    __CPROVER_assume(aws_byte_buf_is_bounded(authtag, 2));
    __CPROVER_assume(aws_byte_buf_is_valid(authtag));

    ensure_byte_buf_has_allocated_buffer_member(header);
    __CPROVER_assume(aws_byte_buf_is_bounded(header, 2));
    __CPROVER_assume(aws_byte_buf_is_valid(header));

    /* save current state of the data structure */
    struct aws_byte_buf old_header;
    old_header.len = header->len;
    old_header.capacity = header->capacity;
    old_header.allocator = header->allocator;
    struct store_byte_from_buffer old_byte_from_header;
    save_byte_from_array(header->buffer, header->len, &old_byte_from_header);

    aws_cryptosdk_verify_header(props, c_key, authtag, header);

    /* assertions */
    assert(aws_byte_buf_is_valid(header));
    assert(aws_byte_buf_is_valid(authtag));
    assert_byte_buf_equivalence(header, &old_header, &old_byte_from_header);
    return 0;
}
