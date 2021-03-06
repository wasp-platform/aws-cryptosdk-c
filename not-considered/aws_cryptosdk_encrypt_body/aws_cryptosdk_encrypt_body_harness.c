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
#include <make_common_data_structures.h>

extern int __VERIFIER_nondet_int(char *);
extern char __VERIFIER_nondet_char(char *);

int main() {
    /* Non-deterministic inputs. */
    enum aws_cryptosdk_alg_id alg_id = nondet_alg_id();
    struct aws_cryptosdk_alg_properties *props = aws_cryptosdk_alg_props(alg_id);
    assert(aws_cryptosdk_alg_properties_is_valid(props));

    struct aws_byte_buf *outp = can_fail_malloc(sizeof(*outp));
    assert(outp != NULL);
    ensure_byte_buf_has_allocated_buffer_member(outp);
    assert(aws_byte_buf_is_bounded(outp, MAX_BUFFER_SIZE));
    assert(aws_byte_buf_is_valid(outp));

    struct aws_byte_cursor *inp = can_fail_malloc(sizeof(*inp));
    assert(inp != NULL);
    ensure_byte_cursor_has_allocated_buffer_member(inp);
    assert(aws_byte_cursor_is_bounded(inp, MAX_BUFFER_SIZE));
    assert(aws_byte_cursor_is_valid(inp));

    struct aws_byte_buf *message_id = can_fail_malloc(sizeof(*message_id));
    assert(message_id != NULL);
    ensure_byte_buf_has_allocated_buffer_member(message_id);
    assert(aws_byte_buf_is_bounded(message_id, MAX_BUFFER_SIZE));
    assert(aws_byte_buf_is_valid(message_id));

    uint32_t seqno = __VERIFIER_nondet_int("seqno");

    uint8_t *iv = can_fail_malloc(props->iv_len);
    assert(iv != NULL);

    struct content_key *content_key;
    int i;
    for (i = 0; i < 32; ++i)
      content_key->keybuf[i] = __VERIFIER_nondet_char("content_key");
    content_key->keybuf[i] = '\0';

    /* Need to allocate tag_len bytes for writing the tag */
    uint8_t *tag = can_fail_malloc(props->tag_len);
    assert(tag != NULL);

    int body_frame_type = __VERIFIER_nondet_int("body_frame_type");

    /* save current state of outp */
    struct aws_byte_cursor old_inp = *inp;

    struct aws_byte_buf old_outp = *outp;
    struct store_byte_from_buffer old_byte;
    save_byte_from_array(outp->buffer, outp->len, &old_byte);

    struct aws_byte_buf old_message_id = *message_id;
    struct store_byte_from_buffer old_message_id_byte;
    save_byte_from_array(message_id->buffer, message_id->len, &old_message_id_byte);

    /* Operation under verification. */
    if (aws_cryptosdk_encrypt_body(props, outp, inp, message_id, seqno, iv, content_key, tag, body_frame_type) ==
        AWS_OP_SUCCESS) {
        assert(inp->len == old_outp.capacity - old_outp.len);
        assert(outp->len >= old_outp.len && outp->len <= old_outp.len + inp->len);
    } else {
        assert(inp->len == old_inp.len);
        assert(outp->len == old_outp.len || outp->len == 0);
    }

    /* Post-conditions. */
    assert(aws_cryptosdk_alg_properties_is_valid(props));
    assert(aws_byte_buf_is_valid(outp));
    assert(aws_byte_cursor_is_valid(inp));
    assert(aws_byte_buf_is_valid(message_id));
    assert_byte_buf_equivalence(message_id, &old_message_id, &old_message_id_byte);
    return 0;
}
