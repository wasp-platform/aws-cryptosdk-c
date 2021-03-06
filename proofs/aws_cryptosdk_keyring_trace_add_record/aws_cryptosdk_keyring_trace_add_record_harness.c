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

#include <aws/common/array_list.h>
#include <aws/common/string.h>
#include <aws/cryptosdk/keyring_trace.h>
#include <aws/cryptosdk/private/keyring_trace.h>
#include <aws/cryptosdk/private/utils.h>
#include <make_common_data_structures.h>
#include <proof_helpers/make_common_data_structures.h>
#include <proof_helpers/utils.h>

int main() {
    /* data structure */
    struct aws_allocator *alloc = can_fail_allocator();                        /* Precondition: valid allocator */
    assert(aws_allocator_is_valid(alloc));

    struct aws_array_list trace;                                               /* Precondition: trace must be non-null */
    struct aws_string *namespace = ensure_string_is_allocated_nondet_length(); /* Precondition: namespace must be non-null */
    struct aws_string *name = ensure_string_is_allocated_nondet_length();      /* Precondition: name must be non-null */
    uint32_t flags;

    assert(aws_string_is_valid(namespace));
    assert(aws_string_is_valid(name));

    ensure_array_list_has_allocated_data_member(&trace);
    ensure_trace_has_allocated_records(&trace, SIZE_MAX);
    assert(
        aws_array_list_is_bounded(&trace, MAX_ITEM_SIZE, sizeof(struct aws_cryptosdk_keyring_trace_record)));
    assert(trace.item_size == sizeof(struct aws_cryptosdk_keyring_trace_record));
    assert(aws_array_list_is_valid(&trace));
    assert(aws_cryptosdk_keyring_trace_is_valid(&trace));

    struct aws_array_list old = trace;
    struct store_byte_from_buffer old_byte;
    save_byte_from_array((uint8_t *)trace.data, trace.current_size, &old_byte);

    if (aws_cryptosdk_keyring_trace_add_record(alloc, &trace, namespace, name, flags) == AWS_OP_SUCCESS) {
        /* assertions */
        // TODO: we should actually perform memcpy and not memcpy_havoc
        // to perform this validity check - assert(aws_cryptosdk_keyring_trace_is_valid(&trace));
        assert(trace.length = old.length + 1);
    } else {
        /* assertions */
        assert_array_list_equivalence(&trace, &old, &old_byte);
    }
    return 0;
}
