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
#include <aws/cryptosdk/private/cipher.h>

extern enum aws_cryptosdk_alg_id nondet_alg_id();

int main() {
    /* Nondet Inputs. */
    enum aws_cryptosdk_alg_id alg_id = nondet_alg_id();

    /* Operation under verification*/
    struct aws_cryptosdk_alg_properties *props = aws_cryptosdk_alg_props(alg_id);

    /* Post-conditions. */
    if (props) {
        assert(aws_cryptosdk_alg_properties_is_valid(props));
        assert(
            props->impl->md_ctor == NULL || props->impl->md_ctor == EVP_sha256 || props->impl->md_ctor == EVP_sha384 ||
            props->impl->md_ctor == EVP_sha512);
        assert(
            props->impl->cipher_ctor == NULL || props->impl->cipher_ctor == EVP_aes_128_gcm ||
            props->impl->cipher_ctor == EVP_aes_192_gcm || props->impl->cipher_ctor == EVP_aes_256_gcm);
    }
    return 0;
}
