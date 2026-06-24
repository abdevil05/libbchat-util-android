package network.loki.messenger.libbchat_util.util

import network.loki.messenger.libbchat_util.LibBChatUtilCApi


object MultiEncrypt : LibBChatUtilCApi() {

    const val KICKED_DOMAIN = "BChatGroupKickedMessage"

    external fun encryptForMultipleSimple(
        messages: Array<ByteArray>,
        recipients: Array<ByteArray>,
        ed25519SecretKey: ByteArray,
        domain: String
    ): ByteArray

    external fun decryptForMultipleSimple(
        encoded: ByteArray,
        ed25519SecretKey: ByteArray,
        senderPubKey: ByteArray,
        domain: String,
    ): ByteArray?
}