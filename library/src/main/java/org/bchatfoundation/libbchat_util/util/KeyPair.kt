package org.bchatfoundation.libbchat_util.util

import androidx.annotation.Keep

class KeyPair(val pubKey: Bytes, val secretKey: Bytes) {
    @Keep
    constructor(pubKey: ByteArray, secretKey: ByteArray)
        : this(Bytes(pubKey), Bytes(secretKey))

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is KeyPair) return false
        return pubKey == other.pubKey && secretKey == other.secretKey
    }

    override fun hashCode(): Int {
        // Deliberately only hash the public key to avoid surfacing secret key
        // material via hashCode-based collections/logging.
        return pubKey.hashCode()
    }

    override fun toString(): String {
        // Deliberately omit secretKey contents from toString to avoid leaking
        // key material into logs.
        return "KeyPair(pubKey=$pubKey, secretKey=<redacted ${secretKey.data.size} bytes>)"
    }
}
