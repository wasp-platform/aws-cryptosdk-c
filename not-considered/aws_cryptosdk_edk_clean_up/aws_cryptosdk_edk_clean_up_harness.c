/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may not
 * use
 * this file except in compliance with the License. A copy of the License is
 * located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed on
 * an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or
 * implied. See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <aws/cryptosdk/edk.h>
#include <proof_helpers/cryptosdk/make_common_data_structures.h>
#include <proof_helpers/make_common_data_structures.h>
#include <proof_helpers/proof_allocators.h>
#include <proof_helpers/utils.h>

int main() {
    struct aws_array_list edk_list;

    struct aws_cryptosdk_edk edk;  // Precondition: non-null
    ensure_cryptosdk_edk_has_allocated_members(&edk);
    assert(aws_cryptosdk_edk_is_valid(&edk));  // Precondition: is_valid()

    aws_cryptosdk_edk_clean_up(&edk);
    assert(aws_cryptosdk_edk_is_valid(&edk));
    return 0;
}
