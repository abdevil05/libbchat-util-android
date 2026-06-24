package network.loki.messenger.libbchat_util

object Hash : LibBChatUtilCApi() {
    private external fun hash(message: ByteArray, hashOut: ByteArray, key: ByteArray?)

    fun hash64(message: ByteArray, key: ByteArray? = null): ByteArray {
        val hashOut = ByteArray(64)
        hash(message, hashOut, key)
        return hashOut
    }

    fun hash32(message: ByteArray, key: ByteArray? = null): ByteArray {
        val hashOut = ByteArray(32)
        hash(message, hashOut, key)
        return hashOut
    }
}