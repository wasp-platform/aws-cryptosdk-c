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

#include <aws/cryptosdk/materials.h>
#include <make_common_data_structures.h>

int main() {
    /* Nondet input. */
    struct aws_cryptosdk_keyring *keyring   = malloc(sizeof(*keyring));
    struct aws_cryptosdk_keyring_vt *vtable = malloc(sizeof(*vtable));

    /* Assumptions. */
    assert(keyring != NULL);
    ensure_nondet_allocate_keyring_vtable_members(vtable, MAX_STRING_LEN);
    assert(aws_cryptosdk_keyring_vt_is_valid(vtable));

    /* Operation under verification. */
    aws_cryptosdk_keyring_base_init(keyring, vtable);

    /* Post-conditions. */
    assert(aws_cryptosdk_keyring_is_valid(keyring));
    assert(IMPLIES(vtable != NULL, aws_cryptosdk_keyring_vt_is_valid(vtable)));
    return 0;
}
