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

#include <aws/cryptosdk/private/keyring_trace.h>
#include <make_common_data_structures.h>
#include <proof_helpers/make_common_data_structures.h>

int main() {
    /* data structure */
    struct aws_cryptosdk_keyring_trace_record source_record; /* Precondition: record is non-null */
    struct aws_cryptosdk_keyring_trace_record dest_record;   /* Precondition: record is non-null */
    struct aws_allocator *alloc = can_fail_allocator();      /* Precondition: alloc must be non-null */

    source_record.wrapping_key_namespace = ensure_string_is_allocated_nondet_length();
    assume(aws_string_is_valid(source_record.wrapping_key_namespace));
    assume(source_record.wrapping_key_namespace->len <= MAX_STRING_LEN);

    source_record.wrapping_key_name = ensure_string_is_allocated_nondet_length();
    assume(aws_string_is_valid(source_record.wrapping_key_name));
    assume(source_record.wrapping_key_name->len <= MAX_STRING_LEN);


    assert(aws_cryptosdk_keyring_trace_record_init_clone(alloc, &dest_record, &source_record) == AWS_OP_SUCCESS);
    assert(aws_string_eq(source_record.wrapping_key_namespace, dest_record.wrapping_key_namespace));
    assert(aws_string_eq(source_record.wrapping_key_name, dest_record.wrapping_key_name));
    assert(source_record.flags == dest_record.flags);
    return 0;
}
