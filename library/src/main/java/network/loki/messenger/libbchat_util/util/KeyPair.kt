package network.loki.messenger.libbchat_util.util

import androidx.annotation.Keep

data class KeyPair(val pubKey: Bytes, val secretKey: Bytes) {
    @Keep
    constructor(pubKey: ByteArray, secretKey: ByteArray)
        : this(Bytes(pubKey), Bytes(secretKey))
}